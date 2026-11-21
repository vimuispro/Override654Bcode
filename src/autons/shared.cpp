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
static const double LIFT_KP        = 4.0;  // speed per inch of distance left
static const double LIFT_MIN_SPEED = 25;   // keeps it moving when it's almost there
static const double LIFT_TOLERANCE = 0.5; // "close enough" to the target
static const int    LIFT_STALL_MS  = 150;  // stop waiting if the lift hasn't moved for this long

static double liftTarget = 0;
static int liftMaxSpeed = 127;
static bool liftActive = false;
static std::uint32_t liftEndTime = 0;
static bool liftTaskStarted = false;
static double liftLastHeight = 0;
static std::uint32_t liftLastMoveTime = 0;

static const int left_sensor_offset = 0; // offset in inches for the left tracking wheel
static const int right_sensor_offset = 0; // offset in inches for the right tracking wheel
/*
void distanceReset(std::string axis, const double field_length){
  double x,y;
  if(axis == "x"){
    double left = left_distance.get()/25.4 + left_sensor_offset;
    double right = right_distance.get()/25.4 + right_sensor_offset;
    x = (left);
    chassis.odom_xyt_set(x, chassis.odom_y_get(), chassis.odom_theta_get());
    master.print(6, 0, "x reset -> %.2f\n", chassis.odom_x_get());
  }
  else if(axis == "y"){
    double left = left_distance.get()/25.4 + left_sensor_offset;
    double right = right_distance.get()/25.4 + right_sensor_offset;
    y = (left);
    chassis.odom_xyt_set(chassis.odom_x_get(), y, chassis.odom_theta_get());

  }
}
  */
double Autons::liftHeight() {
    return getCascadeHeight();
}

void Autons::resetLift() {
    calibrateCascadeHeight(); // the lift must be all the way DOWN when this runs
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
                    const bool bottomTarget = liftTarget <= 0;

                    // remember the last time the lift actually moved
                    if (std::fabs(height - liftLastHeight) > 0.25) {
                        liftLastHeight = height;
                        liftLastMoveTime = pros::millis();
                    }
                    const bool stalled = pros::millis() - liftLastMoveTime > LIFT_STALL_MS;

                    // done when: close enough, stuck (hard stop / too heavy), or out of time
                    const bool targetReached = bottomTarget
                        ? height <= LIFT_TOLERANCE
                        : std::fabs(error) <= LIFT_TOLERANCE;
                    if (targetReached || stalled || pros::millis() > liftEndTime) {
                        cascadeMotors.brake();   // hold mode keeps it at this height
                        liftActive = false;
                    } else {
                        double speed = LIFT_KP * std::fabs(error);
                        if (speed > liftMaxSpeed) speed = liftMaxSpeed;
                        const double minSpeed = liftMaxSpeed < LIFT_MIN_SPEED
                            ? liftMaxSpeed
                            : LIFT_MIN_SPEED;
                        if (speed < minSpeed) speed = minSpeed;
                        // negative motor power raises the lift (same as R1 in driver control)
                        cascadeMotors.move(bottomTarget ? speed : (error > 0 ? -speed : speed));
                    }
                }
                pros::delay(10);
            }
        });
    }

    liftTarget = height;
    liftMaxSpeed = maxSpeed;
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

void Autons::toggle(int rotations) {
    toggle_motor.move(127);
    pros::delay(200*rotations);
    toggle_motor.move(0);
}

void Autons::openClaw() {
    startOpeningClaw();
}

void Autons::closeClaw() {
    startClosingClaw();
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

//--------------------
//Old Stuff---------
//--------------------
/*
resetLift();
closeClaw(); // close claw to hold preload
chassis->setPose(-68.039, -0.62, 90);// start facing center

//rotate toggle
toggle_motor.move(127);
pros::delay(400);
toggle_motor.move(0);


chassis->moveToPoint(-53.929, -19, 600, {.maxSpeed = 90}, true); 
// move to goal and wait

liftTo(20.0); // lift cascade over goal
pros::delay(300);
liftTo(0);
pros::delay(50);
openClaw(); // open claw to score
// //score preload
pros::delay(100);
//reverse from the goal
chassis->moveToPoint(-58.929, 0, 600, {.forwards = false}, false);

chassis->turnToPoint(-32.5,-22.9,1000,{},false);// turn toward cup
chassis->moveToPoint(-37+1,-18.7-1,800,{},false);//go to cup
chassis->moveToPoint(-37,-18.7,800,{.maxSpeed = 30},true);//go to cup slowly

pros::delay(100);
closeClaw();//close the claw
pros::delay(100);

liftTo(26.67);//lift cascade
//turn to goal
chassis->turnToPoint(-44.3,-24.1,500,{},false);
chassis->moveToPoint(-44.3-5,-24.1,1000,{.maxSpeed = 90},false);
//lower cascade
liftTo(9.33);
openClaw();
liftTo(0);
//reveres from the goal
chassis->moveToPoint(-25.6, -16.7, 400, {.forwards = false}, false);

chassis->turnToPoint(-51, -42.8, 400, {},false);//turn to cup

chassis->moveToPoint(-51, -42.8, 500, {},false);//go to cup
chassis->moveToPoint(-51, -42.8, 1000, {.maxSpeed = 50},true);//go to cup slowly

pros::delay(600);
closeClaw();
pros::delay(100);

liftTo(37.33);//lift cascade

chassis->turnToPoint(-50.3, -30.8, 600, {},false);
chassis->moveToPoint(-50.3, -30.8, 1000, {},false);
liftTo(9.33);
openClaw();
liftTo(0);
*/


