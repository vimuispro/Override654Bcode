#include "pros/apix.h"
#include "liblvgl/lvgl.h"
#include "ui.h"
#include "config.h"
#include <cstring>
#include <cstdio>

// Screens
static lv_obj_t* homeScreen;
static lv_obj_t* fieldScreen;

// Auton selection enum
AutonMode selectedAuton = AUTON_NONE;

// Auton buttons
static lv_obj_t* skillsBtn;
static lv_obj_t* auton1Btn;
static lv_obj_t* auton2Btn;
static lv_obj_t* auton3Btn;
static lv_obj_t* testBtn;
static lv_obj_t* coordLabel;
static lv_obj_t* cascadeLabel;

// Styles
static lv_style_t selectedStyle;
static lv_style_t normalStyle;

void initAutonStyles() {
    lv_style_init(&selectedStyle);
    lv_style_set_bg_color(&selectedStyle, lv_color_hex(0x00AAFF));
    lv_style_set_bg_opa(&selectedStyle, LV_OPA_80);

    lv_style_init(&normalStyle);
    lv_style_set_bg_color(&normalStyle, lv_color_hex(0x333333));
    lv_style_set_bg_opa(&normalStyle, LV_OPA_80);
}

// ------------------------
// Build Home Screen
// ------------------------
void buildHomeScreen() {
    lv_obj_set_style_bg_color(homeScreen, lv_color_hex(0x111111), LV_PART_MAIN);

    lv_obj_t* fieldBtn = lv_button_create(homeScreen);
    lv_obj_set_size(fieldBtn, 150, 35);
    lv_obj_set_pos(fieldBtn, 165, 205);

    lv_obj_t* label = lv_label_create(fieldBtn);
    lv_label_set_text(label, "Auton Select");
    lv_obj_center(label);

    lv_obj_add_event_cb(fieldBtn, [](lv_event_t* e){
        lv_screen_load(fieldScreen);
    }, LV_EVENT_CLICKED, NULL);
    coordLabel = lv_label_create(homeScreen);
    lv_label_set_text(coordLabel, "X: 0  Y: 0  H: 0");
    lv_obj_set_pos(coordLabel, 10, 10);   // move wherever you want
    cascadeLabel = lv_label_create(homeScreen);
    lv_label_set_text(cascadeLabel, "Cascade: 0");
    lv_obj_set_pos(cascadeLabel, 10, 30);

}

