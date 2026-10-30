#pragma once
#include "pros/apix.h"
#include "lemlib/api.hpp"

// controller
extern pros::Controller controller;

// motor groups
extern pros::MotorGroup leftMotors;
extern pros::MotorGroup rightMotors;

// sensors
extern pros::Imu imu;
extern pros::Rotation verticalEnc;
extern pros::Rotation cascadeEnc;
extern double cascadeVal;

// tracking wheels
extern lemlib::TrackingWheel vertical;
extern lemlib::TrackingWheel cascade;

// extra motors
extern pros::Motor intake_motor;
extern pros::Motor toggle_motor;

extern pros::MotorGroup cascadeMotors;

// pistons
extern pros::adi::DigitalOut Claw;
extern pros::adi::DigitalOut toggle;


// drivetrain + chassis
extern lemlib::Drivetrain drivetrain;
extern lemlib::ControllerSettings linearController;
extern lemlib::ControllerSettings angularController;
extern lemlib::OdomSensors sensors;
extern lemlib::ExpoDriveCurve throttleCurve;
extern lemlib::ExpoDriveCurve steerCurve;
extern lemlib::Chassis chassis;
extern bool driverControlEnabled;
