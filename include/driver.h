#pragma once
#include "pros/misc.hpp"
#include "pros/motors.hpp"
#include "lemlib/api.hpp"

class DriverControl {
public:
    DriverControl(pros::Controller* controller, lemlib::Chassis* chassis);

    void startPeriod();
    void update();   // called repeatedly during opcontrol

private:
    pros::Controller* controller;
    lemlib::Chassis* chassis;
    bool cascadeLimiterEnabled = false;
};
