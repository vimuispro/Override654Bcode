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
extern pros::Distance back_distance;

// ---- Distance-sensor position reset (see Autons::distanceReset) ----
// Where each sensor is on the robot, measured from the TRACKING CENTER (middle of the
// drivetrain) to the FRONT FACE of the sensor, in inches:
//   right   = + if the sensor is right of center, - if left
//   forward = + if the sensor is in front of center, - if behind
// TUNE these by measuring on the robot.
struct DistanceSensorMount {
    double right;
    double forward;
    double angle;   // which way it points, relative to the robot's front: 0 front, 90 right, 180 back, -90 left
};
constexpr DistanceSensorMount LEFT_DISTANCE_MOUNT = { -4.75,  0.0, -90 };  // TUNE
constexpr DistanceSensorMount BACK_DISTANCE_MOUNT = {  -3.4, -2.0, 180 };  // TUNE
// Distance from the field center to the INSIDE face of each wall, in inches.
// TUNE: a VEX field is about 140.4" inside, so 70.2. Measure yours if resets are off by a constant amount.
constexpr double FIELD_WALL = 70.2;
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
constexpr double CASCADE_MIN_HEIGHT_INCHES = 1.0;  // bottom stop height
double getCascadeHeight();
// lift debugging (shown on the brain screen)
extern std::int32_t cascadeRawMm;   // last raw distance sensor reading, in mm
extern int cascadeBadReads;         // bad sensor readings since the last liftTo()
extern const char* liftStopReason;  // why the last liftTo() ended
extern bool cascadeSensorBlocked;   // true while the sensor is ignored because it jumped (sees the goal)
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