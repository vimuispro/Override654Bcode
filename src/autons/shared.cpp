#include "autons.h"
#include "config.h"   // gives access to motors, pistons, sensors
#include <cmath>

Autons::Autons(lemlib::Chassis* chassis)
    : chassis(chassis) {}


// ========================================================================
// Cascade lift
// ------------------------------------------------------------------------
// Heights use the cascade distance sensor, in inches:
// 0 = calibrated bottom, LIFT_MAX = top.
//   autons.liftTo(16);                  // go to 16 inches and WAIT until there
//   autons.liftTo(24, 2000, false);     // start lifting, keep driving
//   autons.liftTo(24, 2000, true, 80);  // cap lift power at 80
//   autons.liftTo(0);                    // lower past zero, or stop at the mechanical limit
//   autons.waitUntilLiftDone();          // wait for a lift started with false
// ========================================================================

static const double LIFT_MAX       = CASCADE_MAX_HEIGHT_INCHES;
// Going UP the lift always uses full power (liftMaxSpeed): any less and it can't lift itself near
// the top (it stalled at 33" even with 60). Gravity stops it fast when the power cuts, so it
// doesn't overshoot much. If it DOES go past the target going up, raise LIFT_UP_EARLY_STOP.
static const double LIFT_UP_EARLY_STOP = 0.0;  // inches before the target to cut power going up
// Going DOWN it slows near the target but keeps at least LIFT_MIN_SPEED_DOWN power.
// 35 was too weak: the lift stopped moving before reaching its height.
// TUNE: if it still stops short going down, raise LIFT_MIN_SPEED_DOWN; if it goes past, lower it.
static const double LIFT_KP_DOWN        = 10.0;  // speed per inch left, going down
static const double LIFT_MIN_SPEED_DOWN = 70;
static const double LIFT_TOLERANCE = 0.5;  // "close enough" to the target
// Stall check: if the lift moves less than LIFT_STALL_DIST inches in LIFT_STALL_MS, it's treated
// as stuck and the move ends. Was 150ms, which ended slow moves early at the wrong height.
static const double LIFT_STALL_DIST = 0.25;
static const int    LIFT_STALL_MS   = 300;

static double liftTarget = 0;
static int liftMaxSpeed = 127;
static bool liftActive = false;
static std::uint32_t liftEndTime = 0;
static bool liftTaskStarted = false;
static double liftLastHeight = 0;
static std::uint32_t liftLastMoveTime = 0;
static bool liftGoingUp = false;   // direction is picked once when liftTo() starts

double Autons::liftHeight() {
    return getCascadeHeight();
}

// Sets the current lift position as height 0. Only does it if the lift looks like it's at
// (or below) the bottom: if an earlier run left the lift UP, re-zeroing there would make
// the code think the top is 0, and then it only ever tries to go up.
void Autons::resetLift() {
    const std::int32_t readingMm = cascadeDistance.get();
    const bool goodReading = readingMm != PROS_ERR && readingMm > 0 && readingMm < 9999;
    const bool calibratedBefore = cascadeOffsetInches > 0;
    if (goodReading && calibratedBefore &&
        static_cast<double>(readingMm) / 25.4 > cascadeOffsetInches + 2.0) {
        liftStopReason = "not reset: lift was up";  // keep the old zero
        return;
    }
    calibrateCascadeHeight();
}

void Autons::liftTo(double height, int timeout, bool wait, int maxSpeed) {
    // clamp the target between the bottom and the top
    if (height < 0) height = 0;
    if (height > LIFT_MAX) height = LIFT_MAX;
    if (maxSpeed < 0) maxSpeed = 0;
    if (maxSpeed > 127) maxSpeed = 127;

    // start the background lift task the first time we need it
    if (!liftTaskStarted) {
        liftTaskStarted = true;
        pros::Task liftTask([] {
            while (true) {
                if (liftActive) {
                    const double height = getCascadeHeight();
                    const double error = liftTarget - height;

                    // remember the last time the lift actually moved
                    if (std::fabs(height - liftLastHeight) > LIFT_STALL_DIST) {
                        liftLastHeight = height;
                        liftLastMoveTime = pros::millis();
                    }
                    // while the sensor is blocked the height is frozen, so don't call that a stall
                    if (cascadeSensorBlocked) liftLastMoveTime = pros::millis();
                    const bool stalled = pros::millis() - liftLastMoveTime > LIFT_STALL_MS;

                    // done when: close enough, stuck (hard stop / too heavy), or out of time
                    // The move only ever goes ONE way. Once the lift reaches or passes the target
                    // it stops and holds; it never turns around (that caused bouncing).
                    const bool goingUp = liftGoingUp;
                    const bool targetReached = goingUp
                        ? height >= liftTarget - LIFT_TOLERANCE - LIFT_UP_EARLY_STOP
                        : height <= liftTarget + LIFT_TOLERANCE;
                    if (targetReached || stalled || pros::millis() > liftEndTime) {
                        liftStopReason = targetReached ? "reached" : (stalled ? "stalled" : "timeout");
                        cascadeMotors.brake();   // hold mode keeps it at this height
                        liftActive = false;
                    } else {
                        double speed = liftMaxSpeed;   // going up: full power the whole way
                        if (!goingUp) {
                            // going down: slow down near the target (but never below LIFT_MIN_SPEED_DOWN)
                            speed = LIFT_KP_DOWN * std::fabs(error);
                            if (speed > liftMaxSpeed) speed = liftMaxSpeed;
                            const double minSpeed = liftMaxSpeed < LIFT_MIN_SPEED_DOWN
                                ? liftMaxSpeed : LIFT_MIN_SPEED_DOWN;
                            if (speed < minSpeed) speed = minSpeed;
                        }
                        // negative motor power raises the lift (same as R1 in driver control)
                        cascadeMotors.move(goingUp ? -speed : speed);
                    }
                }
                pros::delay(10);
            }
        });
    }

    liftTarget = height;
    liftMaxSpeed = maxSpeed;
    cascadeBadReads = 0;
    liftStopReason = "moving";
    liftGoingUp = height > getCascadeHeight();
    liftEndTime = pros::millis() + timeout;
    liftLastHeight = getCascadeHeight();
    liftLastMoveTime = pros::millis();   // give it LIFT_STALL_MS to start moving
    liftActive = true;

    if (wait) waitUntilLiftDone();
}

