#include "autons.h"
#include "config.h"   // gives access to motors, pistons, sensors
#include <cmath>

Autons::Autons(lemlib::Chassis* chassis)
    : chassis(chassis) {}


// ========================================================================
// Cascade lift
// ------------------------------------------------------------------------
// Heights use the cascade rotation sensor, the same numbers as cascadeVal
// in driver control: 0 = bottom, LIFT_MAX = top.
//   autons.liftTo(1200);                 // go to 1200 and WAIT until there
//   autons.liftTo(1800, 2000, false);    // start lifting, keep driving
//   autons.waitUntilLiftDone();          // wait for a lift started with false
// ========================================================================

static const double LIFT_MAX       = 3000; // never go above this (same as driver limit)
static const double LIFT_KP        = 0.5;  // speed per unit of distance left; raise if too slow
static const double LIFT_MIN_SPEED = 25;   // keeps it moving when it's almost there
static const double LIFT_TOLERANCE = 20;   // "close enough" to the target
static const int    LIFT_STALL_MS  = 150;  // stop waiting if the lift hasn't moved for this long

static double liftTarget = 0;
static bool liftActive = false;
static std::uint32_t liftEndTime = 0;
static bool liftTaskStarted = false;
static double liftLastHeight = 0;
static std::uint32_t liftLastMoveTime = 0;

static const int left_sensor_offset = 0; // offset in inches for the left tracking wheel
static const int right_sensor_offset = 0; // offset in inches for the right tracking wheel
/*
void distanceReset(std::string axis, const double field_length){
  double x,y;
  if(axis == "x"){
    double left = left_distance.get()/25.4 + left_sensor_offset;
    double right = right_distance.get()/25.4 + right_sensor_offset;
    x = (left);
    chassis.odom_xyt_set(x, chassis.odom_y_get(), chassis.odom_theta_get());
    master.print(6, 0, "x reset -> %.2f\n", chassis.odom_x_get());
  }
  else if(axis == "y"){
    double left = left_distance.get()/25.4 + left_sensor_offset;
    double right = right_distance.get()/25.4 + right_sensor_offset;
    y = (left);
    chassis.odom_xyt_set(chassis.odom_x_get(), y, chassis.odom_theta_get());

  }
}
  */
double Autons::liftHeight() {
    return cascadeEnc.get_position() / 100.0;
}

void Autons::resetLift() {
    cascadeEnc.set_position(0);   // the lift must be all the way DOWN when this runs
}

void Autons::liftTo(double height, int timeout, bool wait) {
    // clamp the target between the bottom and the top
    if (height < 0) height = 0;
    if (height > LIFT_MAX) height = LIFT_MAX;

    // start the background lift task the first time we need it
    if (!liftTaskStarted) {
        liftTaskStarted = true;
        pros::Task liftTask([] {
            while (true) {
                if (liftActive) {
                    const double height = cascadeEnc.get_position() / 100.0;
                    const double error = liftTarget - height;

                    // remember the last time the lift actually moved
                    if (std::fabs(height - liftLastHeight) > 2) {
                        liftLastHeight = height;
                        liftLastMoveTime = pros::millis();
                    }
                    const bool stalled = pros::millis() - liftLastMoveTime > LIFT_STALL_MS;

                    // done when: close enough, stuck (hard stop / too heavy), or out of time
                    if (std::fabs(error) <= LIFT_TOLERANCE || stalled || pros::millis() > liftEndTime) {
                        cascadeMotors.brake();   // hold mode keeps it at this height
                        liftActive = false;
                    } else {
                        double speed = LIFT_KP * std::fabs(error);
                        if (speed > 127) speed = 127;
                        if (speed < LIFT_MIN_SPEED) speed = LIFT_MIN_SPEED;
                        // negative motor power raises the lift (same as R1 in driver control)
                        cascadeMotors.move(error > 0 ? -speed : speed);
                    }
                }
                pros::delay(10);
            }
        });
    }

    liftTarget = height;
    liftEndTime = pros::millis() + timeout;
    liftLastHeight = cascadeEnc.get_position() / 100.0;
    liftLastMoveTime = pros::millis();   // give it LIFT_STALL_MS to start moving
    liftActive = true;

    if (wait) waitUntilLiftDone();
}

void Autons::stopLift() {
    if (liftActive) {
        liftActive = false;
        cascadeMotors.brake();
    }
}

void Autons::waitUntilLiftDone() {
    while (liftActive) pros::delay(10);
}

