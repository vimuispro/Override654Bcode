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

void Autons::score(bool on){
    
    
}

void Autons::openClaw() {
    claw_motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    claw_motor.move(127);
    pros::delay(100);
    claw_motor.brake();
}

void Autons::closeClaw() {
    claw_motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    claw_motor.move(-127);
    pros::delay(100);
    claw_motor.brake();
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
    chassis->moveToPoint(0, 24, 2000, {.forwards = false, .maxSpeed = 127});
    chassis->waitUntilDone(); // moves run in the background; wait for it to finish

    pros::delay(500);

    // Backward 24"
    chassis->moveToPoint(0, 0, 2000,{.maxSpeed = 127});
    chassis->waitUntilDone(); // moves run in the background; wait for it to finish

    pros::delay(500);

    // Forward 48"
    chassis->moveToPoint(0, 48, 3000, {.forwards = false, .maxSpeed = 127});
    chassis->waitUntilDone(); // moves run in the background; wait for it to finish

    pros::delay(500);

    // Back to start
    chassis->moveToPoint(0, 0, 3000,{.maxSpeed = 127});
    chassis->waitUntilDone(); // moves run in the background; wait for it to finish

    pros::delay(800);
    
}



void Autons::skillsAuton(){


}
void Autons::auton1(){
resetLift();
closeClaw(); // close claw to hold preload
chassis->setPose(-68.039, -0.62, 90);// start facing center

//rotate toggle
toggle_motor.move(127);
pros::delay(300);
toggle_motor.move(0);


chassis->moveToPoint(-53.929, -19, 800, {.maxSpeed = 127}, true); 
// move to goal and wait
liftTo(700); // lift cascade over goal
pros::delay(300);
liftTo(0);
openClaw(); // open claw to score
// //score preload
pros::delay(100);
//reverse from the goal
chassis->moveToPoint(-58.929, 0, 600, {.forwards = false}, false);

chassis->turnToPoint(-23,-34,1000,{},false);// turn toward cup
chassis->moveToPoint(-23-20,-34+20,800,{},false);//go to cup
chassis->moveToPoint(-23-20+9,-34+20-9,700,{.maxSpeed = 60},true);//go to cup slowly

pros::delay(500);
closeClaw();//close the claw
pros::delay(100);

liftTo(1200);//lift cascade
//turn to goal
chassis->turnToPoint(-44.3,-24.1,500,{},false);
chassis->moveToPoint(-44.3-5,-24.1,500,{},false);
//lower cascade
liftTo(300);
openClaw();
liftTo(0);
//reveres from the goal
chassis->moveToPoint(-25.6, -16.7, 400, {.forwards = false}, false);

chassis->turnToPoint(-48-4, -46.7, 400, {},false);//turn to cup
chassis->moveToPoint(-48-4, -46.7, 500, {},false);//go to cup
chassis->moveToPoint(-48-4+1, -46.7+1, 1800, {.maxSpeed = 60},false);//go to cup slowly
pros::delay(200);
closeClaw();
pros::delay(100);

liftTo(2000);//lift cascade

chassis->turnToPoint(-49.6, -37.9, 600, {},false);
chassis->moveToPoint(-49.6, -37.9+5, 1000, {},false);
liftTo(300);
openClaw();
liftTo(0);
}
void Autons::auton2(){



}
void Autons::auton3(){
    
    
}







