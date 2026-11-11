#include "config.h"

// controller
pros::Controller controller(pros::E_CONTROLLER_MASTER);

// motor groups
// Real front = the end the robot drives toward when you push the stick forward. These are the old right ports (reversed)
// on the left and the old left ports (reversed) on the right, so LemLib's "forward"
// is the real front and the tracking wheel (port 2, positive) agrees with the motors.
pros::MotorGroup leftMotors({-10, 6}, pros::MotorGearset::blue);
pros::MotorGroup rightMotors({-5, 1}, pros::MotorGearset::blue);

// sensors
pros::Imu imu(7);
pros::Rotation verticalEnc(2); // keep POSITIVE
pros::Rotation cascadeEnc(-12);
pros::Optical colorSensor(20);
double cascadeVal = 0.0;
bool isBlueTeam = true;

// tracking wheels
lemlib::TrackingWheel vertical(
    &verticalEnc,              // V5 rotation sensor
    lemlib::Omniwheel::NEW_2,  // 2-inch tracking wheel diameter
    0.6,                       // offset from the tracking center, in inches
    1.0                        // gear ratio (1:1)
);

// extra motors
pros::Motor intake_motor(4, pros::MotorGearset::blue);
pros::Motor toggle_motor(-16, pros::MotorGearset::blue);
pros::MotorGroup cascadeMotors({-14, 21, -13}, pros::MotorGearset::blue);
pros::Motor claw_motor(8, pros::MotorGearset::blue); 

// pistons
pros::adi::DigitalOut clawLift('H', true); // piston that moves the claw up/down, starts true

bool driverControlEnabled = true;

// drivetrain + chassis
lemlib::Drivetrain drivetrain(
        &leftMotors,
        &rightMotors,
        11.25,
        lemlib::Omniwheel::NEW_275,
        450,
        2
);

lemlib::ControllerSettings linearController(
    12,  // Proportional gain (kP)
    0,   // Integral gain (kI), disabled
    110,  // Derivative gain (kD)
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
    45,   // Derivative gain (kD)
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