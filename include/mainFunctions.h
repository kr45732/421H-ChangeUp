/* --- Function prototypes --- */
// -- Main functions -- //
void chassis(double forwardScale = 1.0, double turnScale = 1.0,
             double strafeScale = 1.0, int deadZone = 18);
void move(int degrees, int degreesPerSecond);
void strafe(int degrees, int degreesPerSecond);
void strafe(int inches, int degreesPerSecond, bool usingSonar);
void rotateTo(double degrees, double speed = 60);
void spinIntakes(int speed);
void spinBottomRollers(int speed);
void spinTopRollers(int speed);

// -- Helper functions -- //
void resetDriveEncoders();
double avgDriveEncoderValue();
void moveForward(int left, int right);
void moveStrafe(int left, int right);