// ------------------------
// Build Auton Selector Screen
// ------------------------
void buildFieldScreen() {
    lv_obj_set_style_bg_color(fieldScreen, lv_color_hex(0x000000), LV_PART_MAIN);

    int btnW = 180;
    int btnH = 45;

    // Skills
    skillsBtn = lv_button_create(fieldScreen);
    lv_obj_set_size(skillsBtn, btnW, btnH);
    lv_obj_set_pos(skillsBtn, 20, 20);
    lv_obj_add_style(skillsBtn, &normalStyle, LV_PART_MAIN);
    lv_obj_t* skLabel = lv_label_create(skillsBtn);
    lv_label_set_text(skLabel, "Skills");
    lv_obj_center(skLabel);
    lv_obj_add_event_cb(skillsBtn, [](lv_event_t* e){
        selectedAuton = AUTON_SKILLS;
    }, LV_EVENT_CLICKED, NULL);

    // Auton 1 // change name to whatever
    auton1Btn = lv_button_create(fieldScreen);
    lv_obj_set_size(auton1Btn, btnW, btnH);
    lv_obj_set_pos(auton1Btn, 250, 20);
    lv_obj_add_style(auton1Btn, &normalStyle, LV_PART_MAIN);
    lv_obj_t* p7Label = lv_label_create(auton1Btn);
    lv_label_set_text(p7Label, "a1");
    lv_obj_center(p7Label);
    lv_obj_add_event_cb(auton1Btn, [](lv_event_t* e){
        selectedAuton = AUTON_auton1;
    }, LV_EVENT_CLICKED, NULL);

    // Auton 2
    auton2Btn = lv_button_create(fieldScreen);
    lv_obj_set_size(auton2Btn, btnW, btnH);
    lv_obj_set_pos(auton2Btn, 250, 80);
    lv_obj_add_style(auton2Btn, &normalStyle, LV_PART_MAIN);
    lv_obj_t* m3Label = lv_label_create(auton2Btn);
    lv_label_set_text(m3Label, "a2");
    lv_obj_center(m3Label);
    lv_obj_add_event_cb(auton2Btn, [](lv_event_t* e){
        selectedAuton = AUTON_auton2;
    }, LV_EVENT_CLICKED, NULL);

    // Auton 3
    auton3Btn = lv_button_create(fieldScreen);
    lv_obj_set_size(auton3Btn, btnW, btnH);
    lv_obj_set_pos(auton3Btn, 20, 80);
    lv_obj_add_style(auton3Btn, &normalStyle, LV_PART_MAIN);
    lv_obj_t* awpLabel = lv_label_create(auton3Btn);
    lv_label_set_text(awpLabel, "a3");
    lv_obj_center(awpLabel);
    lv_obj_add_event_cb(auton3Btn, [](lv_event_t* e){
        selectedAuton = AUTON_auton3;
    }, LV_EVENT_CLICKED, NULL);

    // Test auton
    testBtn = lv_button_create(fieldScreen);
    lv_obj_set_size(testBtn, btnW, btnH);
    lv_obj_set_pos(testBtn, 250, 140);
    lv_obj_add_style(testBtn, &normalStyle, LV_PART_MAIN);
    lv_obj_t* testLabel = lv_label_create(testBtn);
    lv_label_set_text(testLabel, "Test");
    lv_obj_center(testLabel);
    lv_obj_add_event_cb(testBtn, [](lv_event_t* e){
        selectedAuton = AUTON_test;
    }, LV_EVENT_CLICKED, NULL);

    // Back button
    lv_obj_t* backBtn = lv_button_create(fieldScreen);
    lv_obj_set_size(backBtn, 120, 40);
    lv_obj_set_pos(backBtn, 150, 190);

    lv_obj_t* backLabel = lv_label_create(backBtn);
    lv_label_set_text(backLabel, "Back");
    lv_obj_center(backLabel);

    lv_obj_add_event_cb(backBtn, [](lv_event_t* e){
        lv_screen_load(homeScreen);
    }, LV_EVENT_CLICKED, NULL);
}

// ------------------------
// Highlight Logic
// ------------------------
void updateAutonHighlight() {
    lv_obj_add_style(skillsBtn, &normalStyle, LV_PART_MAIN);
    lv_obj_add_style(auton1Btn, &normalStyle, LV_PART_MAIN);
    lv_obj_add_style(auton2Btn, &normalStyle, LV_PART_MAIN);
    lv_obj_add_style(auton3Btn, &normalStyle, LV_PART_MAIN);
    lv_obj_add_style(testBtn, &normalStyle, LV_PART_MAIN);

    if (selectedAuton == AUTON_SKILLS)
        lv_obj_add_style(skillsBtn, &selectedStyle, LV_PART_MAIN);

    if (selectedAuton == AUTON_auton1)
        lv_obj_add_style(auton1Btn, &selectedStyle, LV_PART_MAIN);

    if (selectedAuton == AUTON_auton2)
        lv_obj_add_style(auton2Btn, &selectedStyle, LV_PART_MAIN);

    if (selectedAuton == AUTON_auton3)
        lv_obj_add_style(auton3Btn, &selectedStyle, LV_PART_MAIN);

    if (selectedAuton == AUTON_test)
        lv_obj_add_style(testBtn, &selectedStyle, LV_PART_MAIN);
}

// ------------------------
// Init UI
// ------------------------
void uiInit() {
    homeScreen = lv_obj_create(NULL);
    fieldScreen = lv_obj_create(NULL);

    initAutonStyles();
    buildHomeScreen();
    buildFieldScreen();

    lv_screen_load(homeScreen);

    pros::Task uiTask([] {
        while (true) {
            uiUpdate();
            pros::delay(50);
        }
    });
}

// ------------------------
// Update Loop
// ------------------------
void uiUpdate() {
    if (lv_screen_active() == fieldScreen) {
        updateAutonHighlight();
    }
    // Get pose from your odometry system
    double x = chassis.getPose().x;
    double y = chassis.getPose().y;
    double h = chassis.getPose().theta;

    // Format text
    char buf[64];
    snprintf(buf, sizeof(buf), "X: %.1f  Y: %.1f  H: %.1f", x, y, h);

    // Update label
    lv_label_set_text(coordLabel, buf);
    snprintf(buf, sizeof(buf), "Cascade: %.1f", cascadeVal);
    lv_label_set_text(cascadeLabel, buf);
    pros::delay(50);

}

