#include "autons.h"
#include "config.h"   // gives access to motors, pistons, sensors

Autons::Autons(lemlib::Chassis* chassis)
    : chassis(chassis) {}


void Autons::lift(int time){
    

}

void Autons::score(bool on){
    
    
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
    chassis->moveToPoint(0, -24, 2000,{.maxSpeed = 100});

    pros::delay(500);

    // Backward 24"
    chassis->moveToPoint(0, 0, 2000,{.maxSpeed = 100});

    pros::delay(500);

    // Forward 48"
    chassis->moveToPoint(0, -48, 3000, {.maxSpeed = 85});

    pros::delay(500);

    // Back to start
    chassis->moveToPoint(0, 0, 3000,{.maxSpeed = 85});

    pros::delay(800);
    
}



void Autons::skillsAuton(){


}
void Autons::auton1(){
chassis->setPose(-68.039, -0.62, 90);// start facing center
//rotate toggle
chassis->moveToPoint(-46.929, -20.826, 1000,{},false);// move to goal
chassis->turnToHeading(0,500,{},false);// make back side face goal
chassis->moveToPoint(-46.929, -25, 100,{.forwards = false},true);//back up into the goal
//score preload
pros::delay(2000);
chassis->moveToPoint(-46.929, -20, 100,{.forwards = false},false); //move a bit forward
chassis->turnToPoint(-23,-23,100,{.forwards = false},false);// turn toward cup
chassis->moveToPoint(-27.674, -23.575, 5000); // drive to cup
//pick up cup with claw
pros::delay(2000);

chassis->moveToPoint(-45, -23.484, 5000,{.forwards = false},false); // go back to goal
//score cup

//get the other cup and score
//chassis->moveToPoint(-47.248, -43.348, 5000);
//chassis->moveToPoint(-47.018, -26.42, 5000);



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

























