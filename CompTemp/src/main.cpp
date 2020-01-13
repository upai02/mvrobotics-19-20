#define _USE_MATH_DEFINES

#include "vex.h"
#include <cmath>
#include "MiniPID.h"

using namespace vex;  
int driveSpeedFactor = 1;
int auton = -1;
int allSpeed = 100;
int intake = 100;
int mode = 0;
std::string driveType = "Tank Controls";
std::string driveSpeed = "Drive Speed Normal"; 
double degreesPerFoot = 340.0;
double degreesTo360 = degreesPerFoot * 4;

competition Competition;
brain Brain;
motor RightFrontMotor = motor(PORT17, ratio18_1, true);
motor RightRearMotor = motor(PORT20, ratio18_1, true);
motor LeftRearMotor = motor(PORT19, ratio18_1, false);
motor LeftFrontMotor = motor(PORT14, ratio18_1, false);
motor LeftIntakeMotor = motor(PORT11, ratio18_1, false);
motor RightIntakeMotor = motor(PORT12, ratio18_1, true);
motor TilterMotor = motor(PORT18, ratio36_1, true);
motor BarMotor = motor(PORT13, ratio36_1, false);
pot Poten = pot(Brain.ThreeWirePort.H);
inertial Gyro = inertial(21);
motor_group LeftSide = motor_group(LeftFrontMotor, LeftRearMotor);
motor_group RightSide = motor_group(RightFrontMotor, RightRearMotor);
motor_group Intakes = motor_group(LeftIntakeMotor, RightIntakeMotor);
drivetrain Drivetrain = drivetrain(LeftSide, RightSide, 12.56, 13, 9.5, distanceUnits::in, 1.0);
smartdrive SmartDrive = smartdrive(LeftSide, RightSide, Gyro, 12.56, 13, 9.5, distanceUnits::in, 1.0);
controller Controller1;
MiniPID tilt = MiniPID(1, 0, 0);

extern void askPosition() {
  Brain.Screen.clearScreen();
  Brain.Screen.setFont(prop30);
  Brain.Screen.setPenColor(color(180, 180, 180));
  Brain.Screen.printAt(0, 30, "Battery Percentage: %d%%", Brain.Battery.capacity());
  Brain.Screen.setFont(prop20);
  Brain.Screen.drawLine(300, 0, 300, 40);
  Brain.Screen.setPenColor(color(0, 150, 0));
  Brain.Screen.drawRectangle(305, 0, 170, 40, color(0, 150, 0));
  Brain.Screen.setPenColor(black);
  Brain.Screen.printAt(365, 25, false, "Skills");
  Brain.Screen.setPenColor(color(180, 180, 180));
  Brain.Screen.setFont(prop30);
  Brain.Screen.drawLine(0, 40, 480, 40);
  Brain.Screen.setPenColor(color(180, 0, 0));
  Brain.Screen.drawRectangle(10, 50, 220, 85, color(180, 0, 0));
  Brain.Screen.setPenColor(black);
  Brain.Screen.printAt(95, 100, false, "RFS");
  Brain.Screen.setPenColor(color(0, 0, 180));
  Brain.Screen.drawRectangle(250, 50, 220, 85, color(0, 0, 180));
  Brain.Screen.setPenColor(black);
  Brain.Screen.printAt(335, 100, false, "BFS");
  Brain.Screen.setPenColor(color(180, 0, 0));
  Brain.Screen.drawRectangle(10, 145, 220, 85, color(180, 0, 0));
  Brain.Screen.setPenColor(black);
  Brain.Screen.printAt(95, 195, false, "RBS");
  Brain.Screen.setPenColor(color(0, 0, 180));
  Brain.Screen.drawRectangle(250, 145, 220, 85, color(0, 0, 180));
  Brain.Screen.setPenColor(black);
  Brain.Screen.printAt(335, 195, false, "BBS");
  Brain.Screen.setPenColor(color(180, 180, 180));
}

