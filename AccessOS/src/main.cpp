// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Drivetrain           drivetrain    1, 3, 2, 4, 5   
// ---- END VEXCODE CONFIGURED DEVICES ----
/*------------------------------------------------------------------------------------------------------
[MAIN] Sample Main Program > A sample main.cpp file for ACCESS_OS
------------------------------------------------------------------------------------------------------*/

/*++++++++++++++++++++++++++++++++++++++++++| INFOMATION |++++++++++++++++++++++++++++++++++++++++++++++
This is a sample main.cpp file for ACCESS_OS (Sample meaning not complete, a templete/base)

The robot specs are as follows:
2 motor omniwheel drive

A small batch of example programs are provided, but you'll need to modify the code to fit your robot.

Changelog, documentation, and more information can be found at the ACCESS_OS website.
Link to website: https://dev.azure.com/roboVEX/_git/ACCESS_OS
------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------
[GLOBAL] Includes > Include needed external libraries and files
------------------------------------------------------------------------------------------------------*/
//Connected devices file
#ifndef CONFIG_H
#define CONFIG_H
#include "robot-config.h"
#endif
//Drive system class
#ifndef ROBOT_C
#define ROBOT_C
#include "robot-base.cpp"
#endif
//ACCESS_OS file
#ifndef ACCESS_OS_C
#define ACCESS_OS_C
#include "ACCESS_OS.cpp"
#endif

ROBOT_BASE ROBOT;
ACCESS_OS OS;
#include <cmath>

/*------------------------------------------------------------------------------------------------------
[GLOBAL] Variables > Global variables for functions
------------------------------------------------------------------------------------------------------*/

//Movement const variables + calculations
const int circumference = (wheelDiameter * M_PI);	//M_PI is from the math library
const int oneTile = (360 * tileSize) / circumference;	//Calculate how many degrees it takes to move oneTile
const int rotateTile = oneTile / 2;	//If both motors spin at opposite directions at half of oneTile, it will turn 90 degrees

//For checking status
int tempStatus = 0;

/*------------------------------------------------------------------------------------------------------
[PROGRAM]

Functions are here. Read the comments to find out what each does.

------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------
[OPTIMIZING] autonRotateTile > Rotates left or right (Program for BLUE, it will auto-config for RED)
------------------------------------------------------------------------------------------------------*/
void autonRotateTile(float x, double speed) 
{
	//autonRotateTile v2019.5.14.17.20
	//Use x to rotate the robot by 90.
	//rotateTile needs to be calibrated and calculated to work correctly
	//Examples (for BLUE side): 
	//1. Rotate right 45 degrees:	autonRotateTile(x=.5, speed=100, delayTime=50);
	//2. Rotate right 90 degrees:	autonRotateTile(x=1, speed=100, delayTime=50);
	//3. Rotate left 180 degrees:	autonRotateTile(x=-2, speed=50, delayTime=50);

	ROBOT.timeOut(1);
	ROBOT.reset();

	if (OS.getValues(AUTON_COLOR) == RED) //If color is 1 (RED) flip the values 
	{
		x = -x;
	}
	x = x * rotateTile;

	LeftSide.rotateTo(x, vex::rotationUnits::deg, speed, vex::velocityUnits::pct, false);
	RightSide.rotateTo(x, vex::rotationUnits::deg, speed, vex::velocityUnits::pct);
}

/*------------------------------------------------------------------------------------------------------
[STABLE] turn > Turns the bot a specified number of degrees (+ for right, - for left)
------------------------------------------------------------------------------------------------------*/
void turn(double degrees)
{
  double target = degrees;
  double error = target - Inertial.rotation();
  double kP = .5;
  while (std::abs(error) > 1) 
  {
    error = target - Inertial.rotation();
    double percent = kP * error + 10;
    LeftSide.spin(vex::directionType::fwd, percent, vex::pct);
	  RightSide.spin(vex::directionType::rev, percent, vex::pct);
    vex::task::sleep(20);
  }
  LeftSide.stop();
  RightSide.stop();
}

/*------------------------------------------------------------------------------------------------------
[STABLE] flipOut > Flips out tray on bot
------------------------------------------------------------------------------------------------------*/
void flipOut() 
{
  Tilter.setVelocity(100, vex::pct);
  Bar.setVelocity(100, vex::pct);
  Tilter.spinFor(vex::fwd, 425, vex::deg);
  wait(50, vex::msec);
  Tilter.spinFor(vex::directionType::rev, 425, vex::deg, false);
  Bar.spinFor(vex::fwd, 400, vex::deg);
  wait(1, vex::msec);
  Bar.spinFor(vex::directionType::rev, 400, vex::deg);
}

/*------------------------------------------------------------------------------------------------------
[STABLE] deployStack > Potentiometer P-loop for suto-stack
------------------------------------------------------------------------------------------------------*/
void deployStack() 
{
Tilter.setBrake(vex::brake);
  double error = 2000;
  double tilt_speed = 100;
  while(error > 100 && tilt_speed > 5) {
    error = std::abs(1470-Poten.angle(vex::rotationUnits::raw));
    tilt_speed = error/50;
    Tilter.spin(vex::fwd, tilt_speed, vex::pct);
  }
  // wait(1, sec);
  // Drivetrain.driveFor(directionType::rev, 6, inches);
  // while(Poten.value(rotationUnits::raw) < 3700) {
  //   Tilter.spin(directionType::rev, 100, pct);
  // }
}

