#pragma once
#include <cstdint>
#include "lemlib/api.hpp"
#include "pros/misc.hpp"
#include "pros/rtos.hpp"

class Autons {
public:
    Autons(lemlib::Chassis* chassis);

    // ---- Cascade lift (uses the distance sensor, port 18) ----
    // Heights are in inches: 0 = calibrated bottom, 36 = top.
    void resetLift();                                          // call with the lift at the BOTTOM
    void liftTo(double height, int timeout = 2000, bool wait = true, int maxSpeed = 127);
    void waitUntilLiftDone();
    void stopLift();                                           // cancel a lift move that's still running
    double liftHeight();

    void toggle(int rotations);

    // ---- Claw ----
    // openClaw / closeClaw start the claw and WAIT waitMs so the claw has actually
    // gripped / released before the next step. Pass 0 to not wait.
    // startOpeningClaw / startClosingClaw never wait (used by driver control).
    void openClaw(int waitMs = CLAW_WAIT_MS);
    void closeClaw(int waitMs = CLAW_WAIT_MS);
    void startOpeningClaw();
    void startClosingClaw();

    // ---- Drive helpers ----
    // Drive the motors directly for ms milliseconds, then STOP.
    // Use this instead of chassis->tank() so the robot never keeps driving by accident.
    void pushFor(int leftPower, int rightPower, int ms);

    // ---- Position reset from the distance sensors ----
    // Call when the robot is STOPPED and a sensor faces a field wall. Each sensor that
    // can see a wall straight on fixes X or Y (whichever that wall gives). Heading is not changed.
    // Returns how many sensors were used (0, 1 or 2).
    int distanceReset();

    // ---- Routines ----
    void testAuton();
    void skillsAuton();

    void auton1();   // LEFT side
    void auton2();   // RIGHT side (just the shared right-side opening)
    void auton3();   // RIGHT side opening + extra scoring
    void auton4();   // lift test: raise lift to the top

    // how long openClaw / closeClaw wait by default (tune this on the robot)
    static constexpr int CLAW_WAIT_MS = 250;

private:
    lemlib::Chassis* chassis;
    pros::Mutex clawMutex;
    bool clawMoving = false;
    bool clawTaskStarted = false;
    std::uint32_t clawMoveStartTime = 0;

    void startClawMove(int power);
    void updateClaw();

    // first part of auton2 / skills (identical in both, so it lives in one place)
    void rightSideOpening();
    // preload onto the first goal; mirror = false for auton2/right, true for auton1/left
    void goalStart(bool mirror);
};