void Autons::toggle(int rotations) {
    toggle_motor.move(127);
    pros::delay(200*rotations);
    toggle_motor.move(0);
}

void Autons::openClaw() {
    startOpeningClaw();
}

void Autons::closeClaw() {
    startClosingClaw();
}

void Autons::startClawMove(int power) {
    if (!clawTaskStarted) {
        const pros::task_t task = pros::Task::create([this] {
            while (true) {
                updateClaw();
                pros::delay(10);
            }
        }, "Claw hold");
        if (task == nullptr) {
            pros::lcd::print(0, "Claw timer task failed");
            return;
        }
        clawTaskStarted = true;
    }

    clawMutex.take();
    claw_motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    claw_motor.move(power);
    clawMoveStartTime = pros::millis();
    clawMoving = true;
    clawMutex.give();
}

void Autons::startOpeningClaw() {
    startClawMove(127);
}

void Autons::startClosingClaw() {
    startClawMove(-127);
}

void Autons::updateClaw() {
    clawMutex.take();
    if (clawMoving && pros::millis() - clawMoveStartTime >= 2000) {
        claw_motor.brake();
        clawMoving = false;
    }
    clawMutex.give();
}


void Autons::testAuton() {
      //angular test
    
    //chassis->turnToHeading(90, 1500);
    //pros::delay(500);

    // Back to 0°
    //chassis->turnToHeading(0, 1500);
    //pros::delay(500);

    // 180°
    //chassis->turnToHeading(180, 2000);
    //pros::delay(500);

    // Back to 0°
    //chassis->turnToHeading(0, 2000);
    
    
    //LAteral test
    /*
    chassis->moveToPoint(0, 24, 2000, {.forwards = false, .maxSpeed = 127});
    chassis->waitUntilDone(); // moves run in the background; wait for it to finish

    pros::delay(500);

    //Backward 24"
    chassis->moveToPoint(0, 0, 2000,{.maxSpeed = 127});
    chassis->waitUntilDone(); // moves run in the background; wait for it to finish

    pros::delay(500);

    // Forward 48"
    chassis->moveToPoint(0, 48, 3000, {.forwards = false, .maxSpeed = 127});
    chassis->waitUntilDone(); // moves run in the background; wait for it to 
     
    pros::delay(500);

    chassis->moveToPoint(0, 0, 2000,{.maxSpeed = 127});
    chassis->waitUntilDone(); 
    */
}



