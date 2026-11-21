#include "autons.h"

//-----------------------------------------------------------A1: LEFT Side Auton--------------------------------------------
void Autons::auton1(){
resetLift();
closeClaw(); // close claw to hold preload

chassis->setPose(1, -61.7, 0);
liftTo(9.33, 2000, false);
toggle(2);

chassis->moveToPoint(-9, -49, 1000,{.maxSpeed = 127, .minSpeed = 80, .earlyExitRange = 5},true);
chassis->moveToPoint(-18.193, -49, 1000,{.minSpeed = 100},false);

liftTo(4.0);
openClaw();
chassis->setPose(-18.6, -47, chassis->getPose().theta);
chassis->moveToPoint(-9, -47, 1000,{.forwards = false},false);

liftTo(0);
chassis->turnToHeading(-135, 500,{}, true);
chassis->moveToPoint(-20, -61, 1000,{.earlyExitRange = 5},false);

chassis->tank(127,127,true);
pros::delay(600);
closeClaw();
liftTo(10.67, 900);
chassis->tank(0,0,true);

chassis->turnToHeading(-90, 500);
chassis->moveToPoint(0, -61, 1000,{.forwards = false},true);
liftTo(17.33);
chassis->moveToPoint(-23, -47, 1000,{.maxSpeed = 60},false);

liftTo(4.0);
openClaw();
pros::delay(500);
//second pin
chassis->moveToPoint(0, -52, 1000,{.forwards = false},true);
chassis->moveToPoint(15, -52, 2000,{},false);

chassis->turnToHeading(140, 500, {}, false);

chassis->tank(100,100,true);
pros::delay(1000);
closeClaw();
liftTo(17.33, 2000);
chassis->tank(0,0);

chassis->turnToHeading(90, 500);
chassis->moveToPoint(0, -61, 1000,{.forwards = false},true);

liftTo(20.0);
chassis->moveToPoint(23, -47, 1000,{.maxSpeed = 60},false);
liftTo(4.0);
openClaw();

}
