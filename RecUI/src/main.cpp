#define _USE_MATH_DEFINES

#include "vex.h"
#include <cmath>
#include "MiniPID.h"

using namespace vex;
double driveSpeedFactor = 1.25;
std::string driveSpeed = "Drive Speed Normal";
int mode = 0;
int barPos = 0;
bool rec = false;
bool pl = false;
bool rf = false;    
bool bf = false;
bool rs = false;
bool bs = false;
bool sk = false;
int allSpeed = 100;
std::string driveType = "Hybrid Controls";
int driveMode = 2;

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
inertial Inertial = inertial(PORT21);
motor_group LeftSide = motor_group(LeftFrontMotor, LeftRearMotor);
motor_group RightSide = motor_group(RightFrontMotor, RightRearMotor);
motor_group Intakes = motor_group(LeftIntakeMotor, RightIntakeMotor);
drivetrain Drivetrain = drivetrain(LeftSide, RightSide, 12.56, 13, 9.5, distanceUnits::in, 1.0);
smartdrive SmartDrive = smartdrive(LeftSide, RightSide, Inertial, 12.56, 13, 9.5, distanceUnits::in, 1.0);
controller Controller1;

extern void screenReset() {
  Brain.Screen.clearScreen();
  Brain.Screen.setFont(prop30);
  Brain.Screen.setPenColor(color(180, 180, 180));
  Brain.Screen.printAt(0, 30, "Battery Percentage: %d%%", Brain.Battery.capacity());
  Brain.Screen.drawLine(300, 0, 300, 40);
  Brain.Screen.setPenColor(color(180, 180, 180));
  Brain.Screen.drawLine(0, 40, 480, 40);
}

extern void askMode() {
  screenReset();
  Brain.Screen.setPenColor(color(180, 0, 0));
  Brain.Screen.drawRectangle(10, 50, 220, 180, color(180, 0, 0));
  Brain.Screen.setPenColor(black);
  Brain.Screen.printAt(70, 145, false, "Record");
  Brain.Screen.setPenColor(color(0, 0, 180));
  Brain.Screen.drawRectangle(250, 50, 220, 180, color(0, 0, 180));
  Brain.Screen.setPenColor(black);
  Brain.Screen.printAt(305, 145, false, "Playback");
}

extern void askPosition() {
  screenReset();
  Brain.Screen.setFont(prop20);
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
  Brain.Screen.printAt(95, 100, false, "Red");
  Brain.Screen.setPenColor(color(0, 0, 180));
  Brain.Screen.drawRectangle(250, 50, 220, 85, color(0, 0, 180));
  Brain.Screen.setPenColor(black);
  Brain.Screen.printAt(335, 100, false, "Blue");
  Brain.Screen.setPenColor(color(180, 0, 0));
  Brain.Screen.drawRectangle(10, 145, 220, 85, color(180, 0, 0));
  Brain.Screen.setPenColor(black);
  Brain.Screen.printAt(60, 195, false, "Red Slow");
  Brain.Screen.setPenColor(color(0, 0, 180));
  Brain.Screen.drawRectangle(250, 145, 220, 85, color(0, 0, 180));
  Brain.Screen.setPenColor(black);
  Brain.Screen.printAt(300, 195, false, "Blue Slow");
  Brain.Screen.setPenColor(color(180, 180, 180));
}

