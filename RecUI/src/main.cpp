#include "vex.h"

using namespace vex;
double driveSpeedFactor = 1;
std::string driveSpeed;
int mode = 0;
bool rec = false;
bool pl = false;
bool rf = false;
bool bf = false;
bool rs = false;
bool bs = false;
bool sk = false;
int allSpeed = 100;
int intake = 100;
std::string driveType;
int driveMode = 0;

// A global instance of competition
competition Competition;
motor RightFrontMotor = motor(PORT13, ratio18_1, true);
motor RightRearMotor = motor(PORT12, ratio18_1, true);
motor LeftRearMotor = motor(PORT19, ratio18_1, false);
motor LeftFrontMotor = motor(PORT18, ratio18_1, false);
motor LeftIntakeMotor = motor(PORT20, ratio18_1, false);
motor RightIntakeMotor = motor(PORT11, ratio18_1, true);
motor TilterMotor = motor(PORT17, ratio36_1, false);
motor BarMotor = motor(PORT14, ratio36_1, true);
//pot Poten = pot(Brain.ThreeWirePort.A);
motor_group LeftSide = motor_group(LeftFrontMotor, LeftRearMotor);
motor_group RightSide = motor_group(RightFrontMotor, RightRearMotor);
motor_group Intakes = motor_group(LeftIntakeMotor, RightIntakeMotor);
drivetrain Drivetrain = drivetrain(LeftSide, RightSide, 319.19, 355.59999999999997, 292.09999999999997, mm, 1.0);
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
  vexcodeInit();
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
      vex::task::sleep(2000);
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

/*void deployStack() {
  TilterMotor.setBrake(brake);
  int error = 2000;
  int tilt_speed = 100;
  while(error > 100 && tilt_speed > 5) {
    error = 3500-Poten.value(rotationUnits::raw);
    tilt_speed = error * 100/1700;
    TilterMotor.spin(fwd, tilt_speed, pct);
  }
}*/

void controls() {
  LeftSide.spin(directionType::fwd, ((Controller1.Axis3.value()*abs(Controller1.Axis3.value())/100)/driveSpeedFactor), velocityUnits::pct);
  RightSide.spin(directionType::fwd, ((Controller1.Axis2.value()*abs(Controller1.Axis2.value())/100)/driveSpeedFactor), velocityUnits::pct);
    
  // Tilter Commands
  if (Controller1.ButtonL1.pressing()) {
    TilterMotor.spin(directionType::fwd, 25, velocityUnits::pct);
  } else if (Controller1.ButtonL2.pressing()) {
    TilterMotor.spin(directionType::rev, 25, velocityUnits::pct);
  } else {
    TilterMotor.stop(brake);
  }

  // Intake Commands
  if (Controller1.ButtonR1.pressing()) {
    Intakes.spin(directionType::fwd, intake, velocityUnits::pct);
  } else if (Controller1.ButtonR2.pressing()) {
    Intakes.spin(directionType::rev, 60, velocityUnits::pct);
  } else {
    Intakes.stop(coast);
  }

  // Bar Commands
  if (Controller1.ButtonUp.pressing()) {
    BarMotor.spin(directionType::fwd, allSpeed, velocityUnits::pct);
    TilterMotor.spin(directionType::fwd, 25, pct);
  } else if (Controller1.ButtonDown.pressing()) {
    BarMotor.spin(directionType::rev, allSpeed, velocityUnits::pct);
    TilterMotor.spin(directionType::rev, 25, pct);
  } else {
    BarMotor.stop(hold);
    TilterMotor.stop(coast);
  }

  // if (Controller1.ButtonLeft.pressing()) {
  //   flipOut();
  // }
}

