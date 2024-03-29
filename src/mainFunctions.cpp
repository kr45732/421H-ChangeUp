#include "vex.h"
using namespace vex;

/* --- Autonomous period functions --- */
// -- Main functions -- //
void move(int degrees, int degreesPerSecond) {
  // positive degrees = move forward
  // negative degrees = move backward
  int direction = abs(degrees) / degrees; // variable //

  resetDriveEncoders();
  InertialSensor.resetRotation();

  while (avgDriveEncoderValue() < abs(degrees)) {
    moveForward(degreesPerSecond * direction - (InertialSensor.rotation() * 25),
                degreesPerSecond * direction +
                    (InertialSensor.rotation() * 25));
    wait(20, msec);
  } // Conditional statment //

  moveForward(-600 * direction, -600 * direction);
  wait(125, msec);

  moveForward(0, 0);
  wait(20, msec);
} // Move forward or backward using internal motor encoders

void strafe(int degrees, int degreesPerSecond) {
  // positive degrees = strafe right
  // negative degrees = strafe left
  int direction = abs(degrees) / degrees;

  resetDriveEncoders();
  InertialSensor.resetRotation();

  while (avgDriveEncoderValue() < abs(degrees)) {
    moveStrafe(degreesPerSecond * direction - InertialSensor.rotation(),
               degreesPerSecond * direction + InertialSensor.rotation());
    wait(20, msec);
  }

  moveStrafe(-600 * direction, -600 * direction);
  wait(125, msec);

  moveStrafe(0, 0);
  wait(20, msec);
} // Strafe using internal motor encoders

void strafe(double inches, int degreesPerSecond, std::string sonarSide) {
  // positive inches = strafe right
  // negative inches = strafe left
  int direction = fabs(inches) / inches;

  InertialSensor.resetRotation();

  if (sonarSide == "right") {
    if (RightSonar.distance(distanceUnits::in) < fabs(inches) - 2) {
      while (RightSonar.distance(distanceUnits::in) < fabs(inches) - 2) {
        moveStrafe(degreesPerSecond * direction - InertialSensor.rotation(),
                   degreesPerSecond * direction + InertialSensor.rotation());
        wait(20, msec);
      }
    } else {
      while (RightSonar.distance(distanceUnits::in) > fabs(inches) + 4) {
        moveStrafe(degreesPerSecond * direction - InertialSensor.rotation(),
                   degreesPerSecond * direction + InertialSensor.rotation());
        wait(20, msec);
      }
      while (RightSonar.distance(distanceUnits::in) > fabs(inches) + 2) {
        moveStrafe(
            .2 * (degreesPerSecond * direction - InertialSensor.rotation()),
            .2 * (degreesPerSecond * direction + InertialSensor.rotation()));
        wait(20, msec);
      }
    }
  } else {
    while (LeftSonar.distance(distanceUnits::in) < fabs(inches) - 2) {
      moveStrafe(degreesPerSecond * direction - InertialSensor.rotation(),
                 degreesPerSecond * direction + InertialSensor.rotation());
      wait(20, msec);
    }
  }

  moveStrafe(-700 * direction, -700 * direction);
  wait(130, msec);

  moveStrafe(0, 0);
  wait(20, msec);
} // Strafe using sonar sensor

void rotateTo(double degrees, double speed) {
  double rightDeg = 0;
  double leftDeg = 0;
  double difference = 0; // Variable that keeps track of distance from target
  bool turnLeft = false;

  if (InertialSensor.heading() < degrees) {
    rightDeg = degrees - InertialSensor.heading();
    leftDeg = 360 + InertialSensor.heading() - degrees;
  } else {
    rightDeg = 360 - (InertialSensor.heading() - degrees);
    leftDeg = InertialSensor.heading() - degrees;
  }

  if (rightDeg > leftDeg) { // Turning left is shorter
    speed *= -1;
    difference = leftDeg;
    turnLeft = true;
  } else { // Turning right is shorter
    difference = rightDeg;
  }

  if (difference > 30) {
    FrontLeft.spin(forward, speed, pct);
    FrontRight.spin(reverse, speed, pct);
    BackLeft.spin(forward, speed, pct);
    BackRight.spin(reverse, speed, pct);
    task::sleep(200);

    while (difference > 30) {
      if (InertialSensor.heading() < degrees) {
        if (turnLeft) {
          difference = 360 + InertialSensor.heading() - degrees;
        } else {
          difference = degrees - InertialSensor.heading();
        }
      } else {
        if (turnLeft) {
          difference = InertialSensor.heading() - degrees;
        } else {
          difference = 360 - (InertialSensor.heading() - degrees);
        }
      }
      task::sleep(20);
    }
  }

  FrontLeft.spin(forward, speed * 0.1, pct);
  FrontRight.spin(reverse, speed * 0.1, pct);
  BackLeft.spin(forward, speed * 0.1, pct);
  BackRight.spin(reverse, speed * 0.1, pct);

  if (turnLeft) {
    int overshootError = 3;
    if (0 <= degrees && degrees <= overshootError) {
      waitUntil(((360 - overshootError) <= InertialSensor.heading() &&
                 InertialSensor.heading() <= 360) ||
                InertialSensor.heading() <= degrees);
    } else {
      waitUntil(InertialSensor.heading() <= degrees);
    }
  } else {
    waitUntil(InertialSensor.heading() >= (degrees));
  }

  FrontLeft.stop();
  FrontRight.stop();
  BackLeft.stop();
  BackRight.stop();
  wait(20, msec);
} // Rotating using inertial sensor