extern void pre_auton(void) {
  int xLastTouch = Brain.Screen.xPosition();
  int yLastTouch = Brain.Screen.yPosition();
  askMode();
  while (Brain.Screen.xPosition() == xLastTouch && Brain.Screen.yPosition() == yLastTouch) {
    vex::task::sleep(200);
  }
  bool m = true;
  bool h = true;
  while (h) {
    if ((Brain.Screen.xPosition() >= 10 && Brain.Screen.xPosition() <= 230) && (Brain.Screen.yPosition() >= 50 && Brain.Screen.yPosition() <= 230)) {
      rec = true;
      h = false;
    } else if ((Brain.Screen.xPosition() >= 250 && Brain.Screen.xPosition() <= 470) && (Brain.Screen.yPosition() >= 50 && Brain.Screen.yPosition() <= 230)) {
      pl = true;
      h = false;
    }
  }
  int xLastTouch1 = Brain.Screen.xPosition();
  int yLastTouch1 = Brain.Screen.yPosition();
  askPosition();
    while (Brain.Screen.xPosition() == xLastTouch1 && Brain.Screen.yPosition() == yLastTouch1) {
      vex::task::sleep(20);
    }
    while (m) {
      if ((Brain.Screen.xPosition() >= 10 && Brain.Screen.xPosition() <= 230) && (Brain.Screen.yPosition() >= 50 && Brain.Screen.yPosition() <= 135)) {
        rf = true;
        m = false;
      } else if ((Brain.Screen.xPosition() >= 250 && Brain.Screen.xPosition() <= 470) && (Brain.Screen.yPosition() >= 50 && Brain.Screen.yPosition() <= 135)) {
        bf = true;
        m = false;
      } else if ((Brain.Screen.xPosition() >= 10 && Brain.Screen.xPosition() <= 230) && (Brain.Screen.yPosition() >= 145 && Brain.Screen.yPosition() <= 230)) {
        rs = true;
        m = false;
      } else if ((Brain.Screen.xPosition() >= 250 && Brain.Screen.xPosition() <= 470) && (Brain.Screen.yPosition() >= 145 && Brain.Screen.yPosition() <= 230)) {
        bs = true;
        m = false;
      } else if ((Brain.Screen.xPosition() >= 305 && Brain.Screen.xPosition() <= 475) && (Brain.Screen.yPosition() >= 0 && Brain.Screen.yPosition() <= 40)) {
        sk = true;
        m = false;
      }
    }

  if (rec) {
    if (rf) {
      Brain.Screen.clearScreen();
      Brain.Screen.print("Red Recorder Selected");
      Controller1.Screen.clearScreen();
      Controller1.Screen.print("Red Recorder Selected");
    } else if (bf) {
      Brain.Screen.clearScreen();
      Brain.Screen.print("Blue Recorder Selected");
      Controller1.Screen.clearScreen();
      Controller1.Screen.print("Blue Recorder Selected");
    } else if (rs) {
      Brain.Screen.clearScreen();
      Brain.Screen.print("Red Slow Recorder Selected");
      Controller1.Screen.clearScreen();
      Controller1.Screen.print("Red Slow Recorder Selected");
    } else if (bs) {
      Brain.Screen.clearScreen();
      Brain.Screen.print("Blue Slow Recorder Selected");
      Controller1.Screen.clearScreen();
      Controller1.Screen.print("Blue Slow Recorder Selected");
    } else if (sk) {
      Brain.Screen.clearScreen();
      Brain.Screen.print("Skills Recorder Selected");
      Controller1.Screen.clearScreen();
      Controller1.Screen.print("Skills Recorder Selected");
    }
  } else if (pl) {
    if (rf) {
      Brain.Screen.clearScreen();
      Brain.Screen.print("Red Playback Selected");
      Controller1.Screen.clearScreen();
      Controller1.Screen.print("Red Playback Selected");
    } else if (bf) {
      Brain.Screen.clearScreen();
      Brain.Screen.print("Blue Playback Selected");
      Controller1.Screen.clearScreen();
      Controller1.Screen.print("Blue Playback Selected");
    } else if (rs) {
      Brain.Screen.clearScreen();
      Brain.Screen.print("Red Slow Playback Selected");
      Controller1.Screen.clearScreen();
      Controller1.Screen.print("Red Slow Playback Selected");
    } else if (bs) {
      Brain.Screen.clearScreen();
      Brain.Screen.print("Blue Slow Playback Selected");
      Controller1.Screen.clearScreen();
      Controller1.Screen.print("Blue Slow Playback Selected");
    } else if (sk) {
      Brain.Screen.clearScreen();
      Brain.Screen.print("Skills Playback Selected");
      Controller1.Screen.clearScreen();
      Controller1.Screen.print("Skills Playback Selected");
    }
  }
}

void deployStack() {
  TilterMotor.setBrake(brake);
  double error = 1770;
  double tilt_speed = 100;
  while (std::abs(error) > 10 && std::abs(tilt_speed) > 5) {
    error = std::abs(1770-Poten.angle(rotationUnits::raw));
    tilt_speed = error/17.5;
    TilterMotor.spin(fwd, tilt_speed, pct);
    if (tilt_speed < 5) {
      break;
    }
  }
}

void turn(double degrees) {
  if (Inertial.installed()) {
    double target = degrees;
    double error = target - Inertial.rotation();
    double kP = .5;
    if (error > 0) {
      while (std::abs(error) > 1) {
        error = target - Inertial.rotation();
        double percent = kP * error + 15;
        LeftSide.spin(directionType::fwd, percent, pct);
	      RightSide.spin(directionType::rev, percent, pct);
        vex::task::sleep(20);
      }
    } else if (error > 0) {
      double target = degrees; 
      double error = target - Inertial.rotation();
      double kP = .5;
      while (std::abs(error) > 1) {
        error = target - Inertial.rotation();
        double percent = kP * error - 15;
        LeftSide.spin(directionType::fwd, percent, pct);
	      RightSide.spin(directionType::rev, percent, pct);
        vex::task::sleep(20);
      }
    } else {
      LeftSide.stop();
      RightSide.stop();
      goto end;
    }
  } else {
    Brain.Screen.clearScreen();
    Brain.Screen.setFont(fontType::mono40);
    Brain.Screen.setFillColor(red);
    Brain.Screen.print("No Inertial Sensor Installed");
  }
  end:
  vex::task::sleep(10);
}

