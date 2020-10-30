/*---------------------------------------------------------------------------*/
/*    Module:       main.cpp                                                 */
/*    Author:       Krish Ranjan                                             */
/*    Created:      Thu Aug 13 2020                                          */
/*    Description:  Code for VEX Change Up 2020 - 2021                       */
/*---------------------------------------------------------------------------*/

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
  InertialSensor.resetHeading();
  InertialSensor.resetRotation();

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

  resetDriveEncoders();
  InertialSensor.resetHeading();
  InertialSensor.resetRotation();
  wait(20, msec);
  
  /* -- Tower 1 -- */
  strafe(-14, 400, "right");
  rotateTo(0);

  spinBottomRollers(1200);
  spinTopRollers(1200);
  wait(0.5, sec);
  spinTopRollers(0);
  spinIntakes(1000);

  move(750, 540);
  spinIntakes(0);
  wait(0.25, seconds);
  spinBottomRollers(0);

  rotateTo(47);

  move(210, 450);

  BottomRoller.spinFor(fwd, 460, degrees, 1000, velocityUnits::dps, false);
  TopRoller.spinFor(fwd, 500, degrees, 1200, velocityUnits::dps);

  /* -- Tower 2 -- */
  move(-210, 450);
  spinTopRollers(-500);
  wait(0.35, msec);
  spinTopRollers(0);

  rotateTo(0);

  strafe(-25, 400, "right");

  rotateTo(0);

  spinIntakes(1000);
  spinBottomRollers(1200);
  move(210, 450);
  spinTopRollers(1200);
  wait(0.2, sec);
  spinTopRollers(0);
  wait(1.2, sec);
  spinTopRollers(-500);
  spinBottomRollers(-500);
  wait(0.4, seconds);
  spinBottomRollers(0);
  spinTopRollers(0);
  spinIntakes(0);

  rotateTo(0);

  move(-130, 450);

  rotateTo(270);

  spinIntakes(1000);
  spinBottomRollers(1200);
  move(700, 450);
  wait(0.5, sec);
  spinTopRollers(1200);
  wait(0.2, sec);
  spinTopRollers(0);
  spinIntakes(0);
  wait(0.25, seconds);
  spinBottomRollers(0);
  rotateTo(0);
  wait(0.25, sec);
  strafe(62, 400, "right");
  move(200, 450);
  BottomRoller.spinFor(fwd, 600, degrees, 1200, velocityUnits::dps, false);
  TopRoller.spinFor(fwd, 650, degrees, 1200, velocityUnits::dps);

  /* -- Tower 3 -- */
  move(-205, 450);
  rotateTo(0);
  strafe(-100, 400, "right");
  rotateTo(0);

  spinIntakes(1000);
  spinBottomRollers(1200);
  move(360, 450);
  spinTopRollers(1200);
  wait(0.2, sec);
  spinTopRollers(0);
  wait(1.3, sec);
  spinIntakes(0);
  wait(0.25, seconds);
  spinBottomRollers(0);

  rotateTo(0);

  move(-180, 450);

  rotateTo(310);

  move(350, 450);

  BottomRoller.spinFor(fwd, 400, degrees, 1200, velocityUnits::dps, false);
  TopRoller.spinFor(fwd, 450, degrees, 1200, velocityUnits::dps);

  /* -- Tower 4 -- */
  move(-350, 450);

  rotateTo(270);

  strafe(-62, 400, "right");

  rotateTo(270);

  move(250, 450);
  BottomRoller.spinFor(fwd, 500, degrees, 1200, velocityUnits::dps, false);
  TopRoller.spinFor(fwd, 550, degrees, 1200, velocityUnits::dps);

  /* -- Tower 4 -- */
  move(-160, 450);
  rotateTo(180);

  spinBottomRollers(1200);
  spinIntakes(1000);

  move(750, 540);
  spinIntakes(0);
  wait(0.25, seconds);
  spinBottomRollers(0);

  rotateTo(225);

  move(240, 450);

  BottomRoller.spinFor(fwd, 460, degrees, 1000, velocityUnits::dps, false);
  TopRoller.spinFor(fwd, 500, degrees, 1200, velocityUnits::dps);
}
/*---------------------------------------------------------------------------*/
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  int queue = 0;
  while (true) {
    // Chassis
    chassis();

    // Intakes and rollers
    if (Controller.ButtonR1.pressing()) {
      spinIntakes(1000);
      spinBottomRollers(1000);
      spinTopRollers(1000);
      Brain.Screen.printAt(20, 20, "Queue: %d", queue);
      Brain.Screen.printAt(20, 40, "Press: %d", LimitSwitch.value());
      
      if (queue == 0) {
        if (VisionSensor.takeSnapshot(Vision_BLUE, 1) == 1) {
          queue = 1;
        }
      } else if (queue == 1) {
        if (LimitSwitch.pressing()) {
          spinTopRollers(-1000);
          wait(1000, msec);
          spinTopRollers(1000);
          queue = 0;
        }
      }

    } else if (Controller.ButtonL1.pressing()) {
      spinIntakes(-1200);
      spinBottomRollers(-1200);
      spinTopRollers(-1200);
    } else {
      spinIntakes(0);
      spinBottomRollers(0);
      spinTopRollers(0);
    }

    
    // Sleep for a short amount of time to prevent wasted resources
    wait(20, msec);
  }
}

// Main will set up the competition functions and callbacks
int main() {
  // Run the \pre-autonomous function
  pre_auton();

  // Set up callbacks for autonomous and driver control periods
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Check if brain screen is pressed/released
  Brain.Screen.pressed(userTouchCallbackPressed);
  Brain.Screen.released(userTouchCallbackReleased);

  // 421H banner in center of screen
  banner();

  // initial display
  displayButtonControls(0, false);

  // Prevent main from exiting with an infinite loop.
  while (Competition.isAutonomous() && !Competition.isEnabled()) {
    displayAutonSelection();
    task::sleep(100);
  }

  while (true) {
    task::sleep(100);
  }
}

/* --- Brain LCD functions --- */
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