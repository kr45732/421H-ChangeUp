/*---------------------------------------------------------------------------*/
/*    Module:       main.cpp                                                 */
/*    Author:       Krish Ranjan                                             */
/*    Created:      Thu Aug 13 2020                                          */
/*    Description:  Code for VEX Change Up 2020 - 2021                       */
/*----------------------------------------------------------------------------*/

/* --- Includes --- */
#include "vex.h"

/* --- Namespaces --- */
using namespace vex;

/* --- Button data structure --- */
typedef struct _rectangleButton {
  int xpos;
  int ypos;
  int width;
  int height;
  bool state;
  vex::color offColor;
  vex::color onColor;
  const char *label;
  bool changeOutline;
  vex::color fillColor;
} rectangleButton;

rectangleButton rectangleButtons[] = {
    {30, 30, 60, 60, false, 0xff0000, 0x005E16, "3"}, // Three towers autonomous
    {150, 30, 60, 60, false, 0xff0000, 0x005E16, "2"}, // Two towers autonomous
    {270, 30, 60, 60, false, 0xff0000, 0x005E16, "1"}, // One towers autonomous
    {390, 30, 60, 60, false, 0xff0000, 0x005E16, "0"}, // No autonomous
    {270, 168, 60, 62, false, 0xFFFFFF, 0x62fc03, "", true,
     0xE00000}, // Red alliance
    {390, 168, 60, 62, false, 0xFFFFFF, 0x62fc03, "", true,
     0x0000E0}, // Blue alliance
    {30, 168, 60, 62, false, 0xFFFFFF, 0x62fc03, "L", true,
     0xffd700}, // Starting on the left
    {150, 168, 60, 62, false, 0xFFFFFF, 0x62fc03, "R", true,
     0xffd700}, // Starting on the right
};

/* --- Brain LCD function prototypes --- */
int findButton(int16_t xpos, int16_t ypos);
void initButtons(std::string mode);
void userTouchCallbackPressed();
void userTouchCallbackReleased();
void displayButtonControls(int index, bool pressed);
void numberplate(color allianceColor);
void displayAutonSelection();
void banner();

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function. You must return from this function    */
/*  or the autonomous and usercontrol tasks will not be started. This        */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  // Initializing Robot Configuration
  vexcodeInit();

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
  InertialSensor.calibrate();
  while (InertialSensor.isCalibrating()) {
    wait(100, msec);
  }

  resetDriveEncoders();

  Brain.Screen.clearScreen();
  Controller.Screen.clearScreen();
}

/*---------------------------------------------------------------------------*/
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  if (rectangleButtons[4].state) {
    numberplate(red);
  } else {
    numberplate(vex::color(0x00bfff));
  }
  wait(20, msec);
  Controller.Screen.clearLine(5);
  wait(20, msec);
  Controller.Screen.print("Running...");

  // -- Tower 1 -- //
  strafe(30, 360, true);
  rotateTo(315, 75);
  move(600, 520);
  wait(1, sec);
  // -- Tower 2 -- //
  move(-90, 520);
  rotateTo(270);
  // -- Tower 3 -- //

  // -- Misc -- //

  // while (true){
  //   Brain.Screen.clearScreen();
  //   Brain.Screen.printAt(0, 20, "%0.2f", InertialSensor.heading());
  //   wait(20, msec);
  // }
}

/*---------------------------------------------------------------------------*/
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  /* Controller.Screen.clearScreen();
  while (1) {
    // Chassis
    chassis();

    // Intakes and rollers
    int speed = 900;
    if (Controller.ButtonR1.pressing()) {
      BottomRoller.spin(fwd, speed, dps);
      TopRoller.spin(fwd, speed, dps);
      intakes(speed);
    } else if (Controller.ButtonL1.pressing()) {
      BottomRoller.spin(fwd, -speed, dps);
      TopRoller.spin(fwd, -speed, dps);
      intakes(-speed);
    } else {
      intakes(0);
      BottomRoller.spin(fwd, 0, dps);
      TopRoller.spin(fwd, 0, dps);
    }

    // Sleep for a short amount of time to prevent wasted resources
    wait(25, msec);
  }*/

  int speed = 800;
  int blueQueue = 0;
  while (true) {
    if (Controller.ButtonR1.pressing()) {
      spinIntakes(speed);
      spinBottomRollers(speed);
      Brain.Screen.printAt(20, 20, "Queue: %d", blueQueue);
      Brain.Screen.printAt(20, 40, "Press: %d", LimitSwitch.value());
      if (blueQueue == 0) {
        if (VisionSensor.takeSnapshot(Vision_BLUE, 1) == 1) {
          blueQueue = 1;
        }
      } else if (blueQueue == 1) {
        if (LimitSwitch.pressing()) {
          spinTopRollers(speed);
          wait(500, msec);
          spinTopRollers(0);
          blueQueue = 0;
        }
      }
    } else {
      spinIntakes(0);
      spinBottomRollers(0);
      spinTopRollers(0);
    }
    wait(20, msec);
  }
}

