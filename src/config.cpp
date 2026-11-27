#include "config.h"
#include <cmath>
#include <mutex>

// controller
pros::Controller controller(pros::E_CONTROLLER_MASTER);

// motor groups
// Real front = the end the robot drives toward when you push the stick forward. These are the old right ports (reversed)
// on the left and the old left ports (reversed) on the right, so LemLib's "forward"
// is the real front and the tracking wheel (port 2, positive) agrees with the motors.
pros::MotorGroup leftMotors({-10, 6}, pros::MotorGearset::blue);
pros::MotorGroup rightMotors({-5, 1}, pros::MotorGearset::blue);
pros::Distance left_distance(9);
pros::Distance back_distance(18);
pros::Distance cascadeDistance(17);
// sensors
pros::Imu imu(7);
pros::Rotation verticalEnc(2); // keep POSITIVE
pros::Optical colorSensor(20);
double cascadeVal = 0.0;
bool isBlueTeam = true;
std::int32_t cascadeRawMm = 0;
int cascadeBadReads = 0;
const char* liftStopReason = "-";
bool cascadeSensorBlocked = false;

bool cascadeUsingEncoder = false;

// ---- Cascade height: distance sensor + motor encoder backup ----
// The distance sensor gives the height. When the robot lines up with a goal, the sensor can
// see the goal instead of the lift and the reading jumps 6-7" in ONE reading. The lift can't
// really do that: even when it gets stuck and then falls, it moves at most ~2-2.5" between
// readings, so a fall shows up as several smaller steps and is always trusted. Only a single
// step bigger than SENSOR_JUMP_INCHES counts as "blocked".
//
// While blocked (or when the sensor gives no reading), the height comes from the cascade
// motor encoders instead: last good height + how far the motors turned since then.
// Inches per motor degree is LEARNED automatically while the sensor is reading fine,
// so there's nothing to measure. Once the sensor agrees with the encoder estimate again,
// it's trusted again.
static const double        SENSOR_JUMP_INCHES    = 4.5;   // TUNE: above a real fall step, below the goal jump
static const std::uint32_t SENSOR_BLOCKED_MAX_MS = 3000;  // safety: trust the sensor again after this long
static const double        ENC_MATCH_INCHES      = 2.0;   // sensor trusted again when this close to the encoder estimate
static const double        ENC_LEARN_INCHES      = 3.0;   // learn inches/degree over moves at least this long

static double        lastRawInches = -1;  // previous sensor reading (trusted or not)
static double        trustedRaw    = -1;  // last sensor reading we believed
static double        trustedDeg    = 0;   // motor position at that reading
static double        anchorRaw     = -1;  // start of the current learning window
static double        anchorDeg     = 0;
static double        inchesPerDeg  = 0;   // learned; 0 = not known yet
static std::uint32_t blockedSince  = 0;
static pros::Mutex   heightMutex;         // several tasks read the height

static double rawToHeight(double rawInches) {
    double height = std::fabs(rawInches - CASCADE_ZERO_INCHES);
    if (height < 0.0) height = 0.0;
    if (height > CASCADE_MAX_HEIGHT_INCHES) height = CASCADE_MAX_HEIGHT_INCHES;
    return height;
}

double getCascadeHeight() {
    std::lock_guard<pros::Mutex> lock(heightMutex);
    const std::uint32_t now = pros::millis();

    const double deg = cascadeMotors.get_position();
    const bool encoderOk = deg != PROS_ERR_F;
    const bool canEstimate = encoderOk && inchesPerDeg != 0 && trustedRaw >= 0;
    const double estimateRaw = canEstimate ? trustedRaw + (deg - trustedDeg) * inchesPerDeg : 0;

    const std::int32_t readingMm = cascadeDistance.get();
    cascadeRawMm = readingMm;
    if (readingMm == PROS_ERR || readingMm <= 0 || readingMm >= 9999) {
        ++cascadeBadReads;   // sensor can't see the lift
        cascadeUsingEncoder = canEstimate;
        if (canEstimate) cascadeVal = rawToHeight(estimateRaw);
        return cascadeVal;   // (without an estimate: keep the last good height)
    }
    const double raw = static_cast<double>(readingMm) / 25.4;

    if (lastRawInches < 0) {   // first reading
        lastRawInches = raw;
        trustedRaw = raw;
        trustedDeg = deg;
    }
    const bool bigJump = std::fabs(raw - lastRawInches) > SENSOR_JUMP_INCHES;
    lastRawInches = raw;

    if (!cascadeSensorBlocked) {
        if (bigJump) {
            cascadeSensorBlocked = true;   // probably seeing the goal
            blockedSince = now;
        }
    } else {
        const bool backToNormal = canEstimate
            ? std::fabs(raw - estimateRaw) <= ENC_MATCH_INCHES                       // agrees with the encoders
            : (bigJump || std::fabs(raw - trustedRaw) <= SENSOR_JUMP_INCHES);        // no encoder estimate yet
        if (backToNormal || now - blockedSince > SENSOR_BLOCKED_MAX_MS) {
            cascadeSensorBlocked = false;
        }
    }

    if (cascadeSensorBlocked) {
        cascadeUsingEncoder = canEstimate;
        if (canEstimate) cascadeVal = rawToHeight(estimateRaw);
        return cascadeVal;
    }

    // trusted reading
    cascadeUsingEncoder = false;
    trustedRaw = raw;
    if (encoderOk) {
        trustedDeg = deg;
        // learn inches per motor degree from real moves
        if (anchorRaw < 0) {
            anchorRaw = raw;
            anchorDeg = deg;
        } else if (std::fabs(raw - anchorRaw) >= ENC_LEARN_INCHES && std::fabs(deg - anchorDeg) > 10) {
            const double sample = (raw - anchorRaw) / (deg - anchorDeg);
            const bool plausible = inchesPerDeg == 0 ||
                (sample * inchesPerDeg > 0 &&                                   // same direction
                 std::fabs(sample - inchesPerDeg) < 0.5 * std::fabs(inchesPerDeg)); // not wildly different
            if (plausible) {
                inchesPerDeg = inchesPerDeg == 0 ? sample : 0.7 * inchesPerDeg + 0.3 * sample;
            }
            anchorRaw = raw;
            anchorDeg = deg;
        }
    }
    cascadeVal = rawToHeight(raw);
    return cascadeVal;
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