void flipOut() {
  TilterMotor.setVelocity(100, pct);
  Intakes.setVelocity(100, pct);
  TilterMotor.spinFor(fwd, 450, rotationUnits::deg, false);
  wait(25, msec);  
  Intakes.spinFor(directionType::rev, 2.5, rotationUnits::rev);
  TilterMotor.spinFor(directionType::rev, 410, rotationUnits::deg);
}

void drive_Tank(double deadzone){
  LeftSide.spin(vex::directionType::fwd, ((Controller1.Axis3.value() > deadzone || Controller1.Axis3.value() < -deadzone) ? Controller1.Axis3.value() : 0)/driveSpeedFactor, vex::velocityUnits::pct);
  RightSide.spin(vex::directionType::fwd, ((Controller1.Axis2.value() > deadzone || Controller1.Axis2.value() < -deadzone) ? Controller1.Axis2.value() : 0)/driveSpeedFactor, vex::velocityUnits::pct);
}

void drive_Arcade(double deadzone){
  LeftSide.spin(vex::directionType::fwd, (((Controller1.Axis3.value() > deadzone || Controller1.Axis3.value() < -deadzone) ? Controller1.Axis3.value() : 0) + ((Controller1.Axis1.value() > deadzone || Controller1.Axis1.value() < -deadzone) ? Controller1.Axis1.value() : 0))/driveSpeedFactor, vex::velocityUnits::pct);
  RightSide.spin(vex::directionType::fwd, (((Controller1.Axis3.value() > deadzone || Controller1.Axis3.value() < -deadzone) ? Controller1.Axis3.value() : 0) - ((Controller1.Axis1.value() > deadzone || Controller1.Axis1.value() < -deadzone) ? Controller1.Axis1.value() : 0))/driveSpeedFactor, vex::velocityUnits::pct);
}

void drive_Hybrid(double deadzone) {
  LeftSide.spin(vex::directionType::fwd, (((Controller1.Axis3.value() > deadzone || Controller1.Axis3.value() < -deadzone) ? Controller1.Axis3.value() : 0) - ((Controller1.Axis2.value() > deadzone || Controller1.Axis2.value() < -deadzone) ? Controller1.Axis2.value() : 0))/driveSpeedFactor, vex::velocityUnits::pct);
  RightSide.spin(vex::directionType::fwd, (((Controller1.Axis3.value() > deadzone || Controller1.Axis3.value() < -deadzone) ? Controller1.Axis3.value() : 0) + ((Controller1.Axis2.value() > deadzone || Controller1.Axis2.value() < -deadzone) ? Controller1.Axis2.value() : 0))/driveSpeedFactor, vex::velocityUnits::pct);
}

void spdToggle() {
  if (driveSpeedFactor == 1.25) {
    driveSpeedFactor = 2.5;
    driveSpeed = "Drive Speed Halved";
  } else if (driveSpeedFactor == 2.5) {
    driveSpeedFactor = 1.25;
    driveSpeed = "Drive Speed Normal";
  }
  wait(500, msec);
}

void driveSelect() {
  if (mode == 0) {
    mode = 1;
    driveType = "Split Arcade Controls";
  } else if (mode == 1) {
    mode = 2;
    driveType = "Hybrid Controls";
  } else if (mode == 2) {
    mode = 0;
    driveType = "Tank Controls";
  }
}

void fadeBack() {
  Intakes.setVelocity(50, percentUnits::pct);
  Intakes.spinFor(directionType::rev, 3, rotationUnits::rev);
  SmartDrive.driveFor(directionType::rev, 6, distanceUnits::in);
}

void barMacro() {
  if (barPos == 0) {
    TilterMotor.spinFor(fwd, 250, deg, false);
    BarMotor.spinFor(directionType::fwd, 500, rotationUnits::deg);
    barPos = 1;
  } else if (barPos == 1) {
    BarMotor.spinFor(directionType::fwd, 250, rotationUnits::deg);
    barPos = 2;
  } else if (barPos == 2) {
    BarMotor.spinFor(directionType::rev, 750, deg, false);
    TilterMotor.spinFor(directionType::rev, 250, deg);
    barPos = 0;
  }
}

