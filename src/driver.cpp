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
  

    if (driverControlEnabled){
    int rightX = controller->get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
    int leftY = controller->get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    

    chassis->arcade(leftY, rightX);
    }
    //---------------------------Test----------------------------------------
    // Autonomous turn test on button press up
    if (controller->get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)) {
    driverControlEnabled = false; // turn off driver control 
    autonomous(); // run auton 
    driverControlEnabled = true;  // turn on
    }

    if (controller->get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)){
        cascade_motor1.move(127)
        cascade_motor2.move(-127)
    }
    if (controller->get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)){
        cascade_motor1.move(-127)
        cascade_motor2.move(127)
    }
    if (controller->get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)){
        
    }
    if (controller->get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)){
        
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