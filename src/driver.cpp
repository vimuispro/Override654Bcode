#include "lemlib/api.hpp"
#include "driver.h"
#include "config.h"
#include "autons.h"
#include "ui.h"
#include "main.h"
#include <cstring>

extern Autons autons;

DriverControl::DriverControl(pros::Controller* controller, lemlib::Chassis* chassis)
    : controller(controller), chassis(chassis) {}

void DriverControl::startPeriod() {
    cascadeLimiterEnabled = false;
    toggleActive = true;
}

void DriverControl::update() {
    static bool drivetrainReversed = false; // drive motors are fixed in config.cpp, so no flip needed
    static bool cascadeMacroRunning = false;
    static bool clawClosed = true;
    static bool clawLiftState = true; // matches the starting state of clawLift in config.cpp
    if (controller->get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)) {
        drivetrainReversed = !drivetrainReversed;
    }

    if (controller->get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
        isBlueTeam = !isBlueTeam;
    }

    // Team and color are shown on the controller by controllerUiStart() in ui.cpp

    if (driverControlEnabled){
    int rightX = controller->get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
    int leftY = controller->get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    
    if (drivetrainReversed) {
        leftY = -leftY;
    }

    chassis->arcade(leftY, rightX);
    }
    //---------------------------Test----------------------------------------
    // Autonomous turn test on button press up
    if (controller->get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)) {
    driverControlEnabled = false; // turn off driver control 
    autonomous(); // run auton
    // LemLib moves run in the BACKGROUND by default, so autonomous() can return while the
    // last move is still driving. Without this, that move keeps running the drive motors
    // (up to its timeout) and fights the sticks -> the "ghost" driving.
    chassis->waitUntilDone();
    chassis->cancelAllMotions();
    autons.stopLift();            // same for a lift move that's still running
    driverControlEnabled = true;  // turn on
    }

    if (!cascadeLimiterEnabled &&
        controller->get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) {
        cascadeEnc.set_position(0);
        cascadeLimiterEnabled = true;
    }


    cascadeVal = cascadeEnc.get_position() / 100.0;
    
    if (controller->get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
        clawClosed = false;
        autons.startOpeningClaw(); // Y macro opens the claw
        cascadeMacroRunning = cascadeVal > 0;
    }

    if (cascadeMacroRunning) {
        if (cascadeVal > 0) {
            cascadeMotors.move(127);
        } else {
            cascadeMacroRunning = false;
            cascadeMotors.move(0);
        }
        
    } else if (controller->get_digital(pros::E_CONTROLLER_DIGITAL_R2) &&
        (!cascadeLimiterEnabled || cascadeVal > 0)) {
        cascadeMotors.move(127);
    } else if (controller->get_digital(pros::E_CONTROLLER_DIGITAL_R1) &&
        (!cascadeLimiterEnabled || cascadeVal < 3000)) {
        cascadeMotors.move(-127);
    } else {
        cascadeMotors.move(0);
    }
  


    if (controller->get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
        intake_motor.move(127);
    } else if (controller->get_digital(pros::E_CONTROLLER_DIGITAL_L1)){
        intake_motor.move(-127);
    } else {
        intake_motor.move(0);
    }
    pros::delay(25);

    if (controller->get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
        clawClosed = !clawClosed;
        if (clawClosed) {
            autons.startClosingClaw();
        } else {
            autons.startOpeningClaw();
        }
    }

    if (controller->get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
        clawLiftState = !clawLiftState; // B moves the claw up/down
        clawLift.set_value(clawLiftState);
    }
    if (controller->get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
        toggleActive = !toggleActive;
    }

    const char* targetColor = isBlueTeam ? "Blue" : "Red";
    const bool targetColorDetected = std::strcmp(seenColorName(), targetColor) == 0;
    toggle_motor.move(
        toggleActive && !targetColorDetected
            ? 127
            : 0);

}
































    /*
    

    

    static bool wingState = false;
    if (controller->get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
        wingState = !wingState; // flip the state 
        Wing.set_value(wingState);
    }
    
    static bool mlState = false;
    if (controller->get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
        mlState = !mlState; // flip the state 
        ML.set_value(mlState);
    } 

    static bool mgState = true;
    if (controller->get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
        mgState = false; 
        MG.set_value(mgState);
        front_motor.move(107);
        back_motor.move(107);
    }else{
        mgState = true; 
        MG.set_value(mgState);
        if (controller->get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
        front_motor.move(127);
    } else if (controller->get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
        front_motor.move(-127);
    } else {
        front_motor.move(0);
    }

    if (controller->get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
        back_motor.move(127);
    } else if (controller->get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
        back_motor.move(-127);
    } else if (controller->get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
        back_motor.move(25);
    }else {
        back_motor.move(0);
    }

    } 

    static bool mdState = false;
    if (controller->get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
        mdState = !mdState; // flip the state 
        MD.set_value(mdState);
    } 
    

    pros::delay(25);
 }
*/