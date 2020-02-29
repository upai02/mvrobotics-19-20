#ifndef CONFIG_H
#define CONFIG_H
#include "config.h"
#endif

class AUTON {
  private:

  public:
  double degreesPerFoot;
  double degreesTo360;
  int auton;

  AUTON();
  void askPos();
  void initialize();
  void turnLeft(double deg);
  void turnRight(double deg);
  void turn(double deg);
  void drive(double inches, double intRot);
  void drivePID(double target/*, int intake, bool reset*/); 
  void moveForwards(double feet, double speed);
  void moveBackwards(double feet, double speed);
};

AUTON::AUTON() {
  auton = -1;
  degreesPerFoot = 340.0;
  degreesTo360 = degreesPerFoot*4;
}