void controls() {
    // Drive Commands
    if (mode == 0) {
      drive_Tank(17.5);
    } else if (mode == 1) {
      drive_Arcade(17.5);
    } else if (mode == 2) {
      drive_Hybrid(17.5);
    }
    
    // Tilter Commands
    if (Controller1.ButtonL1.pressing()) {
      if (Poten.angle(rotationUnits::raw) < 2000) {
        TilterMotor.spin(directionType::fwd, 25, velocityUnits::pct);
      } else {
        TilterMotor.spin(directionType::fwd, 35, velocityUnits::pct);
      }
    } else if (Controller1.ButtonL2.pressing()) {
      TilterMotor.spin(directionType::rev, 35, velocityUnits::pct);
    } else {
      TilterMotor.stop(brake);
    }

    // Intake Commands
    if (Controller1.ButtonR1.pressing()) {
      Intakes.spin(directionType::fwd, allSpeed, velocityUnits::pct);
    } else if (Controller1.ButtonR2.pressing()) {
      Intakes.spin(directionType::rev, 50, velocityUnits::pct);
    } else {
      Intakes.stop(brake);
    }

    // Bar Commands
    if (Controller1.ButtonUp.pressing()) {
      BarMotor.spin(directionType::fwd, allSpeed, velocityUnits::pct);
      if (Poten.angle(rotationUnits::raw) > 2975) {
        TilterMotor.spin(directionType::fwd, 35.25, pct);
      }
    } else if (Controller1.ButtonDown.pressing()) {
      BarMotor.spin(directionType::rev, allSpeed, velocityUnits::pct);
      while (!(BarMotor.rotation(rotationUnits::deg) > 0)) {
        TilterMotor.spin(directionType::rev, 45, pct); 
        if (Poten.angle(rotationUnits::raw) > 3500) {
          break;
        }
      }
      TilterMotor.stop();
    } else {
      BarMotor.stop(hold);
    }

    if (Controller1.ButtonA.pressing()) {
      deployStack();
    }

    if (Controller1.ButtonB.pressing()) {
      SmartDrive.driveFor(directionType::rev, 6, distanceUnits::in, false);
    }

    if (Controller1.ButtonRight.pressing()) {
      flipOut();
      // barMacro();
    }

    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(0, 0);
    Controller1.Screen.print(driveType.c_str());
    Controller1.Screen.newLine();
    Controller1.Screen.print(driveSpeed.c_str());

    // wait(20, msec);
}

void recordR(void) {
    uint8_t data[3000];
    Controller1.Screen.clearScreen();
    for(int count = 5; count > 0; count--)
    {
      Controller1.Screen.clearLine(1);
      Controller1.Screen.setCursor(1, 1);
      Controller1.Screen.print(count);
      vex::task::sleep(1000);
    }
    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(1, 1);
    Controller1.Screen.print("Recording");
    for (int i = 0; i < 3000; i+= 5) 
    {
        controls();

        data[i] = (uint8_t) (LeftSide.velocity(vex::velocityUnits::pct) + 100);
        data[i + 1] = (uint8_t) (RightSide.velocity(vex::velocityUnits::pct) + 100);
        data[i + 2] = (uint8_t) (Intakes.velocity(vex::velocityUnits::pct) + 100);
        data[i + 3] = (uint8_t) (BarMotor.velocity(vex::velocityUnits::pct) + 100);
        data[i + 4] = (uint8_t) (TilterMotor.velocity(vex::velocityUnits::pct) + 100);
        vex::task::sleep(25);
    }
    Brain.SDcard.savefile("rf.txt", data, 3000);
    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(1, 1);
    Controller1.Screen.print("Complete!");  
}

void recordB(void) {
    uint8_t data[3000];
    Controller1.Screen.clearScreen();
    for(int count = 5; count > 0; count--)
    {
      Controller1.Screen.clearLine(1);
      Controller1.Screen.setCursor(1, 1);
      Controller1.Screen.print(count);
      vex::task::sleep(1000);
    }
    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(1, 1);
    Controller1.Screen.print("Recording");
    for (int i = 0; i < 3000; i+= 5) 
    {
        controls();

        data[i] = (uint8_t) (LeftSide.velocity(vex::velocityUnits::pct) + 100);
        data[i + 1] = (uint8_t) (RightSide.velocity(vex::velocityUnits::pct) + 100);
        data[i + 2] = (uint8_t) (Intakes.velocity(vex::velocityUnits::pct) + 100);
        data[i + 3] = (uint8_t) (BarMotor.velocity(vex::velocityUnits::pct) + 100);
        data[i + 4] = (uint8_t) (TilterMotor.velocity(vex::velocityUnits::pct) + 100);
        vex::task::sleep(25);
    }
    Brain.SDcard.savefile("bf.txt", data, 3000);
    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(1, 1);
    Controller1.Screen.print("Complete!");  
}

