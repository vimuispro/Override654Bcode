#pragma once

enum AutonMode {
    AUTON_NONE,
    AUTON_SKILLS,
    AUTON_auton1,
    AUTON_auton2,
    AUTON_auton3
};

extern AutonMode selectedAuton;

void uiInit();
void uiUpdate();