extern void pre_auton(void) {
  int xLastTouch = Brain.Screen.xPosition();
  int yLastTouch = Brain.Screen.yPosition();
  askPosition();
  while (Brain.Screen.xPosition() == xLastTouch && Brain.Screen.yPosition() == yLastTouch) {
    vex::task::sleep(20);
  }
  bool k = true;
  while (k) {
    if ((Brain.Screen.xPosition() >= 10 && Brain.Screen.xPosition() <= 230) && (Brain.Screen.yPosition() >= 50 && Brain.Screen.yPosition() <= 135)) {
      auton = 0;
      k = false;
    } else if ((Brain.Screen.xPosition() >= 250 && Brain.Screen.xPosition() <= 470) && (Brain.Screen.yPosition() >= 50 && Brain.Screen.yPosition() <= 135)) {
      auton = 1;
      k = false;
    } else if ((Brain.Screen.xPosition() >= 10 && Brain.Screen.xPosition() <= 230) && (Brain.Screen.yPosition() >= 145 && Brain.Screen.yPosition() <= 230)) {
      auton = 2;
      k = false;
    } else if ((Brain.Screen.xPosition() >= 250 && Brain.Screen.xPosition() <= 470) && (Brain.Screen.yPosition() >= 145 && Brain.Screen.yPosition() <= 230)) {
      auton = 3;
      k = false;
    } else if ((Brain.Screen.xPosition() >= 305 && Brain.Screen.xPosition() <= 475) && (Brain.Screen.yPosition() >= 0 && Brain.Screen.yPosition() <= 40)) {
      auton = 4;
      k = false;
    }
  }

  if (auton == 0) {
    Brain.Screen.clearScreen();
    Brain.Screen.print("RFS Selected");
    Controller1.Screen.clearScreen();
    Controller1.Screen.print("RFS Selected");
  } else if (auton == 1) {
    Brain.Screen.clearScreen();
    Brain.Screen.print("BFS Selected");
    Controller1.Screen.clearScreen();
    Controller1.Screen.print("BFS Selected");
  } else if (auton == 2) {
    Brain.Screen.clearScreen();
    Brain.Screen.print("RBS Selected");
    Controller1.Screen.clearScreen();
    Controller1.Screen.print("RBS Selected");
  } else if (auton == 3) {
    Brain.Screen.clearScreen();
    Brain.Screen.print("BBS Selected");
    Controller1.Screen.clearScreen();
    Controller1.Screen.print("BBS Selected");
  } else if (auton == 4) {
    Brain.Screen.clearScreen();
    Brain.Screen.print("Skills Selected");
    Controller1.Screen.clearScreen();
    Controller1.Screen.print("Skills Selected");
  }
}

void deployStack() {
  TilterMotor.setBrake(brake);
  double error = 2000;
  double tilt_speed = 100;
  while(error > 100 && tilt_speed > 5) {
    error = std::abs(1400-Poten.angle(rotationUnits::raw));
    tilt_speed = error/25;
    TilterMotor.spin(fwd, tilt_speed, pct);
  }
  // wait(1, sec);
  // Drivetrain.driveFor(directionType::rev, 6, inches);
  // while(Poten.value(rotationUnits::raw) < 3700) {
  //   TilterMotor.spin(directionType::rev, 100, pct);
  // }
}

void flipOut() {
  TilterMotor.setVelocity(100, pct);
  BarMotor.setVelocity(100, pct);
  TilterMotor.spinFor(fwd, 425, deg);
  wait(50, msec);
  TilterMotor.spinFor(directionType::rev, 425, deg, false);
  BarMotor.spinFor(fwd, 400, deg);
  wait(1, msec);
  BarMotor.spinFor(directionType::rev, 400, deg);
}

// void turnLeft(double deg) {
//   double error = 10;
//   double Kp = 1;
//   double speed = 0;
//   double ang = deg;
//   if (ang >= 180) {
//     while (Gyro.rotation(rotationUnits::deg) > ang) {
//       error = std::abs(ang - Gyro.rotation(rotationUnits::deg));
//       speed = error*Kp;
//       RightSide.spin(fwd, speed, pct);
//       LeftSide.spin(directionType::rev, speed, pct);
//     }
//   }
// }

// void turnRight(double deg) {
//   double error = 100;
//   double Kp = 0.5;
//   double speed = 0;
//   double ang = deg;
//   if (ang <= 180) {
//     while (Gyro.rotation() < ang) {
//       error = std::abs(ang - Gyro.rotation());
//       speed = error*Kp;
//       RightSide.spin(directionType::rev, speed, pct);
//       LeftSide.spin(fwd, speed, pct);
//     }
//   }
// }

