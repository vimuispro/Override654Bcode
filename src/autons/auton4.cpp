#include "autons.h"
#include "config.h"

void Autons::auton4(){
    liftTo(CASCADE_MAX_HEIGHT_INCHES, 5000);  // all the way to the top, and wait (with false, the UP-button test stopped the lift right away)
}
