#include "autons.h"

//-----------------------------------------------------------PID TEST AUTON-----------------------------------------
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
//-----------------------------------------------------------PID TEST AUTON-----------------------------------------

