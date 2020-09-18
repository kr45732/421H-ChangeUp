#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

/* --- Brain Instance --- */
brain Brain;

/* --- Competition Instance --- */
competition Competition;

/* --- Motor constructors --- */
motor FrontLeft = motor(PORT5, ratio18_1, false);
motor FrontRight = motor(PORT10, ratio18_1, true);
motor BackLeft = motor(PORT14, ratio18_1, false);
motor BackRight = motor(PORT19, ratio18_1, true);
motor LeftIntake = motor(PORT7, ratio18_1, true);
motor RightIntake = motor(PORT6, ratio18_1, false);
motor BottomRoller = motor(PORT17, ratio18_1, false);
motor TopRoller = motor(PORT20, ratio18_1, true);

/* --- Sonar sensor constructors --- */
sonar LeftSonar = sonar(Brain.ThreeWirePort.A);
sonar RightSonar = sonar(Brain.ThreeWirePort.C);

/* --- Limit switch constructor --- */
limit LimitSwitch = limit(Brain.ThreeWirePort.H);

/* --- Inertial sensor constructor --- */
inertial InertialSensor = inertial(PORT16);

/* --- Vision sensor constructors --- */
signature Vision_RED =
    signature(1, 7015, 11731, 9372, -1801, -149, -976, 1.700, 0);
signature Vision_BLUE =
    signature(2, -3579, -1959, -2768, 7169, 12199, 9684, 3.000, 0);
vision VisionSensor = vision(PORT4, 150, Vision_RED, Vision_BLUE);

/* --- Controller constructor --- */
controller Controller = controller(primary);

// VEXcode generated functions

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void) {
  // nothing to initialize
}