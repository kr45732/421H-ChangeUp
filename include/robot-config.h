using namespace vex;

extern brain Brain;

using signature = vision::signature;

/* --- Vision sensor --- */
extern signature Vision_RED;
extern signature Vision_BLUE;
extern vision VisionSensor;

/* --- Sonar --- */
extern sonar LeftSonar;
extern sonar RightSonar;

/* --- Limit switch --- */
extern limit LimitSwitch;

/* --- Motors --- */
extern motor FrontLeft;
extern motor FrontRight;
extern motor BackLeft;
extern motor BackRight;
extern motor LeftIntake;
extern motor RightIntake;
extern motor BottomRoller;
extern motor TopRoller;

/* --- Inertial --- */
extern inertial InertialSensor;

/* --- Controller --- */
extern controller Controller;

/* --- Competition --- */
extern competition Competition;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void);