void recordR(void) {
  uint8_t data[6000];
  Controller1.Screen.clearScreen();
  for (int count = 5; count > 0; count--) {
    Controller1.Screen.clearLine(1);
    Controller1.Screen.setCursor(1, 1);
    Controller1.Screen.print(count);
    vex::task::sleep(1000);
    }
  Controller1.Screen.clearScreen();
  Controller1.Screen.setCursor(1, 1);
  Controller1.Screen.print("Recording");
  for (int i = 0; i < 6000; i+= 5) {
    controls();
    data[i] = (uint8_t) (LeftSide.velocity(vex::velocityUnits::pct) + 100);
    data[i + 1] = (uint8_t) (RightSide.velocity(vex::velocityUnits::pct) + 100);
    data[i + 2] = (uint8_t) (BarMotor.velocity(vex::velocityUnits::pct) + 100);
    data[i + 3] = (uint8_t) (TilterMotor.velocity(vex::velocityUnits::pct) + 100);
    data[i + 4] = (uint8_t) (Intakes.velocity(vex::velocityUnits::pct) + 100);
    vex::task::sleep(25/2);
  }
  Brain.Screen.clearScreen();
  Brain.Screen.setFont(prop30);
  if (Brain.SDcard.isInserted()) {
    Brain.Screen.print("SAVING");
    Brain.SDcard.savefile("rf.txt", data, 6000);
  }
  Controller1.Screen.clearScreen();
  Controller1.Screen.setCursor(1, 1);
  Controller1.Screen.print("Complete!"); 
}

void recordB(void) {
  uint8_t data[6000];
  Controller1.Screen.clearScreen();
  for (int count = 5; count > 0; count--) {
    Controller1.Screen.clearLine(1);
    Controller1.Screen.setCursor(1, 1);
    Controller1.Screen.print(count);
    vex::task::sleep(1000);
    }
  Controller1.Screen.clearScreen();
  Controller1.Screen.setCursor(1, 1);
  Controller1.Screen.print("Recording");
  for (int i = 0; i < 6000; i+= 5) {
    controls();
    data[i] = (uint8_t) (LeftSide.velocity(vex::velocityUnits::pct) + 100);
    data[i + 1] = (uint8_t) (RightSide.velocity(vex::velocityUnits::pct) + 100);
    data[i + 2] = (uint8_t) (BarMotor.velocity(vex::velocityUnits::pct) + 100);
    data[i + 3] = (uint8_t) (TilterMotor.velocity(vex::velocityUnits::pct) + 100);
    data[i + 4] = (uint8_t) (Intakes.velocity(vex::velocityUnits::pct) + 100);
    vex::task::sleep(25/2);
  }
  Brain.SDcard.savefile("bf.txt", data, 6000);
  Controller1.Screen.clearScreen();
  Controller1.Screen.setCursor(1, 1);
  Controller1.Screen.print("Complete!"); 
}

void recordS(void) {
  uint8_t data[6000];
  Controller1.Screen.clearScreen();
  for (int count = 5; count > 0; count--) {
    Controller1.Screen.clearLine(1);
    Controller1.Screen.setCursor(1, 1);
    Controller1.Screen.print(count);
    vex::task::sleep(1000);
    }
  Controller1.Screen.clearScreen();
  Controller1.Screen.setCursor(1, 1);
  Controller1.Screen.print("Recording");
  for (int i = 0; i < 6000; i+= 5) {
    controls();
    data[i] = (uint8_t) (LeftSide.velocity(vex::velocityUnits::pct) + 100);
    data[i + 1] = (uint8_t) (RightSide.velocity(vex::velocityUnits::pct) + 100);
    data[i + 2] = (uint8_t) (BarMotor.velocity(vex::velocityUnits::pct) + 100);
    data[i + 3] = (uint8_t) (TilterMotor.velocity(vex::velocityUnits::pct) + 100);
    data[i + 4] = (uint8_t) (Intakes.velocity(vex::velocityUnits::pct) + 100);
    vex::task::sleep(50);
  }
  Brain.SDcard.savefile("st.txt", data, 6000);
  Controller1.Screen.clearScreen();
  Controller1.Screen.setCursor(1, 1);
  Controller1.Screen.print("Complete!"); 
}

