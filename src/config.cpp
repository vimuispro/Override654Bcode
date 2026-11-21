#include "config.h"
#include <cmath>

// controller
pros::Controller controller(pros::E_CONTROLLER_MASTER);

// motor groups
// Real front = the end the robot drives toward when you push the stick forward. These are the old right ports (reversed)
// on the left and the old left ports (reversed) on the right, so LemLib's "forward"
// is the real front and the tracking wheel (port 2, positive) agrees with the motors.
pros::MotorGroup leftMotors({-10, 6}, pros::MotorGearset::blue);
pros::MotorGroup rightMotors({-5, 1}, pros::MotorGearset::blue);
pros::Distance left_distance(0);
pros::Distance right_distance(0);
pros::Distance cascadeDistance(17);
// sensors
pros::Imu imu(7);
pros::Rotation verticalEnc(2); // keep POSITIVE
pros::Optical colorSensor(20);
double cascadeVal = 0.0;
double cascadeOffsetInches = 0.0;
bool isBlueTeam = true;
std::int32_t cascadeRawMm = 0;
int cascadeBadReads = 0;
const char* liftStopReason = "-";

double getCascadeHeight() {
    const std::int32_t readingMm = cascadeDistance.get();
    cascadeRawMm = readingMm;
    if (readingMm == PROS_ERR || readingMm <= 0 || readingMm >= 9999) {
        ++cascadeBadReads;   // sensor can't see the lift: reuse the last good height
        return cascadeVal;
    }

    const double readingInches = static_cast<double>(readingMm) / 25.4;
    double height = std::fabs(readingInches - cascadeOffsetInches);
    if (height < 0.0) height = 0.0;
    if (height > CASCADE_MAX_HEIGHT_INCHES) height = CASCADE_MAX_HEIGHT_INCHES;
    // keep the value shown on the screen fresh. Before, only driver control updated it,
    // so during an auton the screen stayed stuck at 0.
    cascadeVal = height;
    return height;
}

void calibrateCascadeHeight() {
    double sum = 0.0;
    int samples = 0;

    for (int i = 0; i < 10; ++i) {
        const std::int32_t readingMm = cascadeDistance.get();
        if (readingMm != PROS_ERR && readingMm > 0 && readingMm < 9999) {
            sum += static_cast<double>(readingMm) / 25.4;
            ++samples;
        }
        pros::delay(10);
    }

    if (samples > 0) {
        cascadeOffsetInches = sum / samples;
    }
    cascadeVal = getCascadeHeight();
}

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
    250, // Time to remain within the large-error range before settling, in milliseconds
         // (was 500: any move that stopped 1-3" short sat still for half a second)
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
    250,  // Time to remain within the large-error range before settling, in milliseconds
          // (was 500: any turn that stopped 1.5-3 deg short sat still for half a second)
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