// Main will set up the competition functions and callbacks
int main() {
  // Run the pre-autonomous function
  pre_auton();

  // Set up callbacks for autonomous and driver control periods
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Check if brain screen is pressed/released
  Brain.Screen.pressed(userTouchCallbackPressed);
  Brain.Screen.released(userTouchCallbackReleased);

  // 421H banner in center of screen
  // banner();

  // initial display
  // displayButtonControls(0, false);

  // Prevent main from exiting with an infinite loop.
  while (Competition.isAutonomous() && !Competition.isEnabled()) {
    displayAutonSelection();
    task::sleep(100);
  }

  while (true) {
    task::sleep(100);
  }
}

// /* --- Brain LCD functions --- */
int findButton(int16_t xpos, int16_t ypos) {
  int nButtons = sizeof(rectangleButtons) / sizeof(rectangleButton);

  for (int index = 0; index < nButtons; index++) {
    rectangleButton *pButton = &rectangleButtons[index];
    if (xpos < pButton->xpos || xpos > (pButton->xpos + pButton->width))
      continue;

    if (ypos < pButton->ypos || ypos > (pButton->ypos + pButton->height))
      continue;

    return (index);
  }
  return (-1);
} // Check if touch is inside button

void initButtons(std::string mode) {
  int nButtons = sizeof(rectangleButtons) / sizeof(rectangleButton);

  if (mode == "tower") {
    for (int index = 4; index < 6; index++) {
      rectangleButtons[index].state = false;
    }
  } else if (mode == "side") {
    for (int index = 6; index < nButtons; index++) {
      rectangleButtons[index].state = false;
    }
  } else {
    for (int index = 0; index < 4; index++) {
      rectangleButtons[index].state = false;
    }
  }
} // Init button states

void userTouchCallbackPressed() {
  if (!Competition.isEnabled()) {
    int index;
    int xpos = Brain.Screen.xPosition();
    int ypos = Brain.Screen.yPosition();

    if ((index = findButton(xpos, ypos)) >= 0) {
      displayButtonControls(index, true);
    }
  }
} // Screen has been touched

void userTouchCallbackReleased() {
  if (!Competition.isEnabled()) {
    int index;
    int xpos = Brain.Screen.xPosition();
    int ypos = Brain.Screen.yPosition();

    if ((index = findButton(xpos, ypos)) >= 0) {
      if (index == 4 || index == 5) {
        initButtons("tower");
      } else if (index == 6 || index == 7) {
        initButtons("side");
      } else {
        initButtons("");
      }

      // now set this one as true
      if (rectangleButtons[index].state == true) {
        rectangleButtons[index].state = false;
      } else {
        rectangleButtons[index].state = true;
      }

      displayButtonControls(index, false);
    }
  }
} // Screen has been (un)touched