// -- Helper functions -- //
void resetDriveEncoders() {
  FrontLeft.resetPosition();
  FrontRight.resetPosition();
  BackLeft.resetPosition();
  BackRight.resetPosition();
} // Reset chassis motor encoders

double avgDriveEncoderValue() {
  return (fabs(FrontLeft.position(deg)) + fabs(FrontRight.position(deg)) +
          fabs(BackLeft.position(deg)) + fabs(BackRight.position(deg))) /
         4;
} // Get average value of all the chassis motor encoders

void moveForward(int left, int right) {
  FrontLeft.spin(fwd, left, dps);
  FrontRight.spin(fwd, right, dps);
  BackLeft.spin(fwd, left, dps);
  BackRight.spin(fwd, right, dps);
} // Spin chassis forward or backward

void moveStrafe(int left, int right) {
  FrontLeft.spin(fwd, left, dps);
  FrontRight.spin(fwd, -left, dps);
  BackLeft.spin(fwd, -right, dps);
  BackRight.spin(fwd, right, dps);
} // Strafe chassis right or left

/* --- Driver period functions --- */
void chassis(double forwardScale, double turnScale, double strafeScale,
             int deadZone) {
  double forward = Controller.Axis3.position();
  double turnVal = Controller.Axis4.position();
  double strafe = Controller.Axis1.position();

  int frontLeftVoltage = forward + strafe + turnVal;
  int frontRightVoltage = forward - strafe - turnVal;
  int backLeftVoltage = forward + strafe - turnVal;
  int backRightVoltage = forward - strafe + turnVal;

  bool frontLeftMoving =
      !(-deadZone < frontLeftVoltage && frontLeftVoltage < deadZone);
  bool frontRightMoving =
      !(-deadZone < frontRightVoltage && frontRightVoltage < deadZone);
  bool backLeftMoving =
      !(-deadZone < backLeftVoltage && backLeftVoltage < deadZone);
  bool backRightMoving =
      !(-deadZone < backRightVoltage && backRightVoltage < deadZone);

  if (frontLeftMoving || frontRightMoving || backLeftMoving ||
      backRightMoving) {
    FrontLeft.spin(fwd, frontLeftVoltage, rpm);
    FrontRight.spin(fwd, frontRightVoltage, rpm);
    BackLeft.spin(fwd, backLeftVoltage, rpm);
    BackRight.spin(fwd, backRightVoltage, rpm);

  } else {
    FrontLeft.stop(brakeType::brake);
    FrontRight.stop(brakeType::brake);
    BackLeft.stop(brakeType::brake);
    BackRight.stop(brakeType::brake);
  }
} // Moving, rotating, and strafing for chassis

void spinIntakes(int speed) {
  LeftIntake.spin(fwd, speed, dps);
  RightIntake.spin(fwd, speed, dps);
} // Spin intakes at a certian speed

void spinBottomRollers(int speed) {
  BottomRoller.spin(fwd, speed, dps);
} // Spin bottom at a certian dps

void spinTopRollers(int speed) {
  TopRoller.spin(fwd, speed, dps);
} // Spin top rollers at a certian dps

void sortBalls(double totalTime, signature removeColor) {
  auto startTime = std::chrono::high_resolution_clock::now();
  auto currentTime = std::chrono::high_resolution_clock::now();
  double duration =
      std::chrono::duration<double>(currentTime - startTime).count();

  while (duration < totalTime) {
    int queue = 0;
    if (queue == 0) {
      if (VisionSensor.takeSnapshot(removeColor, 1) == 1) {
        queue = 1;
      }
    } else if (queue == 1) {
      if (LimitSwitch.pressing()) {
        spinTopRollers(-1200);
        wait(1000, msec);
        spinTopRollers(1200);
        queue = 0;
      }
    }
    currentTime = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration<double>(currentTime - startTime).count();
  }
}

