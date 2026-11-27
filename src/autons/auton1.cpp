#include "autons.h"

//-----------------------------------------------------------A1: LEFT Side Auton--------------------------------------------
void Autons::auton1(){
closeClaw(0); // close claw on the preload; it keeps closing while the robot starts moving

// same start as auton2, mirrored: X and Y swapped, heading = 90 - auton2's heading
chassis->setPose(-0.646, -61.648, 0);
liftTo(4.3, 2000, false);
toggle(2);

chassis->moveToPoint(-8.6, -47, 1000, {.maxSpeed = 100, .minSpeed = 80, .earlyExitRange = 5}, true);
chassis->moveToPoint(-18.193, -48, 1000, {.maxSpeed = 100, .minSpeed = 50}, false);

pushFor(80, 80, 500);  // square up on the goal

liftTo(0);
openClaw();

distanceReset();  // fix X and/or Y from the distance sensors, if possible
chassis->moveToPoint(0, -47, 1000, {.forwards = false, .maxSpeed = 100}, false);
chassis->turnToPoint(-16, -60, 1000, {}, false);
chassis->moveToPoint(-16, -60, 1000, { .maxSpeed = 100}, false);

pushFor(100, 100, 600);
closeClaw(0);

chassis->turnToHeading(-90, 1000, {}, false);
distanceReset();
liftTo(7);
chassis->moveToPoint(0, -58, 1000, {.forwards = false, .maxSpeed = 100}, false);
waitUntilLiftDone();
chassis->moveToPoint(-21, -49, 1000, {.maxSpeed = 70}, false);
liftTo(4);
waitUntilLiftDone();
openClaw();

distanceReset();
chassis->moveToPoint(-13, -55, 1000, {.forwards = false, .maxSpeed = 100}, false);
chassis->moveToPoint(13, -47, 1000, {.forwards = false, .maxSpeed = 100}, false);
chassis->turnToPoint(24.5, -56, 1000, {}, false);
chassis->moveToPoint(24.5, -56, 1000, { .maxSpeed = 100}, false);

pushFor(100, 100, 600);
closeClaw(0);

chassis->turnToHeading(90, 1000, {}, false);
distanceReset();
liftTo(9);

chassis->moveToPoint(0, -58, 1000, {.forwards = false, .maxSpeed = 100}, false);
waitUntilLiftDone();
chassis->moveToPoint(21, -49, 1000, {.maxSpeed = 70}, false);
liftTo(4);
waitUntilLiftDone();
openClaw();
/*
liftTo(0, 2000, false);  // lower while turning instead of standing still
chassis->turnToHeading(-135, 500,{}, true);
// keep speed into the push below (earlyExitRange does nothing without minSpeed)
chassis->moveToPoint(-20, -61, 1000,{.minSpeed = 60, .earlyExitRange = 5},false);
waitUntilLiftDone();     // lift must be down before grabbing

chassis->tank(127,127,true);
pros::delay(600);
closeClaw();             // keep pushing while the claw grips

chassis->tank(0,0,true); // was left running during the lift
liftTo(10.67, 900, false);

chassis->turnToHeading(-90, 500);
chassis->moveToPoint(0, -61, 1000,{.forwards = false},true);
liftTo(17.33);
chassis->moveToPoint(-23, -47, 1000,{.maxSpeed = 60},false);

liftTo(4.0);
openClaw();              // openClaw now waits for the release (replaces the old delay(500))
//second pin
chassis->moveToPoint(0, -52, 1000,{.forwards = false},true);
chassis->moveToPoint(15, -52, 2000,{},false);

chassis->turnToHeading(140, 1000, {}, false); // was 500ms: could time out and push at the wrong angle

chassis->tank(100,100,true);
pros::delay(1000);
closeClaw();
chassis->tank(0,0);
liftTo(17.33, 2000, false);

chassis->turnToHeading(90, 500);
chassis->moveToPoint(0, -61, 1000,{.forwards = false},true);

liftTo(20.0);
chassis->moveToPoint(23, -47, 1000,{.maxSpeed = 60},false);
liftTo(4.0);
openClaw();
*/
}
