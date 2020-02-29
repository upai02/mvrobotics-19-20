#define USE_MATH_DEFINES

#include <stdio.h>
#include <stdlib.h>
#include <math.h>	
#include <tgmath.h>
#include <string.h>
#include <cmath>

#include "v5.h"
#include "v5_vcs.h"

using namespace vex;
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

#ifndef COMPETITION_DEFINED
#define COMPETITION_DEFINED
competition Competition;
#endif