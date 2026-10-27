#include "lemlib/api.hpp"
#include "driver.h"
#include "config.h"
#include "autons.h"
#include "ui.h"
#include "main.h"
extern Autons autons;

DriverControl::DriverControl(pros::Controller* controller, lemlib::Chassis* chassis)
    : controller(controller), chassis(chassis) {}

void DriverControl::update() {
    static bool drivetrainReversed = false;
    static bool cascadeLimiterEnabled = false;
    static bool cascadeMacroRunning = false;
    static bool claw = false;
    static bool claw2 = true;
    if (controller->get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)) {
        drivetrainReversed = !drivetrainReversed;
    }

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
    driverControlEnabled = true;  // turn on
    }

    if (controller->get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) {
        cascadeEnc.set_position(0);
        cascadeLimiterEnabled = true;
    }
    cascadeVal = cascadeEnc.get_position() / 100;
    
    if (controller->get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
        claw2 = false;
        toggle.set_value(claw2);
        cascadeMacroRunning = cascadeVal < 0;
    }

    if (cascadeMacroRunning) {
        if (cascadeVal < 0) {
            cascadeMotors.move(127);
        } else {
            cascadeMacroRunning = false;
            cascadeMotors.move(0);
        }
        
    } else if (controller->get_digital(pros::E_CONTROLLER_DIGITAL_R2) &&
        (!cascadeLimiterEnabled || cascadeVal < 0)) {
        cascadeMotors.move(127);
    } else if (controller->get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
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

    if (controller->get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
        claw = !claw; // flip the state 
        Claw.set_value(claw);
    }

    if (controller->get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
        claw2 = !claw2; // flip the state 
        toggle.set_value(claw2);
    }
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