#include "autons.h"

void Autons::auton4(){
    resetLift();
    liftTo(40.0, 5000, false);
}