void stop_robot(){
    RightSide.stop();
    LeftSide.stop();
    RightSide.resetRotation();
    LeftSide.resetRotation();
}

void moveForward(double feet, double speed){
    double degrees = feet * degreesPerFoot;
    RightFrontMotor.startRotateFor(degrees, vex::rotationUnits::deg, speed, vex::velocityUnits::rpm);
    LeftFrontMotor.startRotateFor(degrees, vex::rotationUnits::deg, speed, vex::velocityUnits::rpm);
    RightRearMotor.startRotateFor(degrees, vex::rotationUnits::deg, speed, vex::velocityUnits::rpm);
    LeftRearMotor.rotateFor(degrees, vex::rotationUnits::deg, speed, vex::velocityUnits::rpm);
}

void moveBackwards(double feet, double speed){
    double degrees = feet * degreesPerFoot;
    RightFrontMotor.startRotateFor(-degrees, vex::rotationUnits::deg, -speed, vex::velocityUnits::rpm);
    RightRearMotor.startRotateFor(-degrees, vex::rotationUnits::deg, -speed, vex::velocityUnits::rpm);
    LeftFrontMotor.startRotateFor(-degrees, vex::rotationUnits::deg, -speed, vex::velocityUnits::rpm);
    LeftRearMotor.rotateFor(-degrees, vex::rotationUnits::deg, -speed, vex::velocityUnits::rpm);
}

void turnLeft(double ang, double speed){
    double degrees = (ang / 360) * degreesTo360;

    LeftFrontMotor.startRotateTo(-degrees, vex::rotationUnits::deg, -speed, vex::velocityUnits::rpm);
    RightFrontMotor.startRotateTo(degrees, vex::rotationUnits::deg, speed, vex::velocityUnits::rpm);
    LeftRearMotor.startRotateTo(-degrees, vex::rotationUnits::deg, -speed, vex::velocityUnits::rpm);
    RightRearMotor.rotateTo(degrees, vex::rotationUnits::deg, speed, vex::velocityUnits::rpm);
}

void turnRight(double ang, double speed){
    double degrees = (ang / 360) * degreesTo360;
    LeftFrontMotor.startRotateTo(degrees, vex::rotationUnits::deg, speed, vex::velocityUnits::rpm);
    RightFrontMotor.startRotateTo(-degrees, vex::rotationUnits::deg, -speed, vex::velocityUnits::rpm);
    LeftRearMotor.startRotateTo(degrees, vex::rotationUnits::deg, speed, vex::velocityUnits::rpm);
    RightRearMotor.rotateTo(-degrees, vex::rotationUnits::deg, -speed, vex::velocityUnits::rpm);
}

void drive_Tank(double deadzone){
  LeftSide.spin(vex::directionType::fwd, (Controller1.Axis3.value() > deadzone || Controller1.Axis3.value() < -deadzone) ? Controller1.Axis3.value() : 0, vex::velocityUnits::pct);
  RightSide.spin(vex::directionType::fwd, (Controller1.Axis2.value() > deadzone || Controller1.Axis2.value() < -deadzone) ? Controller1.Axis2.value() : 0, vex::velocityUnits::pct);
}

void drive_Arcade(double deadzone){
  LeftSide.spin(vex::directionType::fwd, ((Controller1.Axis3.value() > deadzone || Controller1.Axis3.value() < -deadzone) ? Controller1.Axis3.value() : 0) + ((Controller1.Axis1.value() > deadzone || Controller1.Axis1.value() < -deadzone) ? Controller1.Axis1.value() : 0), vex::velocityUnits::pct);
  RightSide.spin(vex::directionType::fwd, ((Controller1.Axis3.value() > deadzone || Controller1.Axis3.value() < -deadzone) ? Controller1.Axis3.value() : 0) - ((Controller1.Axis1.value() > deadzone || Controller1.Axis1.value() < -deadzone) ? Controller1.Axis1.value() : 0), vex::velocityUnits::pct);
}

