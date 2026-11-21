#include "autons.h"

//-----------------------------------------------------------A3: Right Side + extra scoring---------------------------------
// NOTE: the start is ALMOST the same as rightSideOpening() (shared.cpp) but has its own tweaks
// (lift comes down while pushing, earlyExitRange on the back-up). If those tweaks aren't
// needed, replace lines up to "end of opening" with rightSideOpening().
void Autons::auton3(){

resetLift();
closeClaw(); // close claw to hold preload

chassis->setPose(-61.648, -0.646, 90);
liftTo(13.33, 2000, false);
toggle(2);

chassis->moveToPoint(-47, -8.6, 1000,{.maxSpeed = 127, .minSpeed = 80, .earlyExitRange = 5},true);
chassis->moveToPoint(-47.5, -18.193, 1000,{.minSpeed = 100},false);

// push into the goal while the lift comes down (before, the push lasted as long as the lift took)
liftTo(5.33, 2000, false);
pushFor(100, 100, 500);
waitUntilLiftDone();
openClaw();
chassis->setPose(-47, -18.6, chassis->getPose().theta);

chassis->moveToPoint(-47, 0, 900, {.forwards = false},false);
chassis->moveToPoint(-28.478, -20.31, 1000,{.minSpeed = 50, .earlyExitRange = 20},true);
chassis->moveToPoint(-27, -21, 800,{.maxSpeed = 50},false);

closeClaw();

chassis->moveToPoint(-25, -25, 1000,{.maxSpeed = 127, .minSpeed = 80},true);
liftTo(20.0);
chassis->moveToPoint(-41.2, -23.8, 2000,{.minSpeed = 80},false);

// swing into the goal while lowering; TUNE the 300ms (before, the swing never stopped)
liftTo(5.33, 2000, false);
pushFor(10, 120, 300);
waitUntilLiftDone();
openClaw();

chassis->setPose(-41, -23, chassis->getPose().theta-1);

chassis->moveToPoint(-28, -25, 1500,{.forwards = false},false);  // timeout was 5000
chassis->moveToPoint(-43, -43, 1000,{.minSpeed = 50, .earlyExitRange = 20},true);
chassis->moveToPoint(-45, -41, 2000,{.maxSpeed = 50},false);

closeClaw();

liftTo(26.67, 2000, false);
chassis->turnToPoint(-47.183, -23, 1000,{},false);
waitUntilLiftDone();
chassis->moveToPoint(-47.183, -23, 1000,{.maxSpeed = 60, .minSpeed = 50},false);
liftTo(13.33);
openClaw();
// ---- end of opening ----

liftTo(0, 2000, false);  // lower while backing up

chassis->moveToPoint(-47, -60, 500,{.forwards = false},false);
chassis->moveToPoint(-34, -23, 2000,{.maxSpeed = 127, .minSpeed = 120, .earlyExitRange = 5},false);

chassis->moveToPoint(-60, 13, 1500,{.maxSpeed = 127, .minSpeed = 120},false);
waitUntilLiftDone();

chassis->tank(100,100,true);
pros::delay(600);
closeClaw();
chassis->tank(0,0);
liftTo(28.0, 2000, false);  // was 17.33 then 28 later: go straight to 28 while turning/backing

chassis->turnToHeading(0, 500);
chassis->moveToPoint(-60, 0, 1000,{.forwards = false},false);
waitUntilLiftDone();
chassis->moveToPoint(-47, 23, 2000,{.minSpeed = 80},false);
liftTo(6.67);
openClaw();

}
