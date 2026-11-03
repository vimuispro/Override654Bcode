#pragma once

enum AutonMode {
    AUTON_NONE,
    AUTON_SKILLS,
    AUTON_test,
    AUTON_auton1,
    AUTON_auton2,
    AUTON_auton3
};

extern AutonMode selectedAuton;

void uiInit();
void uiUpdate();

// Controller screen + controller auton selector (runs in its own task)
void controllerUiStart();

// Short name of an auton, e.g. "a1"
const char* autonName(AutonMode mode);

// What the color sensor sees right now: "Blue", "Red", "Yellow", "Other", "None" or "Error"
const char* seenColorName();