void displayButtonControls(int index, bool pressed) {
  vex::color c;
  Brain.Screen.setPenColor(vex::color(0xe0e0e0));

  for (int i = 0; i < sizeof(rectangleButtons) / sizeof(rectangleButton); i++) {

    if (rectangleButtons[i].state)
      c = rectangleButtons[i].onColor;
    else
      c = rectangleButtons[i].offColor;

    Brain.Screen.setFillColor(c);

    if (rectangleButtons[i].changeOutline) {
      // button fill and outline
      Brain.Screen.setFillColor(rectangleButtons[i].fillColor);
      Brain.Screen.setPenColor(c);
      Brain.Screen.setPenWidth(5);
      Brain.Screen.drawRectangle(
          rectangleButtons[i].xpos, rectangleButtons[i].ypos,
          rectangleButtons[i].width, rectangleButtons[i].height);
    } else {
      Brain.Screen.setFillColor(c);
      Brain.Screen.setPenColor(white);
      Brain.Screen.setPenWidth(5);
      Brain.Screen.drawRectangle(
          rectangleButtons[i].xpos, rectangleButtons[i].ypos,
          rectangleButtons[i].width, rectangleButtons[i].height);

      Brain.Screen.setPenWidth(1);
      Brain.Screen.setPenColor(c);
      Brain.Screen.drawRectangle(
          rectangleButtons[i].xpos + 10, rectangleButtons[i].ypos - 2,
          rectangleButtons[i].width - 15, rectangleButtons[i].height + 8);
    }

    // draw label
    if (rectangleButtons[i].label != NULL) {
      Brain.Screen.setFont(fontType::mono60);
      Brain.Screen.setPenColor(white);
      Brain.Screen.printAt(rectangleButtons[i].xpos + 16,
                           rectangleButtons[i].ypos +
                               rectangleButtons[i].height - 11,
                           rectangleButtons[i].label);
    }
  }
} // Draw all buttons

void numberplate(color allianceColor) {
  Brain.Screen.clearScreen();

  Brain.Screen.setFillColor(allianceColor);
  Brain.Screen.setPenColor(allianceColor);
  Brain.Screen.drawRectangle(0, 0, 480, 240);
  Brain.Screen.setFont(fontType::mono60);

  Brain.Screen.setPenColor(vex::color(0xFFFFFF));
  Brain.Screen.setPenWidth(15);

  Brain.Screen.drawLine(25, 40, 25, 140);
  Brain.Screen.drawLine(18, 140, 100, 140);
  Brain.Screen.drawLine(100, 40, 100, 205);

  Brain.Screen.drawLine(150, 50, 230, 50);
  Brain.Screen.drawLine(230, 43, 230, 128);
  Brain.Screen.drawLine(150, 122, 230, 122);
  Brain.Screen.drawLine(155, 115, 155, 200);
  Brain.Screen.drawLine(150, 195, 235, 195);

  Brain.Screen.drawLine(300, 43, 300, 200);

  Brain.Screen.drawLine(375, 43, 375, 200);
  Brain.Screen.drawLine(375, 122, 455, 122);
  Brain.Screen.drawLine(450, 43, 450, 200);
} // Draw 421H on Brain lcd

void displayAutonSelection() {
  Controller.Screen.clearScreen();
  wait(20, msec);
  Controller.Screen.setCursor(1, 1);
  if (rectangleButtons[0].state) {
    Controller.Screen.print("Tower: 3");
  } else if (rectangleButtons[1].state) {
    Controller.Screen.print("Tower: 2");
  } else if (rectangleButtons[2].state) {
    Controller.Screen.print("Tower: 1");
  } else {
    Controller.Screen.print("Tower: 0");
  }

  Controller.Screen.setCursor(2, 1);
  if (rectangleButtons[6].state) {
    Controller.Screen.print("Side: Left");
  } else {
    Controller.Screen.print("Side: Right");
  }

  Controller.Screen.setCursor(3, 1);
  if (rectangleButtons[4].state) {
    Controller.Screen.print("Color: Red");
  } else {
    Controller.Screen.print("Color: Blue");
  }

  // if ((rectangleButtons[0].state || rectangleButtons[1].state ||
  //      rectangleButtons[2].state || rectangleButtons[3].state) &&
  //     (rectangleButtons[4].state || rectangleButtons[5].state)) {
  //   Controller.Screen.setCursor(5, 1);
  //   Controller.Screen.print("Ready!");
  // } else {
  //   Controller.Screen.setCursor(5, 1);
  //   Controller.Screen.print("Waiting...");
  // }
} // Display autonomous selection on remote

void banner() {
  Brain.Screen.setFillColor(vex::color(0x001169));
  Brain.Screen.setPenColor(vex::color(0x001169));
  Brain.Screen.drawRectangle(0, 110, 480, 40);

  Brain.Screen.setFillColor(vex::color::black);
  Brain.Screen.setPenColor(vex::color::black);
  Brain.Screen.drawRectangle(10, 122, 460, 15);

  Brain.Screen.setFont(fontType::mono40);

  Brain.Screen.setFillColor(vex::color::transparent);
  Brain.Screen.setPenColor(vex::color(0xFFFFFF));
  Brain.Screen.printAt(150, 142, "421H IHHS");
} // Print 421H banner on autonomous selection screen