void autoRBS() {
  // Drivetrain.driveFor(fwd, 6, inches);
  // Drivetrain.driveFor(directionType::rev, 6, inches, false);
  // flipOut();
  Drivetrain.driveFor(fwd, 42.5, inches, false);
  while (Drivetrain.isMoving()) {
    Intakes.spin(fwd, 100, pct);
  }
  Drivetrain.driveFor(directionType::rev, 42.5, inches);
  Drivetrain.driveFor(fwd, 8, inches);
  turnRight(135, 50);
  Intakes.stop();
  Drivetrain.driveFor(fwd, 10, inches);
  Intakes.spinFor(directionType::rev, 2, rev);
  wait(100, msec);
  deployStack();
  wait(100, msec); 
  Intakes.spinFor(directionType::rev, 2, rev, false);
  Drivetrain.driveFor(directionType::rev, 12, inches);
}

void autoBBS() {
  // SmartDrive.driveFor(fwd, 6, inches);
  // SmartDrive.driveFor(directionType::rev, 6, inches, false);
  // flipOut();
  SmartDrive.driveFor(fwd, 42.5, inches, false);
  while (SmartDrive.isMoving()) {
    Intakes.spin(fwd, 100, pct);
  }
  SmartDrive.driveFor(directionType::rev, 42.5, inches);
  SmartDrive.driveFor(fwd, 8, inches);
  turnLeft(135, 50);
  Intakes.stop();
  SmartDrive.driveFor(fwd, 10, inches);
  Intakes.spinFor(directionType::rev, 2, rev);
  wait(100, msec);
  deployStack();
  wait(100, msec); 
  Intakes.spinFor(directionType::rev, 2, rev, false);
  SmartDrive.driveFor(directionType::rev, 12, inches);
}

void autoRFS() {
  // Drivetrain.driveFor(fwd, 6, inches);
  // Drivetrain.driveFor(directionType::rev, 6, inches, false);
  // flipOut();
  Drivetrain.driveFor(fwd, 42.5, inches, false);
  while (Drivetrain.isMoving()) {
    Intakes.spin(fwd, 100, pct);
  }
  Drivetrain.driveFor(directionType::rev, 23.4, inches);
  turnLeft(90, 50);
  Drivetrain.driveFor(fwd, 23.4, inches, false);
  while (Drivetrain.isMoving()) {
    Intakes.spin(fwd, 100, pct);
  }
  turnLeft(45, 50);
  Intakes.stop();
  Drivetrain.driveFor(fwd, 6, inches);
  Intakes.spinFor(directionType::rev, 2, rev);
  wait(100, msec);
  deployStack();
  wait(100, msec);
  Intakes.spinFor(directionType::rev, 2, rev, false);
  Drivetrain.driveFor(directionType::rev, 12, inches);
}

void autoBFS() {
  // SmartDrive.driveFor(fwd, 6, inches);
  // SmartDrive.driveFor(directionType::rev, 6, inches, false);
  // flipOut();
  SmartDrive.driveFor(fwd, 42.5, inches, false);
  while (SmartDrive.isMoving()) {
    Intakes.spin(fwd, 100, pct);
  }
  SmartDrive.driveFor(directionType::rev, 23.4, inches);
  turnLeft(90, 50);
  SmartDrive.driveFor(fwd, 23.4, inches, false);
  while (SmartDrive.isMoving()) {
    Intakes.spin(fwd, 100, pct);
  }
  turnRight(45, 50);
  Intakes.stop();
  SmartDrive.driveFor(fwd, 6, inches);
  Intakes.spinFor(directionType::rev, 2, rev);
  wait(100, msec);
  deployStack();
  wait(100, msec);
  Intakes.spinFor(directionType::rev, 2, rev, false);
  SmartDrive.driveFor(directionType::rev, 12, inches);
}

void autoSkills() {
  // Drivetrain.setDriveVelocity(50, pct);
  // Drivetrain.driveFor(directionType::rev, 16, inches);
  // wait(100, msec);
  // Drivetrain.driveFor(fwd, 22, inches); 
  turnRight(90, 50);
}