void Autons::stopLift() {
    if (liftActive) {
        liftActive = false;
        cascadeMotors.brake();
    }
}

void Autons::waitUntilLiftDone() {
    while (liftActive) pros::delay(10);
}

// Blocks: the drivetrain must NOT move while the toggle motor is flipping the toggle.
// Start the lift and claw BEFORE calling this so they move during the flip.
void Autons::toggle(int rotations) {
    toggle_motor.move(127);
    pros::delay(200 * rotations);
    toggle_motor.move(0);
}

void Autons::openClaw(int waitMs) {
    startOpeningClaw();
    if (waitMs > 0) pros::delay(waitMs);  // let it actually release before driving away
}

void Autons::closeClaw(int waitMs) {
    startClosingClaw();
    if (waitMs > 0) pros::delay(waitMs);  // let it actually grip before lifting / driving
}

void Autons::pushFor(int leftPower, int rightPower, int ms) {
    chassis->tank(leftPower, rightPower, true);
    pros::delay(ms);
    chassis->tank(0, 0, true);
}

void Autons::startClawMove(int power) {
    if (!clawTaskStarted) {
        const pros::task_t task = pros::Task::create([this] {
            while (true) {
                updateClaw();
                pros::delay(10);
            }
        }, "Claw hold");
        if (task == nullptr) {
            pros::lcd::print(0, "Claw timer task failed");
            return;
        }
        clawTaskStarted = true;
    }

    clawMutex.take();
    claw_motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    claw_motor.move(power);
    clawMoveStartTime = pros::millis();
    clawMoving = true;
    clawMutex.give();
}

void Autons::startOpeningClaw() {
    startClawMove(127);
}

void Autons::startClosingClaw() {
    startClawMove(-127);
}

void Autons::updateClaw() {
    clawMutex.take();
    if (clawMoving && pros::millis() - clawMoveStartTime >= 2000) {
        claw_motor.brake();
        clawMoving = false;
    }
    clawMutex.give();
}

// ========================================================================
// Shared right-side opening
// ------------------------------------------------------------------------
// auton2 and skillsAuton both start with exactly this. Fix/tune it HERE and
// both routines get the change.
// ========================================================================
void Autons::rightSideOpening() {
    resetLift();
    closeClaw(0); // close claw on the preload; it keeps closing while the robot starts moving

    chassis->setPose(-61.648, -0.646, 90);
    liftTo(4.3, 2000, false);
    toggle(2);

    chassis->moveToPoint(-45, -8.6, 1000, {.maxSpeed = 100, .minSpeed = 80, .earlyExitRange = 5}, true);
    chassis->moveToPoint(-47.5, -18.193, 1000, {.maxSpeed = 100, .minSpeed = 50}, false);

    pushFor(80, 80, 500);  // square up on the goal

    liftTo(0);
    openClaw();
    
    chassis->setPose(-47, -18.6, chassis->getPose().theta);

    chassis->moveToPoint(-47, 0, 900, { .forwards = false, .maxSpeed = 100}, false);
    // fast approach, then exit 20" out and finish slowly
    // (earlyExitRange does nothing without minSpeed, so before this the move just ran out its 500ms timeout)
    chassis->moveToPoint(-28.478, -20.31, 1000, {.minSpeed = 50, .earlyExitRange = 20}, true);
    chassis->moveToPoint(-27, -21, 1000, {.maxSpeed = 50}, false);

    closeClaw();

    chassis->moveToPoint(-25, -25, 1000, {.maxSpeed = 127, .minSpeed = 80, .earlyExitRange = 1}, true);
    liftTo(7.0);
    chassis->moveToPoint(-41.2, -23.8, 2000, {.minSpeed = 80}, false);

    // swing into the goal while the lift comes down. Before, the swing ran for however long
    // the lift took and was never stopped. TUNE the 300ms on the robot.
    liftTo(4.33, 2000, false);
    pushFor(10, 120, 300);
    waitUntilLiftDone();
    openClaw();

    chassis->setPose(-41, -23, chassis->getPose().theta);

    liftTo(0, 2000, false);

    chassis->moveToPoint(-20, -24, 1500, {.forwards = false, .maxSpeed = 100}, false);  // timeout was 5000
    chassis->moveToPoint(-35, -35, 1000, {.maxSpeed = 100, .minSpeed = 50, .earlyExitRange = 5}, true);
    chassis->moveToPoint(-42, -42, 2000, {.maxSpeed = 50}, false);

    closeClaw();

    // lift while turning instead of standing still, but make sure it's up before driving in
    liftTo(14, 2000, false);
    chassis->turnToPoint(-47.183, -23, 1000, {}, false);
    waitUntilLiftDone();
    chassis->moveToPoint(-47.183, -23, 1000, {.maxSpeed = 60, .minSpeed = 50}, false);
    liftTo(7, 2000, false);
    pushFor(127, -10, 800);
    waitUntilLiftDone();
    openClaw();
    
}

