#pragma once
#include <cstdint>
#include "lemlib/api.hpp"
#include "pros/misc.hpp"
#include "pros/rtos.hpp"

class Autons {
public:
    Autons(lemlib::Chassis* chassis);


    

    // ---- Cascade lift (uses the distance sensor, port 18) ----
    // Heights are in inches: 0 = calibrated bottom, 40 = top.
    void resetLift();                                          // call with the lift at the BOTTOM
    void liftTo(double height, int timeout = 2000, bool wait = true, int maxSpeed = 127);
    void waitUntilLiftDone();
    void stopLift();                                           // cancel a lift move that's still running
    double liftHeight();
    void toggle(int rotations);
    void openClaw();
    void closeClaw();
    void startOpeningClaw();
    void startClosingClaw();
    void testAuton();      
    void skillsAuton(); 
    
    void auton1();
    void auton2();
    void auton3();
    void auton4();

    

private:
    lemlib::Chassis* chassis;
    pros::Mutex clawMutex;
    bool clawMoving = false;
    bool clawTaskStarted = false;
    std::uint32_t clawMoveStartTime = 0;

    void startClawMove(int power);
    void updateClaw();
};