void recordS(void) {
    uint8_t data[3000];
    Controller1.Screen.clearScreen();
    for(int count = 5; count > 0; count--)
    {
      Controller1.Screen.clearLine(1);
      Controller1.Screen.setCursor(1, 1);
      Controller1.Screen.print(count);
      vex::task::sleep(1000);
    }
    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(1, 1);
    Controller1.Screen.print("Recording");
    for (int i = 0; i < 3000; i+= 5) 
    {
        controls();

        data[i] = (uint8_t) (LeftSide.velocity(vex::velocityUnits::pct) + 100);
        data[i + 1] = (uint8_t) (RightSide.velocity(vex::velocityUnits::pct) + 100);
        data[i + 2] = (uint8_t) (Intakes.velocity(vex::velocityUnits::pct) + 100);
        data[i + 3] = (uint8_t) (BarMotor.velocity(vex::velocityUnits::pct) + 100);
        data[i + 4] = (uint8_t) (TilterMotor.velocity(vex::velocityUnits::pct) + 100);
        vex::task::sleep(100);
    }
    Brain.SDcard.savefile("st.txt", data, 3000);
    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(1, 1);
    Controller1.Screen.print("Complete!");  
}

void recordRS(void) {
      uint8_t data[3000];
    Controller1.Screen.clearScreen();
    for(int count = 5; count > 0; count--)
    {
      Controller1.Screen.clearLine(1);
      Controller1.Screen.setCursor(1, 1);
      Controller1.Screen.print(count);
      vex::task::sleep(1000);
    }
    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(1, 1);
    Controller1.Screen.print("Recording");
    for (int i = 0; i < 3000; i+= 5) 
    {
        controls();

        data[i] = (uint8_t) (LeftSide.velocity(vex::velocityUnits::pct) + 100);
        data[i + 1] = (uint8_t) (RightSide.velocity(vex::velocityUnits::pct) + 100);
        data[i + 2] = (uint8_t) (Intakes.velocity(vex::velocityUnits::pct) + 100);
        data[i + 3] = (uint8_t) (BarMotor.velocity(vex::velocityUnits::pct) + 100);
        data[i + 4] = (uint8_t) (TilterMotor.velocity(vex::velocityUnits::pct) + 100);
        vex::task::sleep(50);
    }
    Brain.SDcard.savefile("rs.txt", data, 3000);
    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(1, 1);
    Controller1.Screen.print("Complete!");  
}

void recordBS(void) {
      uint8_t data[3000];
    Controller1.Screen.clearScreen();
    for(int count = 5; count > 0; count--)
    {
      Controller1.Screen.clearLine(1);
      Controller1.Screen.setCursor(1, 1);
      Controller1.Screen.print(count);
      vex::task::sleep(1000);
    }
    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(1, 1);
    Controller1.Screen.print("Recording");
    for (int i = 0; i < 3000; i+= 5) 
    {
        controls();

        data[i] = (uint8_t) (LeftSide.velocity(vex::velocityUnits::pct) + 100);
        data[i + 1] = (uint8_t) (RightSide.velocity(vex::velocityUnits::pct) + 100);
        data[i + 2] = (uint8_t) (Intakes.velocity(vex::velocityUnits::pct) + 100);
        data[i + 3] = (uint8_t) (BarMotor.velocity(vex::velocityUnits::pct) + 100);
        data[i + 4] = (uint8_t) (TilterMotor.velocity(vex::velocityUnits::pct) + 100);
        vex::task::sleep(50);
    }
    Brain.SDcard.savefile("bs.txt", data, 3000);
    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(1, 1);
    Controller1.Screen.print("Complete!");  
}

