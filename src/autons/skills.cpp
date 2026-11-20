#include "autons.h"

//-----------------------------------------------------------SKILLS AUTON--------------------------------------------
void Autons::skillsAuton(){

resetLift();
closeClaw(); // close claw to hold preload

chassis->setPose(-61.648, -0.646, 90);
liftTo(1000, 2000, false);
toggle(2);


chassis->moveToPoint(-47, -8.6, 1000,{.maxSpeed = 127, .minSpeed = 80, .earlyExitRange = 5},true);
chassis->moveToPoint(-47.5, -18.193, 1000,{.minSpeed = 100},false);
//chassis->moveToPose(-47.071, -18.193, 180, 5000,{.minSpeed = 100},false);

chassis->tank(100,100,true);
pros::delay(500);
chassis->tank(0,0,true);

liftTo(400);
openClaw();
chassis->setPose(-47, -18.6, chassis->getPose().theta);

chassis->moveToPoint(-47, 0, 900, {.forwards = false},false);
chassis->moveToPoint(-28.478, -20.31, 500,{.earlyExitRange = 20},true);
chassis->moveToPoint(-27, -21, 800,{.maxSpeed = 50},false);

closeClaw(); 


chassis->moveToPoint(-25, -25, 1000,{.maxSpeed = 127, .minSpeed = 80, .earlyExitRange = 1},true);
liftTo(1500);
chassis->moveToPoint(-41.2, -23.8, 2000,{.minSpeed = 80},false);
chassis->tank(10,120,true);

liftTo(400);
openClaw();

chassis->setPose(-41, -23, chassis->getPose().theta-1);

chassis->moveToPoint(-28, -25, 5000,{.forwards = false},false);
chassis->moveToPoint(-43, -43, 500,{.earlyExitRange = 20,},true);
chassis->moveToPoint(-45, -41, 2000,{.maxSpeed = 50},false);

closeClaw(); 

liftTo(2000);
chassis->turnToPoint(-47.183, -23, 1000,{},true);
chassis->moveToPoint(-47.183, -23, 1000,{.maxSpeed = 60, .minSpeed = 50},false);
liftTo(1000);
openClaw();
//auton 2 end

chassis->setPose(-47, -28, chassis->getPose().theta-1);
liftTo(0);

chassis->moveToPoint(-47, -59, 1000,{.forwards = false},false);
chassis->turnToHeading(270, 1000,{},false);
chassis->moveToPoint(-63.5, -59, 1000,{},false);
closeClaw(); 
chassis->moveToPoint(-47, -59, 1000,{.forwards = false},true);
liftTo(2500, 2000, false);
chassis->turnToHeading(0, 1000,{},false);
chassis->moveToPoint(-47, -26, 2000,{.maxSpeed = 60},false);
chassis->setPose(-47, -30, chassis->getPose().theta-1);
liftTo(1800, 2000, true, 100);
openClaw();

liftTo(0);

chassis->moveToPoint(-47, -59, 1000,{.forwards = false},false);
chassis->turnToHeading(270, 1000,{},false);
chassis->moveToPoint(-63.5, -59, 1000,{},false);
closeClaw(); 
chassis->moveToPoint(-47, -59, 1000,{.forwards = false},true);
liftTo(4000, 5000, false);
chassis->turnToHeading(0, 1000,{},false);
chassis->moveToPoint(-47, -26, 2000,{.maxSpeed = 60},false);
chassis->setPose(-47, -30, chassis->getPose().theta-1);
liftTo(2300,  2000, true, 100);
openClaw();

liftTo(0);

chassis->moveToPoint(-47, -59, 1000,{.forwards = false},false);
chassis->turnToHeading(270, 1000,{},false);
chassis->moveToPoint(-63.5, -59, 1000,{},false);
closeClaw(); 
chassis->moveToPoint(-50, -59, 1000,{.forwards = false},false);
liftTo(700, 5000, false);
chassis->moveToPoint(-34, -47, 1000,{.forwards = false},false);
chassis->turnToHeading(90, 1000,{},false);
chassis->moveToPoint(-26, -47, 1000,{},false);
liftTo(0, 5000, false);
openClaw();
}
//-----------------------------------------------------------SKILLS AUTON--------------------------------------------
