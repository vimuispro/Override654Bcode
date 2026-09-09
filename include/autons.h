#pragma once
#include "lemlib/api.hpp"
#include "pros/misc.hpp"

class Autons {
public:
    Autons(lemlib::Chassis* chassis);


    

    void lift(int time);
    void score(bool on);

    void testAuton();      
    void skillsAuton(); 

    void auton1();
    void auton2();
    void auton3();

    

private:
    lemlib::Chassis* chassis;
};