// void record(void) {
//   uint8_t data[6000];
//   Controller1.Screen.clearScreen();
//   for (int count = 5; count > 0; count--) {
//     Controller1.Screen.clearLine(1);
//     Controller1.Screen.setCursor(1, 1);
//     Controller1.Screen.print(count);
//     vex::task::sleep(1000);
//     }
//   Controller1.Screen.clearScreen();
//   Controller1.Screen.setCursor(1, 1);
//   Controller1.Screen.print("Recording");
//   if (sk) {
//     for (int i = 0; i < 6000; i+= 5) {
//       controls();
//       data[i] = (uint8_t) (LeftSide.velocity(vex::velocityUnits::pct) + 100);
//       data[i + 1] = (uint8_t) (RightSide.velocity(vex::velocityUnits::pct) + 100);
//       data[i + 2] = (uint8_t) (BarMotor.velocity(vex::velocityUnits::pct) + 100);
//       data[i + 3] = (uint8_t) (TilterMotor.velocity(vex::velocityUnits::pct) + 100);
//       data[i + 4] = (uint8_t) (Intakes.velocity(vex::velocityUnits::pct) + 100);
//       vex::task::sleep(50);
//     }
//     Brain.SDcard.savefile("st.txt", data, 6000);
//   } else if (rf) {
//     for (int i = 0; i < 6000; i+= 5) {
//       controls();
//       data[i] = (uint8_t) (LeftSide.velocity(vex::velocityUnits::pct) + 100);
//       data[i + 1] = (uint8_t) (RightSide.velocity(vex::velocityUnits::pct) + 100);
//       data[i + 2] = (uint8_t) (BarMotor.velocity(vex::velocityUnits::pct) + 100);
//       data[i + 3] = (uint8_t) (TilterMotor.velocity(vex::velocityUnits::pct) + 100);
//       data[i + 4] = (uint8_t) (Intakes.velocity(vex::velocityUnits::pct) + 100);
//       vex::task::sleep(25/2);
//     }
//     Controller1.Screen.clearScreen();
//     Controller1.Screen.setCursor(1, 1);
//     Controller1.Screen.print("SAVING");
//     Brain.SDcard.savefile("rf.txt", data, 6000);
//   } else if (bf) {
//     for (int i = 0; i < 6000; i+= 5) {
//       controls();
//       data[i] = (uint8_t) (LeftSide.velocity(vex::velocityUnits::pct) + 100);
//       data[i + 1] = (uint8_t) (RightSide.velocity(vex::velocityUnits::pct) + 100);
//       data[i + 2] = (uint8_t) (BarMotor.velocity(vex::velocityUnits::pct) + 100);
//       data[i + 3] = (uint8_t) (TilterMotor.velocity(vex::velocityUnits::pct) + 100);
//       data[i + 4] = (uint8_t) (Intakes.velocity(vex::velocityUnits::pct) + 100);
//       vex::task::sleep(25/2);
//     }
//     Brain.SDcard.savefile("bf.txt", data, 6000);
//   } else if (rs) {
//     for (int i = 0; i < 6000; i+= 5) {
//       controls();
//       data[i] = (uint8_t) (LeftSide.velocity(vex::velocityUnits::pct) + 100);
//       data[i + 1] = (uint8_t) (RightSide.velocity(vex::velocityUnits::pct) + 100);
//       data[i + 2] = (uint8_t) (BarMotor.velocity(vex::velocityUnits::pct) + 100);
//       data[i + 3] = (uint8_t) (TilterMotor.velocity(vex::velocityUnits::pct) + 100);
//       data[i + 4] = (uint8_t) (Intakes.velocity(vex::velocityUnits::pct) + 100);
//       vex::task::sleep(17);
//     }
//     Brain.SDcard.savefile("rs.txt", data, 6000);
//   } else if (bs) {
//     for (int i = 0; i < 6000; i+= 5) {
//       controls();
//       data[i] = (uint8_t) (LeftSide.velocity(vex::velocityUnits::pct) + 100);
//       data[i + 1] = (uint8_t) (RightSide.velocity(vex::velocityUnits::pct) + 100);
//       data[i + 2] = (uint8_t) (BarMotor.velocity(vex::velocityUnits::pct) + 100);
//       data[i + 3] = (uint8_t) (TilterMotor.velocity(vex::velocityUnits::pct) + 100);
//       data[i + 4] = (uint8_t) (Intakes.velocity(vex::velocityUnits::pct) + 100);
//       vex::task::sleep(17);
//     }
//     Brain.SDcard.savefile("bs.txt", data, 6000);
//   }
// Controller1.Screen.clearScreen();
// Controller1.Screen.setCursor(1, 1);
// Controller1.Screen.print("Complete!");
// }  

// void playR(void) {
//   uint8_t buf[6000];
//   Brain.SDcard.loadfile("rt.txt", buf, 6000);
//   for (int i = 0; i < 6000; i+= 5) {    
//     LeftFrontMotor.spin(vex::directionType::fwd, buf[i] - 100, vex::velocityUnits::pct);
//     LeftRearMotor.spin(vex::directionType::fwd, buf[i] - 100, vex::velocityUnits::pct);
//     RightFrontMotor.spin(vex::directionType::fwd, buf[i + 1] - 100, vex::velocityUnits::pct);
//     RightRearMotor.spin(vex::directionType::fwd, buf[i + 1] - 100, vex::velocityUnits::pct);
//     BarMotor.spin(vex::directionType::fwd, buf[i + 2] - 100, vex::velocityUnits::pct);
//     TilterMotor.spin(vex::directionType::fwd, buf[i + 3] - 100, vex::velocityUnits::pct);
//     LeftIntakeMotor.spin(vex::directionType::fwd, buf[i + 4] - 100, vex::velocityUnits::pct);
//     RightIntakeMotor.spin(vex::directionType::fwd, buf[i + 4] - 100, vex::velocityUnits::pct);
//     vex::task::sleep(25/2);
//   }
// }