void recordRS(void) {
  uint8_t data[6000];
  Controller1.Screen.clearScreen();
  for (int count = 5; count > 0; count--) {
    Controller1.Screen.clearLine(1);
    Controller1.Screen.setCursor(1, 1);
    Controller1.Screen.print(count);
    vex::task::sleep(1000);
    }
  Controller1.Screen.clearScreen();
  Controller1.Screen.setCursor(1, 1);
  Controller1.Screen.print("Recording");
  for (int i = 0; i < 6000; i+= 5) {
    controls();
    data[i] = (uint8_t) (LeftSide.velocity(vex::velocityUnits::pct) + 100);
    data[i + 1] = (uint8_t) (RightSide.velocity(vex::velocityUnits::pct) + 100);
    data[i + 2] = (uint8_t) (BarMotor.velocity(vex::velocityUnits::pct) + 100);
    data[i + 3] = (uint8_t) (TilterMotor.velocity(vex::velocityUnits::pct) + 100);
    data[i + 4] = (uint8_t) (Intakes.velocity(vex::velocityUnits::pct) + 100);
    vex::task::sleep(17);
  }
  Brain.SDcard.savefile("rs.txt", data, 6000);  
  Controller1.Screen.clearScreen();
  Controller1.Screen.setCursor(1, 1);
  Controller1.Screen.print("Complete!"); 
}

