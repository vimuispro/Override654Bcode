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
lemlib::TrackingWheel cascade(&cascadeEnc, lemlib::Omniwheel::NEW_2, 0);


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
    9, 
    0, 
    170, 
    3, 
    1, 
    100, 
    3, 
    500, 
    22
);
lemlib::ControllerSettings angularController( 
    6, 
    0.01, 
    60.2, 
    3, 
    1.5, 
    100, 
    3, 
    500, 
    20
);

lemlib::OdomSensors sensors(
    &vertical, 
    &cascade, 
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