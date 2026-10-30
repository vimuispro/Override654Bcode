#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "config.h"
#include "driver.h"
#include "autons.h"
#include "ui.h"
DriverControl driver(&controller, &chassis);
Autons autons(&chassis);



/*
So the coordinate system is:
x = 0 → 479
y = 0 → 239
Top‑left corner = (0, 0)
Bottom‑right corner = (479, 239)
*/


void initialize() {
    pros::lcd::initialize();   // optional, for PROS text
    cascadeMotors.set_brake_mode_all(pros::MotorBrake::hold);
    chassis.calibrate();
    chassis.setPose(0, 0, 0);
    uiInit();                  // ⭐ build your LVGL UI

 }


 

/**
 * Runs while the robot is disabled
 */
void disabled() {}

/**
 * runs after initialize if the robot is connected to field control
 */
void competition_initialize() {}

// get a path used for pure pursuit
// this needs to be put outside a function
ASSET(example_txt); // '.' replaced with "_" to make c++ happy


void  autonomous() {

    switch (selectedAuton) {

        case AUTON_SKILLS:
            autons.skillsAuton();
            break;

        case AUTON_test:
            autons.testAuton();
            break;

        case AUTON_auton1:
            autons.auton1();
            break;

        case AUTON_auton2:
            autons.auton2();
            break;

        case AUTON_auton3:
            autons.auton3();
            break;

        default:
            autons.testAuton(); // fallback
            break;
    }
}
void opcontrol() {
    driver.startPeriod();
    while (true) {
        driver.update();
        pros::delay(15);
        lemlib::Pose pose = chassis.getPose();
        double x = pose.x;         // Current X coordinate (in inches)
        double y = pose.y;   
        controller.print(0, 0, "X: %.1f Y: %.1f", x, y);

    }
}