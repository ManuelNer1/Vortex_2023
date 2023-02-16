/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Vortex                                                    */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Script                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include <string>

using namespace vex;
using namespace std;
#define PI (3.141592653589793)

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  Indexer.close();

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*---------------------------------------------------------------------------*/

// Declare functions for autonomous test

// Function to move fordward to an specific distance
void move_forward(double vel){
  LeftMotors.spin(directionType::fwd, vel, velocityUnits::pct);
  RightMotors.spin(directionType::fwd, vel, velocityUnits::pct);
}

// Function to move backward to an specific distance
void move_backward(double vel){
  LeftMotors.spin(directionType::rev, vel, velocityUnits::pct);
  RightMotors.spin(directionType::rev, vel, velocityUnits::pct);
}

// Function to stop every single motor
void stop_everything(){
  LeftMotors.stop();
  RightMotors.stop();
}

// Function to turn left or right with an specific angle
void just_turn(string dir, double dg, double vel, bool waitForCompletion=true){
  LeftMotors.resetRotation();
  RightMotors.resetRotation();
  if (dir == "rigth") {
    LeftMotors.spinFor(directionType::fwd, dg, rotationUnits::deg, vel, velocityUnits::pct, waitForCompletion);
  }
  else if (dir == "left") {
    LeftMotors.spinFor(directionType::fwd, dg, rotationUnits::deg, vel, velocityUnits::pct, waitForCompletion);
  }
}

// Shoot the discs with 95% of power
void shoot(double FLYWHEEL_VEL, double INDEXER_BACK, double INDEXER_GO, int throws){
  Flywheel.spin(directionType::fwd, FLYWHEEL_VEL, percentUnits::pct);
  wait(3.5, timeUnits::sec);
  for(int i=0; i<throws; i++){
    Indexer.open();
    wait(INDEXER_BACK, msec);
    Indexer.close();
    wait(INDEXER_GO, msec);
  }
}

void autonomous(void) {
  // Constants
  // double FLYWHEEL_VEL = 95;
  // double INDEXER_GO = 2000;
  // double INDEXER_BACK = 200; 

  // Insert autonomous user code here.
  move_backward(50);
  wait(500, msec);
  // Turn the intake
  move_forward(60);
  wait(500, msec);
  // just_turn("left", 10, 60);
  
  // shoot(FLYWHEEL_VEL, INDEXER_BACK, INDEXER_GO, 2);
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // User control code here, inside the loop
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.

    // ........................................................................

    wait(20, msec); // Sleep the task for a short amount of time to prevent 
                    // wasted resources.
  }
}

// Main for set up the competition functions and callbacks.
int main() {
  // Set up callbacks for autonomous and driver control periods.
  // Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
