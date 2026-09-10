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
extern pros::Rotation horizontalEnc;
extern pros::Rotation verticalEnc;

// tracking wheels
extern lemlib::TrackingWheel horizontal;
extern lemlib::TrackingWheel vertical;

// extra motors
//extern pros::Motor front_motor;
//extern pros::Motor back_motor;

// pistons
extern pros::adi::DigitalOut claw;
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

