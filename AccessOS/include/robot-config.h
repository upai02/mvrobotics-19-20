/*------------------------------------------------------------------------------------------------------
[CONFIG] robot-config > Defines connected devices.
Every file MUST have these lines of code on top:
//Connected devices file
#ifndef CONFIG_H
#define CONFIG_H
#include "robot-config.h"
#endif
------------------------------------------------------------------------------------------------------*/

/*++++++++++++++++++++++++++++++++++++++++++| INFOMATION |++++++++++++++++++++++++++++++++++++++++++++++
This is a sample robot-config.h file for ACCESS_OS (Sample meaning not complete, a templete/base)

The robot specs are as follows:
2 motor omniwheel drive

A small batch of example programs are provided, but you'll need to modify the code to fit your robot.

Changelog, documentation, and more information can be found at the ACCESS_OS website.
Link to website: https://dev.azure.com/roboVEX/_git/ACCESS_OS
------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------
[GLOBAL] Includes > Include needed external libraries and files
------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>		//Include math.h in order to gain access to math functions like PI.
#include <tgmath.h>
#include <string.h>

#include "v5.h"
#include "v5_vcs.h"

/*------------------------------------------------------------------------------------------------------
[GLOBAL] Devices > Connected devices and sensors
------------------------------------------------------------------------------------------------------*/
//Main compoments
vex::brain Brain;
vex::controller Controller = vex::controller();

//Sensors
vex::pot Poten = vex::pot(Brain.ThreeWirePort.H);
vex::inertial Gyro = vex::inertial(2);

//Drive motors, groups, and drivetrain
vex::motor RightFront = vex::motor(vex::PORT13, vex::gearSetting::ratio18_1, true);
vex::motor RightRear = vex::motor(vex::PORT20, vex::gearSetting::ratio18_1, true);
vex::motor LeftRear = vex::motor(vex::PORT19, vex::gearSetting::ratio18_1, false);
vex::motor LeftFront = vex::motor(vex::PORT18, vex::gearSetting::ratio18_1, false);
vex::motor_group LeftSide = vex::motor_group(LeftFront, LeftRear);
vex::motor_group RightSide = vex::motor_group(RightFront, RightRear);
vex::smartdrive Drivetrain = vex::smartdrive(LeftSide, RightSide, Gyro, 319.19, 355.59999999999997, 292.09999999999997, vex::mm, 1.0);

//Intake motors, group
vex::motor LeftIntake = vex::motor(vex::PORT11, vex::ratio18_1, true);
vex::motor RightIntake = vex::motor(vex::PORT12, vex::ratio18_1, false);
vex::motor_group Intakes = vex::motor_group(LeftIntake, RightIntake);

//Bar and tilter motors
vex::motor Tilter = vex::motor(vex::PORT17, vex::ratio36_1, true);
vex::motor Bar = vex::motor(vex::PORT14, vex::ratio36_1, false);

//Create object Compettition of the Competition class
#ifndef COMPETITION_DEFINED
#define COMPETITION_DEFINED
vex::competition Competition;
#endif