// void playB(void) {
//   uint8_t buf[6000];
//   Brain.SDcard.loadfile("bt.txt", buf, 6000);
//   for (int i = 0; i < 6000; i+= 5) {    
//     LeftFrontMotor.spin(vex::directionType::fwd, buf[i] - 100, vex::velocityUnits::pct);
//     LeftRearMotor.spin(vex::directionType::fwd, buf[i] - 100, vex::velocityUnits::pct);
//     RightFrontMotor.spin(vex::directionType::fwd, buf[i + 1] - 100, vex::velocityUnits::pct);
//     RightRearMotor.spin(vex::directionType::fwd, buf[i + 1] - 100, vex::velocityUnits::pct);
//     BarMotor.spin(vex::directionType::fwd, buf[i + 2] - 100, vex::velocityUnits::pct);
//     TilterMotor.spin(vex::directionType::fwd, buf[i + 3] - 100, vex::velocityUnits::pct);
//     LeftIntakeMotor.spin(vex::directionType::fwd, buf[i + 4] - 100, vex::velocityUnits::pct);
//     RightIntakeMotor.spin(vex::directionType::fwd, buf[i + 4] - 100, vex::velocityUnits::pct);
//     vex::task::sleep(25/2);
//   }
// }

// void playS(void) {
//   uint8_t buf[6000];
//   Brain.SDcard.loadfile("st.txt", buf, 6000);
//   for (int i = 0; i < 6000; i+= 5) {    
//     LeftFrontMotor.spin(vex::directionType::fwd, buf[i] - 100, vex::velocityUnits::pct);
//     LeftRearMotor.spin(vex::directionType::fwd, buf[i] - 100, vex::velocityUnits::pct);
//     RightFrontMotor.spin(vex::directionType::fwd, buf[i + 1] - 100, vex::velocityUnits::pct);
//     RightRearMotor.spin(vex::directionType::fwd, buf[i + 1] - 100, vex::velocityUnits::pct);
//     BarMotor.spin(vex::directionType::fwd, buf[i + 2] - 100, vex::velocityUnits::pct);
//     TilterMotor.spin(vex::directionType::fwd, buf[i + 3] - 100, vex::velocityUnits::pct);
//     LeftIntakeMotor.spin(vex::directionType::fwd, buf[i + 4] - 100, vex::velocityUnits::pct);
//     RightIntakeMotor.spin(vex::directionType::fwd, buf[i + 4] - 100, vex::velocityUnits::pct);
//     vex::task::sleep(50);
//   }
// }

// void playRS(void) {
//   uint8_t buf[6000];
//   Brain.SDcard.loadfile("rs.txt", buf, 6000);
//   for (int i = 0; i < 6000; i+= 5) {    
//     LeftFrontMotor.spin(vex::directionType::fwd, buf[i] - 100, vex::velocityUnits::pct);
//     LeftRearMotor.spin(vex::directionType::fwd, buf[i] - 100, vex::velocityUnits::pct);
//     RightFrontMotor.spin(vex::directionType::fwd, buf[i + 1] - 100, vex::velocityUnits::pct);
//     RightRearMotor.spin(vex::directionType::fwd, buf[i + 1] - 100, vex::velocityUnits::pct);
//     BarMotor.spin(vex::directionType::fwd, buf[i + 2] - 100, vex::velocityUnits::pct);
//     TilterMotor.spin(vex::directionType::fwd, buf[i + 3] - 100, vex::velocityUnits::pct);
//     LeftIntakeMotor.spin(vex::directionType::fwd, buf[i + 4] - 100, vex::velocityUnits::pct);
//     RightIntakeMotor.spin(vex::directionType::fwd, buf[i + 4] - 100, vex::velocityUnits::pct);
//     vex::task::sleep(17);
//   }
// }