/*


//-------------RIGHT HALF--------------------------
        //------long goal------//
            
            chassis->setPose(-47,-2,0);
            front_motor.move(127);
            chassis->moveToPoint(-47,10,900,{.maxSpeed = 100},false);
            chassis->moveToPoint(-47,-47,1400,{.forwards = false, .maxSpeed = 95},false);//infront loader
            ML.set_value(true);
            chassis->turnToHeading(270,600,{},false);//turn toward loader
            chassis->moveToPoint(-65,-47,1600,{.maxSpeed = 95},true);//go to loader

            load();
            front_motor.move(127);

            chassis->moveToPoint(-32,-47,600,{.forwards = false,.maxSpeed = 95},true);//go back
            
            pros::delay(1000);
            long_score(1000);//score, issue with time
            ML.set_value(false);
            chassis->setPose(-30,-47,270);


        //------middle blocks------//
        
            chassis->moveToPoint(-40,-47,300,{.maxSpeed = 90},false);//move back a little bit
            chassis->turnToHeading(15,500,{},false);// turn toward blocks
            chassis->moveToPoint(-23,-25,1000 ,{.maxSpeed = 85} , true);//go to blocks
            
            pros::delay(500);
            collect(true);//intake,fix this
            chassis->moveToPoint(-23,22,1300 ,{.maxSpeed = 95} , true);
            collect(false);
            front_motor.move(127);
            pros::delay(900);
            collect(true);
            //chassis->setPose(-23,11,chassis->getPose().theta);


            chassis->turnToHeading(315,700,{},false);
            chassis->moveToPoint(-10,10,900,{.forwards = false,.maxSpeed = 85},false);//go to middle goal
            chassis->setPose(-7,7,315);//reset pose
            middle_score(1000);//score
            chassis->moveToPoint(-45,47,700,{.maxSpeed = 85},false);

    //--------LEFT HALF---------------
    //Code is def co pilot














 //long goal
    chassis->setPose(-47,18.7+2,0);
    chassis->moveToPoint(-47,52,1000,{.maxSpeed = 85},false);//infront loader
    ML.set_value(true);
    chassis->turnToHeading(270,600,{},false);//turn toward loader
    chassis->moveToPoint(-60,52,900,{.maxSpeed = 80},true);//go to loader

    load();
    front_motor.move(127);
    
    chassis->moveToPoint(-20,52,500,{.forwards = false,.maxSpeed = 85},true);//go back
    
    pros::delay(1000);
    long_score(1000);//score, issue with time
    ML.set_value(false);
    
    //middle goal
    chassis->moveToPoint(-38,52,300,{.maxSpeed = 90},false);//move back a little bit
    chassis->turnToHeading(150,500,{},false);// turn toward blocks
    chassis->moveToPoint(-22+8,22.6,1000 ,{.maxSpeed = 85} , true);//go to blocks
    pros::delay(500);
    collect(true);//intake,fix this
    chassis->turnToHeading(315,700,{},false);//rotate 180 and face middle goal
    collect(false);//stop intake
    chassis->moveToPoint(-3,5,400,{.forwards = false,.maxSpeed = 85},false);//go to middle goal
    chassis->setPose(-7,7,315);//reset pose
    middle_score(1000);//score

    ///wing
    chassis->moveToPoint(-20,20,600,{.maxSpeed = 85},false);//reverse a bit
    chassis->turnToHeading(330,300,{},false);//turn toward front of long goal
    chassis->moveToPoint(-40,35-1,600,{.maxSpeed = 85},false);//go to side of goal
    chassis->turnToHeading(270,500,{},false);//rotate facing alliance side
    chassis->moveToPoint(-10,35-1,2000,{.forwards = false,.maxSpeed = 60},false);//go backwards with wing



 //get blocks
    chassis->setPose(-45.25,-7.5+1,90);
    chassis->turnToHeading(120,800,{},false);//turn toward block
    chassis->moveToPoint(-22,-22,900,{.maxSpeed = 95},true);//go to blocks
    pros::delay(500);
    collect(true);//collect
    chassis->turnToHeading(210,300,{},false);//turn towards goal
    collect(false);//stop

    //score
    chassis->moveToPoint(-37,-47,600,{.maxSpeed = 90},false);//go infornt goal
    chassis->turnToHeading(270,300,{},false);//turn facing alliance
    chassis->moveToPoint(-15,-47,700,{.forwards = false,.maxSpeed = 90},false);//go into goal
    pros::delay(1000);
    long_score(1000);//score, issue with time
    chassis->setPose(-20,-47,270);

    //wing
    chassis->moveToPoint(-47+10,-47-3,600,{.maxSpeed = 90},false);//reverse
    chassis->turnToHeading(300,600,{},false);//turn toward side
    chassis->moveToPoint(-25,-56-3,600,{.forwards = false,.maxSpeed = 90},false);//go backwards toward point
    chassis->turnToHeading(270,600,{},false);
    chassis->moveToPoint(-5,-56-3,2000,{.forwards = false,.maxSpeed = 90},false);//go backwards while wing down


*/