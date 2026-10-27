#include "pros/apix.h"
#include "ui.h"
#include "config.h"

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

    // Back button
    lv_obj_t* backBtn = lv_button_create(fieldScreen);
    lv_obj_set_size(backBtn, 120, 40);
    lv_obj_set_pos(backBtn, 150, 180);

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

    if (selectedAuton == AUTON_SKILLS)
        lv_obj_add_style(skillsBtn, &selectedStyle, LV_PART_MAIN);

    if (selectedAuton == AUTON_auton1)
        lv_obj_add_style(auton1Btn, &selectedStyle, LV_PART_MAIN);

    if (selectedAuton == AUTON_auton2)
        lv_obj_add_style(auton2Btn, &selectedStyle, LV_PART_MAIN);

    if (selectedAuton == AUTON_auton3)
        lv_obj_add_style(auton3Btn, &selectedStyle, LV_PART_MAIN);
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
    snprintf(buf, sizeof(buf), "Cascade: %d", cascadeVal);
    lv_label_set_text(cascadeLabel, buf);
    pros::delay(50);

}