// void playBS(void) {
//   uint8_t buf[6000];
//   Brain.SDcard.loadfile("bs.txt", buf, 6000);
//   for (int i = 0; i < 6000; i+= 5) {    
//     LeftFrontMotor.spin(vex::directionType::fwd, buf[i] - 100, vex::velocityUnits::pct);
//     LeftRearMotor.spin(vex::directionType::fwd, buf[i] - 100, vex::velocityUnits::pct);
//     RightFrontMotor.spin(vex::directionType::fwd, buf[i + 1] - 100, vex::velocityUnits::pct);
//     RightRearMotor.spin(vex::directionType::fwd, buf[i + 1] - 100, vex::velocityUnits::pct);
//     BarMotor.spin(vex::directionType::fwd, buf[i + 2] - 100, vex::velocityUnits::pct);
//     TilterMotor.spin(vex::directionType::fwd, buf[i + 3] - 100, vex::velocityUnits::pct);
//     LeftIntakeMotor.spin(vex::directionType::fwd, buf[i + 4] - 100, vex::velocityUnits::pct);
//     RightIntakeMotor.spin(vex::directionType::fwd, buf[i + 4] - 100, vex::velocityUnits::pct);
//     vex::task::sleep(17);
//   }
// }

void play(void) {
  uint8_t buf[3000];
  if (rf) {
    Brain.SDcard.loadfile("rf.txt", buf, 3000);
  } else if (bf) {
    Brain.SDcard.loadfile("bf.txt", buf, 3000);
  } else if (rs) {
    Brain.SDcard.loadfile("rs.txt", buf, 3000);
  } else if (bs) {
    Brain.SDcard.loadfile("bs.txt", buf, 3000);
  } else if (sk) {
    Brain.SDcard.loadfile("st.txt", buf, 3000);
  }
    
  for (int i = 0; i < 3000; i+= 5) {    
    LeftSide.spin(vex::directionType::fwd, buf[i] - 100, vex::velocityUnits::pct);
    RightSide.spin(vex::directionType::fwd, buf[i + 1] - 100, vex::velocityUnits::pct);
    Intakes.spin(vex::directionType::fwd, buf[i + 2] - 100, vex::velocityUnits::pct);
    BarMotor.spin(vex::directionType::fwd, buf[i + 3] - 100, vex::velocityUnits::pct);
    TilterMotor.spin(vex::directionType::fwd, buf[i + 4] - 100, vex::velocityUnits::pct);
    if (rf || bf) {
      vex::task::sleep(25);
    } else if (rs || bs) {
      vex::task::sleep(50);
    } else if (sk) {
      vex::task::sleep(100);
    }
  }
}

void autonomous(void) {
  LeftSide.setStopping(hold);
  RightSide.setStopping(hold);
  Intakes.setStopping(coast);
  BarMotor.setStopping(brake);
  TilterMotor.setStopping(coast);
  Brain.Screen.setFont(prop30);
  Brain.Screen.setPenColor(white);
  if (pl) {
    if (rf) {
      Brain.Screen.clearScreen();
      Brain.Screen.print("Playing Red Auton");
      play();
    } else if (bf) {
      Brain.Screen.clearScreen();
      Brain.Screen.print("Playing Blue Auton");
      play();
    } else if (rs) {
      Brain.Screen.clearScreen();
      Brain.Screen.print("Playing Red Slow Auton");
      play();
    } else if (bs) {
      Brain.Screen.clearScreen();
      Brain.Screen.print("Playing Blue Slow Auton");
      play();
    } else if (sk) {
      Brain.Screen.clearScreen();
      Brain.Screen.print("Playing Skills Auton");
      play();
    } else {
      Brain.Screen.clearScreen();
      Brain.Screen.print("No Auton Running");
    }
  }
}

void usercontrol(void) {
  LeftSide.setStopping(coast);
  RightSide.setStopping(coast);
  Intakes.setStopping(coast);
  BarMotor.setStopping(hold);
  TilterMotor.setStopping(brake);
  BarMotor.resetRotation();
  Controller1.ButtonY.pressed(spdToggle);
  Controller1.ButtonX.pressed(driveSelect);
  Controller1.ButtonLeft.pressed(fadeBack);
  if (rec) {
    if (rf) {
      Brain.Screen.clearScreen();
      Brain.Screen.print("Recording Red Side");
      recordR();
    } else if (bf) {
      Brain.Screen.clearScreen();
      Brain.Screen.print("Recording Blue Side");
      recordB();
    } else if (rs) {
      Brain.Screen.clearScreen();
      Brain.Screen.print("Recording Red Side Slow");
      recordRS();
    } else if (bs) {
      Brain.Screen.clearScreen();
      Brain.Screen.print("Recording Blue Side Slow");
      recordBS();
    } else if (sk) {
      Brain.Screen.clearScreen();
      Brain.Screen.print("Recording Skills Auton");
      recordS();
    } else {
      Brain.Screen.clearScreen();
      Brain.Screen.print("No Recorder Running");
    }
  } else {
    while (1) {
      controls();
    }
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
