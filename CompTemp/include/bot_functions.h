#ifndef CONFIG_H
#define CONFIG_H
#include "config.h"
#endif

class ROBOT {
  private: 

  public:
  double driveSpeedFactor;
  int mode;
  std::string driveType;
  std::string driveSpeed;
  int barPos;

  ROBOT();
  void delay(int d);
  void rumble();
  void stop(int type);
  void tank(double deadzone);
  void arcade(double deadzone);
  void hybrid(double deadzone);
  double slew(double target, vex::motor_group iSide);
  void tankExpSlew();
  void arcadeExpSlew();
  void hybridExpSlew();
  void timeOut(double time);
  void resetDrive();
  void reset();
};

ROBOT::ROBOT() {
  driveSpeedFactor = 1.25;
  mode = 2;
  driveType = "Hybrid Controls";
  driveSpeed = "Drive Speed Normal";
  barPos = 0;
}

void ROBOT::delay(int d) {
  vex::task::sleep(d);
}

void ROBOT::rumble() {
  Controller1.rumble(".");
}

void ROBOT::stop(int type = 0) {
  if (type == 0) {
    LeftSide.stop(coast);
    RightSide.stop(coast);
  } else if (type == 1) {
    LeftSide.stop(brake);
    RightSide.stop(brake);
  } else if (type == 2) {
    LeftSide.stop(hold);
    RightSide.stop(hold);
  }
}

double ROBOT::slew(double target, vex::motor_group iSide) {
  int acc_rate = 15;
  int dec_rate = 15;
  double speed = 0;

  if (target != iSide.velocity(pct)) {
    if (iSide.velocity(pct) < target) {
      speed = iSide.velocity(pct) + acc_rate;
      if (iSide.velocity(pct) > target) {
        speed = target;
      }
    } else if (iSide.velocity(pct) > target) {
      speed = iSide.velocity(pct) - dec_rate;
      if (iSide.velocity(pct) < target) {
        speed = target;
      }
    }
  }
  return speed;
}

void ROBOT::tank(double deadzone) {
  LeftSide.spin(vex::directionType::fwd, ((Controller1.Axis3.value() > deadzone || Controller1.Axis3.value() < -deadzone) ? Controller1.Axis3.value() : 0)/driveSpeedFactor, vex::velocityUnits::pct);
  RightSide.spin(vex::directionType::fwd, ((Controller1.Axis2.value() > deadzone || Controller1.Axis2.value() < -deadzone) ? Controller1.Axis2.value() : 0)/driveSpeedFactor, vex::velocityUnits::pct);
}

void ROBOT::arcade(double deadzone) {
  LeftSide.spin(vex::directionType::fwd, (((Controller1.Axis3.value() > deadzone || Controller1.Axis3.value() < -deadzone) ? Controller1.Axis3.value() : 0) + ((Controller1.Axis1.value() > deadzone || Controller1.Axis1.value() < -deadzone) ? Controller1.Axis1.value() : 0))/driveSpeedFactor, vex::velocityUnits::pct);
  RightSide.spin(vex::directionType::fwd, (((Controller1.Axis3.value() > deadzone || Controller1.Axis3.value() < -deadzone) ? Controller1.Axis3.value() : 0) - ((Controller1.Axis1.value() > deadzone || Controller1.Axis1.value() < -deadzone) ? Controller1.Axis1.value() : 0))/driveSpeedFactor, vex::velocityUnits::pct);
}

void ROBOT::hybrid(double deadzone) {
  LeftSide.spin(vex::directionType::fwd, (((Controller1.Axis3.value() > deadzone || Controller1.Axis3.value() < -deadzone) ? Controller1.Axis3.value() : 0) - ((Controller1.Axis2.value() > deadzone || Controller1.Axis2.value() < -deadzone) ? Controller1.Axis2.value() : 0))/driveSpeedFactor, vex::velocityUnits::pct);
  RightSide.spin(vex::directionType::fwd, (((Controller1.Axis3.value() > deadzone || Controller1.Axis3.value() < -deadzone) ? Controller1.Axis3.value() : 0) + ((Controller1.Axis2.value() > deadzone || Controller1.Axis2.value() < -deadzone) ? Controller1.Axis2.value() : 0))/driveSpeedFactor, vex::velocityUnits::pct);
}

void ROBOT::tankExpSlew() {
  LeftSide.spin(vex::directionType::fwd, slew(0.0001*pow(Controller1.Axis3.value(), 3), LeftSide)/driveSpeedFactor, vex::velocityUnits::pct);
  RightSide.spin(vex::directionType::fwd, slew(0.0001*pow(Controller1.Axis2.value(), 3), RightSide)/driveSpeedFactor, vex::velocityUnits::pct);
}

void ROBOT::arcadeExpSlew() {
  LeftSide.spin(vex::directionType::fwd, slew(0.0001*pow(Controller1.Axis3.value() + Controller1.Axis1.value(), 3), LeftSide)/driveSpeedFactor, vex::velocityUnits::pct);
  RightSide.spin(vex::directionType::fwd, slew(0.0001*pow(Controller1.Axis3.value() - Controller1.Axis1.value(), 3), RightSide)/driveSpeedFactor, vex::velocityUnits::pct);
}

void ROBOT::hybridExpSlew() {
  LeftSide.spin(vex::directionType::fwd, slew(0.0001*pow(Controller1.Axis3.value() - Controller1.Axis2.value(), 3), LeftSide)/driveSpeedFactor, vex::velocityUnits::pct);
  RightSide.spin(vex::directionType::fwd, slew(0.0001*pow(Controller1.Axis3.value() + Controller1.Axis2.value(), 3), RightSide)/driveSpeedFactor, vex::velocityUnits::pct);
}

void ROBOT::timeOut(double time) {
  LeftSide.setTimeout(time, vex::timeUnits::sec);
	RightSide.setTimeout(time, vex::timeUnits::sec);
}

void ROBOT::resetDrive() {
  LeftSide.setRotation(0, vex::rotationUnits::deg);
	RightSide.setRotation(0, vex::rotationUnits::deg);
}

void ROBOT::reset() {
  resetDrive();
  BarMotor.setRotation(0, deg);
  Inertial.calibrate(2000);
  vex::task::sleep(2000);
}