void Autons::skillsAuton(){
resetLift();
closeClaw(); // close claw to hold preload
chassis->setPose(-68.039, -0.62, 90);// start facing center

//rotate toggle
toggle(2);


chassis->moveToPoint(-53.929, -19, 600, {.maxSpeed = 90}, true); 
// move to goal and wait

liftTo(1500); // lift cascade over goal
pros::delay(300);
liftTo(0);
pros::delay(50);
openClaw(); // open claw to score
// //score preload
pros::delay(100);
//reverse from the goal
chassis->moveToPoint(-58.929, 0, 600, {.forwards = false}, false);

chassis->turnToPoint(-32.5,-22.9,1000,{},false);// turn toward cup
chassis->moveToPoint(-37+1,-18.7-1,800,{},false);//go to cup
chassis->moveToPoint(-37,-18.7,800,{.maxSpeed = 30},true);//go to cup slowly

pros::delay(100);
closeClaw();//close the claw
pros::delay(100);

liftTo(2000);//lift cascade
//turn to goal
chassis->turnToPoint(-44.3,-24.1,500,{},false);
chassis->moveToPoint(-44.3-5,-24.1,1000,{.maxSpeed = 90},false);
//lower cascade
liftTo(700);
openClaw();
liftTo(0);
//reveres from the goal
chassis->moveToPoint(-25.6, -16.7, 400, {.forwards = false}, false);

chassis->turnToPoint(-51, -42.8, 400, {},false);//turn to cup

chassis->moveToPoint(-51, -42.8, 500, {},false);//go to cup
chassis->moveToPoint(-51, -42.8, 1000, {.maxSpeed = 50},true);//go to cup slowly

pros::delay(600);
closeClaw();
pros::delay(100);

liftTo(2800);//lift cascade

chassis->turnToPoint(-50.3, -30.8, 600, {},false);
chassis->moveToPoint(-50.3, -30.8, 1000, {},false);
liftTo(700);
openClaw();
liftTo(0);
chassis->moveToPoint(-47.4, -42.8, 1000, {.forwards = false},false);
chassis->turnToPoint(-11.3, -9.7, 600, {},false);
chassis->moveToPoint(-11.3, -9.7, 1000, {},false);


}
void Autons::auton1(){

}
void Autons::auton2(){

resetLift();
closeClaw(); // close claw to hold preload

chassis->setPose(-61.648, -0.646, 90);
liftTo(1000, 2000, false);
toggle(2);


chassis->moveToPoint(-46.2, -8.6, 1000,{.maxSpeed = 127, .minSpeed = 80, .earlyExitRange = 5},true);
chassis->moveToPoint(-47.071, -18.193, 1000,{.minSpeed = 100},false);
//chassis->moveToPose(-47.071, -18.193, 180, 5000,{.minSpeed = 100},false);

chassis->tank(10,100,true);
pros::delay(500);
chassis->tank(0,0,true);

liftTo(400);
openClaw();
chassis->setPose(-47, -18.6, chassis->getPose().theta);

chassis->moveToPoint(-47, 0, 2000, {.forwards = false},false);
chassis->moveToPoint(-28.478, -20.31, 500,{.earlyExitRange = 20},true);
chassis->moveToPoint(-27, -21, 900,{.maxSpeed = 50},false);

closeClaw(); 


chassis->moveToPoint(-25, -25, 1000,{.maxSpeed = 127, .minSpeed = 80, .earlyExitRange = 5},true);
liftTo(1500);
chassis->moveToPoint(-41.2, -23.8, 2000,{.minSpeed = 80},false);
chassis->tank(10,120,true);

liftTo(400);
openClaw();

chassis->setPose(-41, -23, chassis->getPose().theta-1);

chassis->moveToPoint(-28, -25, 5000,{.forwards = false},false);
chassis->moveToPoint(-42.555, -41.714, 500,{.earlyExitRange = 20,},true);
chassis->moveToPoint(-44, -43, 2000,{.maxSpeed = 50},false);

closeClaw(); 

liftTo(2000);
chassis->turnToPoint(-47.183, -23, 1000,{},true);
chassis->moveToPoint(-47.183, -23, 1000,{.maxSpeed = 60, .minSpeed = 50},false);
liftTo(1000);
openClaw();

}
void Autons::auton3(){
  
    
}


//--------------------
//Old Stuff---------
//--------------------
/*
resetLift();
closeClaw(); // close claw to hold preload
chassis->setPose(-68.039, -0.62, 90);// start facing center

//rotate toggle
toggle_motor.move(127);
pros::delay(400);
toggle_motor.move(0);


chassis->moveToPoint(-53.929, -19, 600, {.maxSpeed = 90}, true); 
// move to goal and wait

liftTo(1500); // lift cascade over goal
pros::delay(300);
liftTo(0);
pros::delay(50);
openClaw(); // open claw to score
// //score preload
pros::delay(100);
//reverse from the goal
chassis->moveToPoint(-58.929, 0, 600, {.forwards = false}, false);

chassis->turnToPoint(-32.5,-22.9,1000,{},false);// turn toward cup
chassis->moveToPoint(-37+1,-18.7-1,800,{},false);//go to cup
chassis->moveToPoint(-37,-18.7,800,{.maxSpeed = 30},true);//go to cup slowly

pros::delay(100);
closeClaw();//close the claw
pros::delay(100);

liftTo(2000);//lift cascade
//turn to goal
chassis->turnToPoint(-44.3,-24.1,500,{},false);
chassis->moveToPoint(-44.3-5,-24.1,1000,{.maxSpeed = 90},false);
//lower cascade
liftTo(700);
openClaw();
liftTo(0);
//reveres from the goal
chassis->moveToPoint(-25.6, -16.7, 400, {.forwards = false}, false);

chassis->turnToPoint(-51, -42.8, 400, {},false);//turn to cup

chassis->moveToPoint(-51, -42.8, 500, {},false);//go to cup
chassis->moveToPoint(-51, -42.8, 1000, {.maxSpeed = 50},true);//go to cup slowly

pros::delay(600);
closeClaw();
pros::delay(100);

liftTo(2800);//lift cascade

chassis->turnToPoint(-50.3, -30.8, 600, {},false);
chassis->moveToPoint(-50.3, -30.8, 1000, {},false);
liftTo(700);
openClaw();
liftTo(0);
*/


