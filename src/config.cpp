#include "config.h"

// controller
pros::Controller controller(pros::E_CONTROLLER_MASTER);

// motor groups
pros::MotorGroup leftMotors({5, -11}, pros::MotorGearset::blue);
pros::MotorGroup rightMotors({10, -6}, pros::MotorGearset::blue);

// sensors
pros::Imu imu(7);
pros::Rotation verticalEnc(-2);
pros::Rotation cascadeEnc(12);
pros::Optical colorSensor(20);
double cascadeVal = 0.0;
bool isBlueTeam = true;

// tracking wheels
lemlib::TrackingWheel vertical(&verticalEnc, lemlib::Omniwheel::NEW_2, 0);

// extra motors
pros::Motor intake_motor(4, pros::MotorGearset::blue);
pros::Motor toggle_motor(-16, pros::MotorGearset::blue);
pros::MotorGroup cascadeMotors({-14, 21, -13}, pros::MotorGearset::blue);

// pistons
pros::adi::DigitalOut Claw('G', true);
pros::adi::DigitalOut toggle('H');

bool driverControlEnabled = true;

// drivetrain + chassis
lemlib::Drivetrain drivetrain(&leftMotors, &rightMotors, 11.425,
                              lemlib::Omniwheel::NEW_275, 450, 2);

lemlib::ControllerSettings linearController(
    12,  // Proportional gain (kP)
    0,   // Integral gain (kI), disabled
    40,  // Derivative gain (kD)
    3,   // Integral anti-windup error range, in inches
    1,   // Small-error range, in inches
    100, // Time to remain within the small-error range before settling, in milliseconds
    3,   // Large-error range, in inches
    500, // Time to remain within the large-error range before settling, in milliseconds
    20   // Maximum acceleration (slew)
);
lemlib::ControllerSettings angularController(
    5,    // Proportional gain (kP)
    0.00, // Integral gain (kI), disabled
    36,   // Derivative gain (kD)
    3,    // Integral anti-windup error range, in degrees
    1.5,  // Small-error range, in degrees
    100,  // Time to remain within the small-error range before settling, in milliseconds
    3,    // Large-error range, in degrees
    500,  // Time to remain within the large-error range before settling, in milliseconds
    0     // Maximum acceleration (slew); 0 disables slew limiting
);

lemlib::OdomSensors sensors(
    &vertical, 
    nullptr, 
    nullptr, //horizontal gone
    nullptr, 
    &imu
);

lemlib::ExpoDriveCurve throttleCurve(
    3, 
    10, 
    1.019
);
lemlib::ExpoDriveCurve steerCurve(
    3, 
    10, 
    1.019
);

lemlib::Chassis chassis(
    drivetrain, 
    linearController, 
    angularController,          
    sensors, 
    &throttleCurve, 
    &steerCurve
);