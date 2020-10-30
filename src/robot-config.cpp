#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

/* --- Brain Instance --- */
brain Brain;

/* --- Competition Instance --- */
competition Competition;

/* --- Motor constructors --- */
motor FrontLeft = motor(PORT20, ratio18_1, false);
motor FrontRight = motor(PORT11, ratio18_1, true);
motor BackLeft = motor(PORT10, ratio18_1, false);
motor BackRight = motor(PORT1, ratio18_1, true);
motor LeftIntake = motor(PORT15, ratio18_1, false);
motor RightIntake = motor(PORT16, ratio18_1, true);
motor BottomRoller = motor(PORT17, ratio18_1, false);
motor TopRoller = motor(PORT4, ratio18_1, true);

/* --- Sonar sensor constructors --- */
sonar LeftSonar = sonar(Brain.ThreeWirePort.E);  // Output is E, input is F
sonar RightSonar = sonar(Brain.ThreeWirePort.A); // Output is A, input is B

/* --- Limit switch constructor --- */
limit LimitSwitch = limit(Brain.ThreeWirePort.C);

/* --- Inertial sensor constructor --- */
inertial InertialSensor = inertial(PORT9);

/* --- Vision sensor constructors --- */
signature Vision_RED =
    signature(1, 7171, 10083, 8626, -273, 447, 86, 3.000, 0);
signature Vision_BLUE =
    signature(2, -3067, -2345, -2706, 8881, 13061, 10972, 3.000, 0);
vision VisionSensor = vision(PORT7, 150, Vision_RED, Vision_BLUE);

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