// ========================================================================
// Controller screen
// ------------------------------------------------------------------------
// The V5 controller only accepts one screen update about every 50 ms, so
// ONE task does all the printing and updates one line at a time.
//
// While the robot is DISABLED (on the field / competition switch, before
// the match):
//   Line 1: Auto:< a1 >   <- LEFT / RIGHT arrows change the auton
//   Line 2: Team: Blue    <- A switches Blue / Red
//   Line 3: Sees: Red     <- what the color sensor sees
// During driver control:
//   Line 1: X and Y position
//   Line 2: Team  (RIGHT arrow still switches it, in driver.cpp)
//   Line 3: What the color sensor sees
// ========================================================================

// Order the LEFT / RIGHT arrows step through
static const AutonMode autonOrder[] = {
    AUTON_NONE, AUTON_SKILLS, AUTON_test, AUTON_auton1, AUTON_auton2, AUTON_auton3
};
static const int autonCount = sizeof(autonOrder) / sizeof(autonOrder[0]);

const char* autonName(AutonMode mode) {
    switch (mode) {
        case AUTON_SKILLS: return "Skills";
        case AUTON_test:   return "Test";
        case AUTON_auton1: return "a1";
        case AUTON_auton2: return "a2";
        case AUTON_auton3: return "a3";
        default:           return "None";
    }
}

static void stepAuton(int direction) {
    int index = 0;
    for (int i = 0; i < autonCount; i++) {
        if (autonOrder[i] == selectedAuton) index = i;
    }
    index = (index + direction + autonCount) % autonCount;
    selectedAuton = autonOrder[index];
}

const char* seenColorName() {
    const std::int32_t proximity = colorSensor.get_proximity();
    const double hue = colorSensor.get_hue();
    const double saturation = colorSensor.get_saturation();

    if (proximity < 0 || hue < 0 || saturation < 0) return "Error";
    if (proximity < 80 || saturation < 0.2) return "None";
    if (hue >= 190 && hue <= 260) return "Blue";
    if (hue <= 30 || hue >= 330) return "Red";
    if (hue >= 40 && hue <= 80) return "Yellow";
    return "Other";
}

void controllerUiStart() {
    pros::Task controllerTask([] {
        char wanted[3][20];
        char shown[3][20] = {"", "", ""};
        int nextLine = 0;

        controller.clear();
        pros::delay(60);

        while (true) {
            const bool selecting = pros::competition::is_disabled();

            // Selector buttons only work while disabled, so they never
            // clash with the driver-control buttons.
            if (selecting) {
                if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT))  stepAuton(-1);
                if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) stepAuton(+1);
                if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A))     isBlueTeam = !isBlueTeam;
            }

            // Line 1
            if (selecting) {
                snprintf(wanted[0], sizeof(wanted[0]), "Auto:< %s >", autonName(selectedAuton));
            } else if (pros::competition::is_autonomous()) {
                snprintf(wanted[0], sizeof(wanted[0]), "Auto: %s", autonName(selectedAuton));
            } else {
                lemlib::Pose pose = chassis.getPose();
                snprintf(wanted[0], sizeof(wanted[0]), "X:%.1f Y:%.1f", pose.x, pose.y);
            }
            // Line 2
            snprintf(wanted[1], sizeof(wanted[1]), "Team: %s", isBlueTeam ? "Blue" : "Red");
            // Line 3
            snprintf(wanted[2], sizeof(wanted[2]), "Sees: %s", seenColorName());

            // Print ONE changed line, taking turns so every line gets updated
            for (int i = 0; i < 3; i++) {
                const int line = (nextLine + i) % 3;
                if (strcmp(wanted[line], shown[line]) != 0) {
                    controller.print(line, 0, "%-15s", wanted[line]); // pad to wipe old text
                    strcpy(shown[line], wanted[line]);
                    nextLine = line + 1;
                    break;
                }
            }

            pros::delay(50);
        }
    });
}