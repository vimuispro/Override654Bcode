# Copilot instructions for Override654Bcode

## Architecture
- This is a PROS V5 C++ robot project using LemLib for drivetrain odometry and motion planning.
- `include/config.h` and `src/config.cpp` are the source of truth for robot-wide globals: controller, motor groups, sensors, drivetrain, and solenoids. Read these before changing subsystem behavior.
- `src/main.cpp` owns the PROS lifecycle hooks (`initialize`, `opcontrol`, `autonomous`) and picks the selected auton using `selectedAuton`.
- `src/driver.cpp` is the operator-control layer; it reads controller inputs and drives intake/cascade/claw behavior directly.
- `src/autons.cpp` contains autonomous path logic and issue commands on `lemlib::Chassis` (`turnToHeading`, `moveToPoint`, etc.).
- `src/ui.cpp` and `include/ui.h` build the LVGL auton selector and update pose display on the controller screen.
- Data flow is centralized: hardware objects are instantiated once in config, then passed into the chassis and subsystem classes; do not duplicate robot state in separate files.

## Build and debug workflow
- Build from the repo root with `make` or `make quick`; generated files appear under `bin/` and the linked firmware is under `firmware/`.
- Do not hand-edit generated artifacts in `bin/` or `firmware/`; treat them as build outputs.
- The project uses `pros::Task` for UI and calibration loops; this is the pattern for long-running background work.
- For quick testing, `src/main.cpp` calls `autonomous();` directly from driver control on button press; this is intentional for debugging, not a normal drive pattern.

## Project-specific conventions
- Keep hardware configuration centralized in `config.*` instead of repeating motor/sensor definitions across files.
- Reuse the existing global names (`controller`, `leftMotors`, `rightMotors`, `chassis`, `selectedAuton`, `driverControlEnabled`) instead of creating parallel state.
- Solenoids use uppercase names like `Claw` and `toggle`; they are toggled with `get_digital_new_press` patterns.
- Auton selection is enum-based in `include/ui.h`; button callbacks in `src/ui.cpp` update `selectedAuton`.
- Prefer direct LemLib commands on the existing `chassis` object over custom wrappers unless a new behavior is genuinely shared across multiple autons.
- The project is still evolving: `Autons::lift`, `Autons::score`, `skillsAuton`, and several auton stubs are placeholders, so do not assume all commands are implemented yet.

## Important files
- `include/config.h` / `src/config.cpp`: robot hardware and motion configuration
- `src/main.cpp`: top-level lifecycle and autonomous dispatch
- `src/driver.cpp`: teleop control logic
- `src/autons.cpp`: path planning and auton routines
- `src/ui.cpp`: LVGL auton selector and pose UI
