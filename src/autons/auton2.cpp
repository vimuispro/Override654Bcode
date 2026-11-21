#include "autons.h"

//-----------------------------------------------------------A2: Right Side Auton--------------------------------------------
void Autons::auton2(){

resetLift();
closeClaw(); // close claw to hold preload

chassis->setPose(-61.648, -0.646, 90);
liftTo(13.33, 2000, false);
toggle(2);


chassis->moveToPoint(-47, -8.6, 1000,{.maxSpeed = 127, .minSpeed = 80, .earlyExitRange = 5},true);
chassis->moveToPoint(-47.5, -18.193, 1000,{.minSpeed = 100},false);
//chassis->moveToPose(-47.071, -18.193, 180, 5000,{.minSpeed = 100},false);

chassis->tank(100,100,true);
pros::delay(500);
chassis->tank(0,0,true);

liftTo(5.33);
openClaw();
chassis->setPose(-47, -18.6, chassis->getPose().theta);

chassis->moveToPoint(-47, 0, 900, {.forwards = false},false);
chassis->moveToPoint(-28.478, -20.31, 500,{.earlyExitRange = 20},true);
chassis->moveToPoint(-27, -21, 800,{.maxSpeed = 50},false);

closeClaw(); 


chassis->moveToPoint(-25, -25, 1000,{.maxSpeed = 127, .minSpeed = 80, .earlyExitRange = 1},true);
liftTo(20.0);
chassis->moveToPoint(-41.2, -23.8, 2000,{.minSpeed = 80},false);
chassis->tank(10,120,true);

liftTo(5.33);
openClaw();

chassis->setPose(-41, -23, chassis->getPose().theta-1);

chassis->moveToPoint(-28, -25, 5000,{.forwards = false},false);
chassis->moveToPoint(-43, -43, 500,{.earlyExitRange = 20,},true);
chassis->moveToPoint(-45, -41, 2000,{.maxSpeed = 50},false);

closeClaw(); 

liftTo(26.67);
chassis->turnToPoint(-47.183, -23, 1000,{},true);
chassis->moveToPoint(-47.183, -23, 1000,{.maxSpeed = 60, .minSpeed = 50},false);
liftTo(13.33);
openClaw();

}
//-----------------------------------------------------------A2: Right Side Auton--------------------------------------------