void autonomous(void) {
  Gyro.calibrate();
  Drivetrain.setStopping(hold);
  Intakes.setStopping(coast);
  BarMotor.setStopping(brake);
  TilterMotor.setStopping(brake);
  Drivetrain.setDriveVelocity(30, pct);
  if (auton == 0) {
    Controller1.Screen.clearScreen();
    Controller1.Screen.print("Running RFS");
    autoRFS();
  } else if (auton == 1) {
    Controller1.Screen.clearScreen();
    Controller1.Screen.print("Running BFS");
    autoBFS();
    } else if (auton == 2) {
    Controller1.Screen.clearScreen();
    Controller1.Screen.print("Running RBS");
    autoRBS();
  } else if (auton == 3) {
    Controller1.Screen.clearScreen();
    Controller1.Screen.print("Running BBS");
    autoBBS();
  } else if (auton == 4) {
    Controller1.Screen.clearScreen();
    Controller1.Screen.print("Running Skills");
    autoSkills();
  } else {
    // Doesn't run Auton
    Controller1.Screen.clearScreen();
    Controller1.Screen.print("No Auton Running");
  }
}

double vel(double distT, double distF) {
  double num = sin(M_PI * ((distT-distF)/distF)) * 127 + 10;
  return num;
}

void intToggle() {
  while (intake) {
    Intakes.spin(fwd, 100, pct);
    if (Controller1.ButtonRight.pressing()) {
      break;
    }
  }
}

void usercontrol(void) {
  LeftSide.setStopping(coast);
  RightSide.setStopping(coast);
  Intakes.setStopping(coast);
  BarMotor.setStopping(hold);
  TilterMotor.setStopping(brake);
  while (1) {
    // Drive Commands
    if (mode == 0) {
      drive_Tank(15);
    } else if (mode == 1) {
      drive_Arcade(15);
    }
    
    // Tilter Commands
    if (Controller1.ButtonL1.pressing()) {
      TilterMotor.spin(directionType::fwd, 45, velocityUnits::pct);
    } else if (Controller1.ButtonL2.pressing()) {
      TilterMotor.spin(directionType::rev, 45, velocityUnits::pct);
    } else {
      TilterMotor.stop(brake);
    }

    // Intake Commands
    if (Controller1.ButtonR1.pressing()) {
      Intakes.spin(directionType::fwd, intake, velocityUnits::pct);
    } else if (Controller1.ButtonR2.pressing()) {
      Intakes.spin(directionType::rev, 50, velocityUnits::pct);
    } else {
      Intakes.stop(brake);
    }

    // Bar Commands
    if (Controller1.ButtonUp.pressing()) {
      BarMotor.spin(directionType::fwd, allSpeed, velocityUnits::pct);
      // if (Poten.angle(rotationUnits::raw) > 2800) {
      //   TilterMotor.spin(directionType::fwd, 45, pct);
      // }
    } else if (Controller1.ButtonDown.pressing()) {
      BarMotor.spin(directionType::rev, allSpeed, velocityUnits::pct);
      //TilterMotor.spin(directionType::rev, 35, pct);
    } else {
      BarMotor.stop(hold);
    }

    // Speed Control
    // if (Controller1.ButtonA.pressing()) {
    //   if (driveSpeedFactor == 1) {
    //   driveSpeedFactor = 2;

    //   driveSpeed = "Drive Speed Halved";
    //   }
    //   else if (driveSpeedFactor == 2) {
    //   driveSpeedFactor = 1;
    //   driveSpeed = "Drive Speed Normal";
    //   }
    //   wait(50, msec);
    // }

    // Drive Select
    if (Controller1.ButtonX.pressing()) {
      if (mode == 0) {
        mode = 1;
        driveType = "Split Arcade Controls";
      } else if (mode == 1) {
        mode = 0;
        driveType = "Tank Controls";
      }
      wait(50, msec);
    }

    if (Controller1.ButtonA.pressing()) {
      deployStack();
    }

    if (Controller1.ButtonB.pressing()) {
      SmartDrive.driveFor(directionType::rev, 6, distanceUnits::in);
    }

    if (Controller1.ButtonY.pressing()) {
      flipOut();
    }

    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(0, 0);
    Controller1.Screen.print(driveType.c_str());
    Controller1.Screen.newLine();
    // Controller1.Screen.print(driveSpeed.c_str());

    // wait(20, msec);
  }
}

int main() {
  // Run the pre-autonomous function.
  pre_auton();

  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}