/*------------------------------------------------------------------------------------------------------
[STABLE] driverScore > User scoring control systems
------------------------------------------------------------------------------------------------------*/
void driverScore()
{
	if (Controller.ButtonL1.pressing()) {
      Tilter.spin(vex::directionType::fwd, 35, vex::velocityUnits::pct);
    } else if (Controller.ButtonL2.pressing()) {
      Tilter.spin(vex::directionType::rev, 35, vex::velocityUnits::pct);
    } else {
      Tilter.stop(vex::brake);
    }

    // Intake Commands
    if (Controller.ButtonR1.pressing()) {
      Intakes.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);
    } else if (Controller.ButtonR2.pressing()) {
      Intakes.spin(vex::directionType::rev, 50, vex::velocityUnits::pct);
    } else {
      Intakes.stop(vex::coast);
    }

    // Bar Commands
    if (Controller.ButtonUp.pressing()) {
      Bar.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);
      // if (Poten.angle(vex::rotationUnits::raw) > 2600) {
      //   Tilter.spin(vex::directionType::fwd, 33, vex::pct);
      // }
    } else if (Controller.ButtonDown.pressing()) {
      Bar.spin(vex::directionType::rev, 100, vex::velocityUnits::pct);
      // Tilter.spin(vex::directionType::rev, 30, vex::pct);
    } else {
      Bar.stop(vex::hold);
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
    if (Controller.ButtonX.pressing()) {
      switch (OS.getValues(AUTON_DRIVE))
      {
		  case RC:
			  OS.setValues(AUTON_DRIVE, 0);
			  break;
		  case TANK:
			  OS.setValues(AUTON_DRIVE, 1);
			  break;
		  default:
			  break;
		  }
    }

    if (Controller.ButtonA.pressing()) {
      deployStack();
    }

    if (Controller.ButtonLeft.pressing()) {
      flipOut();
    }

    if (Controller.ButtonB.pressing()) {
      Drivetrain.driveFor(vex::directionType::rev, 6, vex::distanceUnits::in);
    }

    // wait(20, msec);
}

/*------------------------------------------------------------------------------------------------------
[STABLE] pre_auton > Set up any initialization stuff here before auton
------------------------------------------------------------------------------------------------------*/
void pre_auton(void)
{
	ROBOT.reset();		//Resets the motor's position
	OS.menuCONFIG();	//Calls ACCESS_OS's menuCONFIG, the main reason why you downloaded this
	OS.notificationHUD("Robot: OK");	//Sends message to driver
}

/*------------------------------------------------------------------------------------------------------
[OPTIMIZING] autonFrontRow > A.I. scores in the front row
------------------------------------------------------------------------------------------------------*/
void autonFrontRow() //BLUE SIDE - large zone
{
	//Add code for front row auton here
}

/*------------------------------------------------------------------------------------------------------
[OPTIMIZING] autonBackRow > Smooth moves by our A.I. back stage
------------------------------------------------------------------------------------------------------*/
void autonBackRow() //BLUE SIDE - small zone
{
  //Add code for back row auton here
}

/*------------------------------------------------------------------------------------------------------
[OPTIMIZING] autonSkills > May you be charmed by the A.I.
------------------------------------------------------------------------------------------------------*/
void autonSkills()
{
	//Add code for skills auton here
}

/*------------------------------------------------------------------------------------------------------
[STABLE] autonomous > The heart and soul of our A.I.
------------------------------------------------------------------------------------------------------*/
void autonomous(void)
{
	//If configuration[1] is 0 (Front row), 1 (Back row), or 2 (Skills), run the correct auton
	if (OS.getValues(AUTON_TYPE) == FRONT) {
		autonFrontRow();
	}
	else if (OS.getValues(AUTON_TYPE) == BACK) {
		autonBackRow();
	}
	else if (OS.getValues(AUTON_TYPE) == SKILLS) {
		autonSkills();
	}

	//Auton end. Prints auton end on the controller + vibrate controller
	OS.notificationHUD("Auton: DONE");
	ROBOT.rumble();
}

/*------------------------------------------------------------------------------------------------------
[STABLE] usercontrol > Driver control stuff here, the A.I. is in your hands
------------------------------------------------------------------------------------------------------*/
void usercontrol(void)
{
	Brain.Screen.clearScreen(vex::color::green); //Get the driver control ready with visual cues
	//User control code here, inside the loop
	while (1) {
		switch (OS.getValues(AUTON_DRIVE))
		{
		case RC:
			ROBOT.RCDrive(17.5);
			break;
		case TANK:
			ROBOT.tankDrive(17.5);
			break;
		default:
			break;
		}
		driverScore();
	}
}

/*------------------------------------------------------------------------------------------------------
[STABLE] main > System required loop, (Can't?) add stuff here
------------------------------------------------------------------------------------------------------*/
int main()
{
	//Run the pre-autonomous function.
	pre_auton();

	//Set up callbacks for autonomous and driver control periods.
	Competition.autonomous(autonomous);
	Competition.drivercontrol(usercontrol);

	//Prevent main from exiting with an infinite loop.
	while (1)
	{
		//Checks if current status changed. If it didn't, don't do anything. If it did, refresh statusHUD.
		if (tempStatus != OS.currStatus())
		{
      OS.statusHUD();
			tempStatus = OS.currStatus();
		}
		vex::task::sleep(175); //Sleep the task for a short amount of time to prevent wasted resources.
	}
}