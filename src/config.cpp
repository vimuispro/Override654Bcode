#include "config.h"

// controller
pros::Controller controller(pros::E_CONTROLLER_MASTER);

// motor groups
pros::MotorGroup leftMotors({11, -12}, pros::MotorGearset::blue);
pros::MotorGroup rightMotors({19, -20}, pros::MotorGearset::blue);

// sensors
pros::Imu imu(13);
pros::Rotation verticalEnc(10);
pros::Rotation cascadeEnc(14);
int cascadeVal = 0;
// tracking wheels
lemlib::TrackingWheel vertical(&verticalEnc, lemlib::Omniwheel::NEW_2, 0);


// extra motors
pros::Motor intake_motor(4, pros::MotorGearset::blue);
pros::MotorGroup cascadeMotors({17, -18}, pros::MotorGearset::blue);


// pistons
pros::adi::DigitalOut Claw('G');
pros::adi::DigitalOut toggle('H');

bool driverControlEnabled = true;

// drivetrain + chassis
lemlib::Drivetrain drivetrain(&leftMotors, &rightMotors, 11.425,
                              lemlib::Omniwheel::NEW_275, 450, 2);

lemlib::ControllerSettings linearController(
    9,   // Proportional gain (kP)
    0,   // Integral gain (kI)
    170, // Derivative gain (kD)
    3,   // Integral anti-windup error range
    1,   // Small position error threshold (inches)
    100, // Time to remain within the small error threshold (milliseconds)
    3,   // Large position error threshold (inches)
    500, // Time to remain within the large error threshold (milliseconds)
    22   // Maximum acceleration (slew)
);
lemlib::ControllerSettings angularController(
    6,    // Proportional gain (kP)
    0.01, // Integral gain (kI)
    60.2, // Derivative gain (kD)
    3,    // Integral anti-windup error range
    1.5,  // Small heading error threshold (degrees)
    100,  // Time to remain within the small error threshold (milliseconds)
    3,    // Large heading error threshold (degrees)
    500,  // Time to remain within the large error threshold (milliseconds)
    20    // Maximum acceleration (slew)
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