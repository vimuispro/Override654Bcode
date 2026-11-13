#pragma once
#include <cstdint>
#include "lemlib/api.hpp"
#include "pros/misc.hpp"
#include "pros/rtos.hpp"

class Autons {
public:
    Autons(lemlib::Chassis* chassis);


    

    // ---- Cascade lift (uses the cascade rotation sensor, port 12) ----
    // Heights are in the same units as cascadeVal: 0 = bottom, about 3000 = top.
    void resetLift();                                          // call with the lift at the BOTTOM
    void liftTo(double height, int timeout = 2000, bool wait = true);
    void waitUntilLiftDone();
    void stopLift();                                           // cancel a lift move that's still running
    double liftHeight();
    void toggle(int rotations);
    void openClaw();
    void closeClaw();
    void startOpeningClaw();
    void startClosingClaw();
    void distanceReset(std::string axis, const double field_length);
    void testAuton();      
    void skillsAuton(); 
    
    void auton1();
    void auton2();
    void auton3();

    

private:
    lemlib::Chassis* chassis;
    pros::Mutex clawMutex;
    bool clawMoving = false;
    bool clawTaskStarted = false;
    std::uint32_t clawMoveStartTime = 0;

    void startClawMove(int power);
    void updateClaw();
};
