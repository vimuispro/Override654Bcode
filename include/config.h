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
extern pros::Optical colorSensor;
extern double cascadeVal;
extern double cascadeOffsetInches;
extern bool isBlueTeam;
extern pros::Distance left_distance;
extern pros::Distance right_distance;
extern pros::Distance cascadeDistance;
// tracking wheels
extern lemlib::TrackingWheel vertical;
extern lemlib::TrackingWheel cascade;

// extra motors
extern pros::Motor intake_motor;
extern pros::Motor toggle_motor;
extern pros::Motor claw_motor;
extern pros::MotorGroup cascadeMotors;

// pistons
extern pros::adi::DigitalOut clawLift; // piston that moves the claw up/down

constexpr double CASCADE_MAX_HEIGHT_INCHES = 36.0; // measured top of the lift
double getCascadeHeight();
// lift debugging (shown on the brain screen)
extern std::int32_t cascadeRawMm;   // last raw distance sensor reading, in mm
extern int cascadeBadReads;         // bad sensor readings since the last liftTo()
extern const char* liftStopReason;  // why the last liftTo() ended
void calibrateCascadeHeight();


// drivetrain + chassis
extern lemlib::Drivetrain drivetrain;
extern lemlib::ControllerSettings linearController;
extern lemlib::ControllerSettings angularController;
extern lemlib::OdomSensors sensors;
extern lemlib::ExpoDriveCurve throttleCurve;
extern lemlib::ExpoDriveCurve steerCurve;
extern lemlib::Chassis chassis;
extern bool driverControlEnabled;