void recordBS(void) {
  uint8_t data[6000];
  Controller1.Screen.clearScreen();
  for (int count = 5; count > 0; count--) {
    Controller1.Screen.clearLine(1);
    Controller1.Screen.setCursor(1, 1);
    Controller1.Screen.print(count);
    vex::task::sleep(1000);
    }
  Controller1.Screen.clearScreen();
  Controller1.Screen.setCursor(1, 1);
  Controller1.Screen.print("Recording");
  for (int i = 0; i < 6000; i+= 5) {
    controls();
    data[i] = (uint8_t) (LeftSide.velocity(vex::velocityUnits::pct) + 100);
    data[i + 1] = (uint8_t) (RightSide.velocity(vex::velocityUnits::pct) + 100);
    data[i + 2] = (uint8_t) (BarMotor.velocity(vex::velocityUnits::pct) + 100);
    data[i + 3] = (uint8_t) (TilterMotor.velocity(vex::velocityUnits::pct) + 100);
    data[i + 4] = (uint8_t) (Intakes.velocity(vex::velocityUnits::pct) + 100);
    vex::task::sleep(17);
  }
  Brain.SDcard.savefile("bs.txt", data, 6000);
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
  uint8_t buf[6000];
  if (rf) {
    Brain.SDcard.loadfile("rf.txt", buf, 6000);
  } else if (bf) {
    Brain.SDcard.loadfile("bf.txt", buf, 6000);
  } else if (rs) {
    Brain.SDcard.loadfile("rs.txt", buf, 6000);
  } else if (bs) {
    Brain.SDcard.loadfile("bs.txt", buf, 6000);
  } else if (sk) {
    Brain.SDcard.loadfile("st.txt", buf, 6000);
  }
    
  for (int i = 0; i < 6000; i+= 5) {    
    LeftFrontMotor.spin(vex::directionType::fwd, buf[i] - 100, vex::velocityUnits::pct);
    LeftRearMotor.spin(vex::directionType::fwd, buf[i] - 100, vex::velocityUnits::pct);
    RightFrontMotor.spin(vex::directionType::fwd, buf[i + 1] - 100, vex::velocityUnits::pct);
    RightRearMotor.spin(vex::directionType::fwd, buf[i + 1] - 100, vex::velocityUnits::pct);
    BarMotor.spin(vex::directionType::fwd, buf[i + 2] - 100, vex::velocityUnits::pct);
    TilterMotor.spin(vex::directionType::fwd, buf[i + 3] - 100, vex::velocityUnits::pct);
    LeftIntakeMotor.spin(vex::directionType::fwd, buf[i + 4] - 100, vex::velocityUnits::pct);
    RightIntakeMotor.spin(vex::directionType::fwd, buf[i + 4] - 100, vex::velocityUnits::pct);
    if (rf || bf) {
      vex::task::sleep(25/2);
    } else if (rs || bs) {
      vex::task::sleep(17);
    } else if (sk) {
      vex::task::sleep(50);
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
  // if (rec) {
  //   if (rf) {
  //     Brain.Screen.clearScreen();
  //     Brain.Screen.print("Recording Red Side");
  //     record();
  //   } else if (bf) {
  //     Brain.Screen.clearScreen();
  //     Brain.Screen.print("Recording Blue Side");
  //     record();
  //   } else if (rs) {
  //     Brain.Screen.clearScreen();
  //     Brain.Screen.print("Recording Red Side Slow");
  //     record();
  //   } else if (bs) {
  //     Brain.Screen.clearScreen();
  //     Brain.Screen.print("Recording Blue Side Slow");
  //     record();
  //   } else if (sk) {
  //     Brain.Screen.clearScreen();
  //     Brain.Screen.print("Recording Skills Auton");
  //     record();
  //   } else {
  //     Brain.Screen.clearScreen();
  //     Brain.Screen.print("No Auton Running");
  //   }
  // } 
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
      Brain.Screen.print("No Auton Running");
    }
  } else {
    while (1) {
      // Drive Commands
      if (driveMode == 0) {
        LeftSide.spin(directionType::fwd, ((Controller1.Axis3.value()*abs(Controller1.Axis3.value())/100)/driveSpeedFactor), velocityUnits::pct);
        RightSide.spin(directionType::fwd, ((Controller1.Axis2.value()*abs(Controller1.Axis2.value())/100)/driveSpeedFactor), velocityUnits::pct);
      } else if (driveMode == 1) {
        LeftSide.spin(directionType::fwd, ((Controller1.Axis3.value()+Controller1.Axis1.value())/2)/driveSpeedFactor, velocityUnits::pct);
        RightSide.spin(directionType::fwd, ((Controller1.Axis3.value()-Controller1.Axis1.value())/2)/driveSpeedFactor, velocityUnits::pct);
      }
    
      // Tilter Commands
      if (Controller1.ButtonL1.pressing()) {
        TilterMotor.spin(directionType::fwd, 25, velocityUnits::pct);
      } else if (Controller1.ButtonL2.pressing()) {
        TilterMotor.spin(directionType::rev, 25, velocityUnits::pct);
      } else {
        TilterMotor.stop(brake);
      }

      // Intake Commands
      if (Controller1.ButtonR1.pressing()) {
        Intakes.spin(directionType::fwd, intake, velocityUnits::pct);
      } else if (Controller1.ButtonR2.pressing()) {
        Intakes.spin(directionType::rev, 60, velocityUnits::pct);
      } else {
        Intakes.stop(coast);
      }

      // Bar Commands
      if (Controller1.ButtonUp.pressing()) {
        BarMotor.spin(directionType::fwd, allSpeed, velocityUnits::pct);
        TilterMotor.spin(directionType::fwd, 25, pct);
      } else if (Controller1.ButtonDown.pressing()) {
        BarMotor.spin(directionType::rev, allSpeed, velocityUnits::pct);
        TilterMotor.spin(directionType::rev, 25, pct);
      } else {
        BarMotor.stop(hold);
        TilterMotor.stop(coast);
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
      // }

      //Drive Select
      if (Controller1.ButtonX.pressing()) {
        if (driveMode == 0) {
          mode = 1;
          driveType = "Split Arcade Controls";
        } else if (driveMode == 1) {
          mode = 0;
          driveType = "Tank Controls";
        }
      }

      // if (Controller1.ButtonLeft.pressing()) {
      //   flipOut();
      // }

      Controller1.Screen.clearScreen();
      Controller1.Screen.setCursor(0, 0);
      Controller1.Screen.newLine();
      Controller1.Screen.print(driveType.c_str());
      Controller1.Screen.newLine();
      // Controller1.Screen.print(driveSpeed.c_str());

      // wait(20, msec);
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