// ========================================================================
// Position reset from the distance sensors
// ------------------------------------------------------------------------
// The sensor measures how far it is from a wall. We know where the walls are
// (FIELD_WALL), where the sensor is on the robot (the *_MOUNT values in config.h)
// and which way the robot faces (IMU), so we can work out the robot's X or Y.
//
//   chassis->moveToPoint(...);   // stop somewhere a sensor faces a wall
//   distanceReset();             // fixes X and/or Y
// ========================================================================

static const double DR_MAX_ANGLE_OFF   = 20.0;  // skip if the sensor isn't within 20 deg of facing a wall
static const double DR_MAX_RANGE       = 80.0;  // inches; past this the reading is too noisy
static const double DR_MAX_CORRECTION  = 8.0;   // skip if it would move the pose more than this (seeing a robot/goal, not the wall)
static const int    DR_MIN_CONFIDENCE  = 30;    // 0-63; only meaningful past ~8"

static double degToRad(double d) { return d * M_PI / 180.0; }

// Works out the new X or Y from one sensor. Returns false if it can't be trusted.
static bool distanceFix(pros::Distance& sensor, const DistanceSensorMount& mount,
                        const lemlib::Pose& pose, bool& fixesX, double& value) {
    // average a few readings
    double sum = 0;
    int count = 0;
    for (int i = 0; i < 3; ++i) {
        const std::int32_t mm = sensor.get();
        const std::int32_t conf = sensor.get_confidence();
        const bool valid = mm != PROS_ERR && mm > 0 && mm < 9999 &&
                           (mm < 200 || conf == PROS_ERR || conf >= DR_MIN_CONFIDENCE);
        if (valid) { sum += mm / 25.4; ++count; }
        pros::delay(15);
    }
    if (count == 0) return false;
    const double reading = sum / count;
    if (reading > DR_MAX_RANGE) return false;

    // LemLib heading: 0 = +Y, 90 = +X (clockwise)
    const double h = degToRad(pose.theta);
    // sensor position relative to the tracking center, in field directions
    const double offX = mount.right * std::cos(h) + mount.forward * std::sin(h);
    const double offY = -mount.right * std::sin(h) + mount.forward * std::cos(h);
    // direction the beam points on the field
    const double a = degToRad(pose.theta + mount.angle);
    const double dirX = std::sin(a);
    const double dirY = std::cos(a);

    const double minCos = std::cos(degToRad(DR_MAX_ANGLE_OFF));
    if (std::fabs(dirX) >= minCos) {
        // facing the +X or -X wall
        const double wall = dirX > 0 ? FIELD_WALL : -FIELD_WALL;
        value = wall - reading * dirX - offX;
        fixesX = true;
        return std::fabs(value - pose.x) <= DR_MAX_CORRECTION;
    }
    if (std::fabs(dirY) >= minCos) {
        // facing the +Y or -Y wall
        const double wall = dirY > 0 ? FIELD_WALL : -FIELD_WALL;
        value = wall - reading * dirY - offY;
        fixesX = false;
        return std::fabs(value - pose.y) <= DR_MAX_CORRECTION;
    }
    return false;  // pointing at a corner / at an angle
}

int Autons::distanceReset() {
    lemlib::Pose pose = chassis->getPose();
    int used = 0;

    bool fixesX = false;
    double value = 0;
    if (distanceFix(left_distance, LEFT_DISTANCE_MOUNT, pose, fixesX, value)) {
        (fixesX ? pose.x : pose.y) = value;
        ++used;
    }
    if (distanceFix(back_distance, BACK_DISTANCE_MOUNT, pose, fixesX, value)) {
        (fixesX ? pose.x : pose.y) = value;
        ++used;
    }

    if (used > 0) chassis->setPose(pose.x, pose.y, pose.theta);
    return used;
}
