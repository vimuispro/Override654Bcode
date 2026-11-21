#include "autons.h"

//-----------------------------------------------------------SKILLS AUTON--------------------------------------------
void Autons::skillsAuton() {

rightSideOpening();  // same start as auton2 (shared.cpp)

chassis->setPose(-47, -28, chassis->getPose().theta - 1); //set new pose

struct PinCycle {
    double liftHeight;
    int liftTimeout;
    bool raiseAboveGoal;   // wait for the lift to reach liftHeight before lowering to score
    double scoreHeight;
};
const PinCycle pinCycles[] = {
    {33.33, 2000, false, 24.0},
    {36.0, 5000, true, 30.67},   // 36 = top (was 40, which the lift can never reach)
};
for (const PinCycle& pin : pinCycles) {
    chassis->moveToPoint(-47, -59, 1000, {.forwards = false}, false);
    liftTo(0, 2000, false);
    chassis->turnToHeading(270, 1000, {}, false);
    chassis->moveToPoint(-63.5, -60, 1000, {}, false);
    closeClaw();
    chassis->moveToPoint(-47, -59, 1000, {.forwards = false}, true);
    liftTo(pin.liftHeight, pin.liftTimeout, false);
    chassis->turnToHeading(0, 1000, {}, false);
    chassis->moveToPoint(-47, -26, 2000, {.maxSpeed = 60}, false);
    if (pin.raiseAboveGoal) {
        // Before this was liftTo(40, 5000, false), which the liftTo() right below replaced
        // straight away, so the lift never went above the goal first.
        waitUntilLiftDone();
    }
    chassis->setPose(-47, -28, chassis->getPose().theta);
    liftTo(pin.scoreHeight, 2000, true, 100);
    openClaw();
}

//pin only
chassis->moveToPoint(-47, -59, 1000, {.forwards = false}, false);
liftTo(0, 2000, false);
chassis->turnToHeading(270, 1000, {}, false);
chassis->moveToPoint(-63.5, -60, 1000, {}, false);
closeClaw();
chassis->moveToPoint(-50, -59, 1000, {.forwards = false}, false);
liftTo(10.67, 5000, false);
chassis->moveToPoint(-34, -47, 1000, {.forwards = false}, false);
chassis->turnToHeading(90, 1000, {}, true);
chassis->moveToPoint(-26, -47, 1000, {}, false);
pushFor(127, 127, 500);
liftTo(0);
openClaw();


struct CupCycle {
    double pickupY;
    double liftHeight;
    bool resetPoseAfter;
};
const CupCycle cupCycles[] = {
    {-50, 16.0, true},
    {-60, 26.67, false},
};
for (const CupCycle& cup : cupCycles) {
    chassis->moveToPoint(-34, -46, 1000, {}, false);
    chassis->moveToPoint(-65, cup.pickupY, 2000, {}, false);
    closeClaw();
    liftTo(cup.liftHeight, 5000, false);
    chassis->moveToPoint(-34, -47, 1000, {.forwards = false}, false);
    chassis->turnToHeading(90, 1000, {}, true);
    chassis->moveToPoint(-26, -47, 1000, {}, false);
    pushFor(127, 127, 500);
    liftTo(0);
    openClaw();
    if (cup.resetPoseAfter) {
        chassis->setPose(-47, -29, chassis->getPose().theta);
    }
}
}
//-----------------------------------------------------------SKILLS AUTON--------------------------------------------
