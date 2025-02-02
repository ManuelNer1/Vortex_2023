/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       robot-config.cpp                                          */
/*    Author:       VORTEX Robotics                                           */
/*                  For more information contact A01706424@tec.mx             */
/*    Created:      18-feb-2023                                               */
/*    Description:  Default header for V5 projects                            */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

/************ Constants ****************/
int    DEADBAND       = 10;          //pct
double INDEXER_GO     = 460;         //pct 1500 -> 
double INDEXER_BACK   = 180;         //ms  200 -> 
double WAIT_UNTIL_LAUNCH = 100;      //ms
double FLYWHEEL_VEL   = 69;          //pct 100 -> 70 -> 65 -> 
double MAX_FLYWHEEL_VEL = 80;        //pct 
double INTAKE_VEL     = 85;          //pcd 70 -> 
double EXPANSOR_DEG   = 40;          //deg

/********* Devices definition **********/
// Brain screen
brain Brain;

// Drivetrain motors  
motor LeftFrontMotor = motor(PORT1, ratio18_1, false);
motor LeftMiddleMotor = motor(PORT2, ratio18_1, true);
motor LeftBackMotor = motor(PORT3, ratio18_1, false);

motor RightFrontMotor = motor(PORT4, ratio18_1, true);
motor RightMiddleMotor = motor(PORT5, ratio18_1, false);
motor RightBackMotor = motor(PORT6, ratio18_1, true);

motor_group LeftMotors = motor_group(LeftFrontMotor, LeftMiddleMotor, LeftBackMotor);
motor_group RightMotors = motor_group(RightFrontMotor, RightMiddleMotor, RightBackMotor);

distanceUnits units = distanceUnits::mm;   // Imperial measurements.
const double WHEEL_TRAVEL = 84*M_PI;       // Circumference of the drive wheels (mm)
double trackWidth   = 290;                 // Distance between the left and right center of wheel. (mm) 
double wheelBase    = 230;                 // Distince between the center of the front and back axle. (mm)
double gearRatio    = 2;                   // Ratio of motor rotations to wheel rotations if using gears. (mm)

inertial inertialSensor(PORT16);

smartdrive Drive = smartdrive(RightMotors, LeftMotors, inertialSensor, WHEEL_TRAVEL, trackWidth, wheelBase, units, gearRatio);

//Expansor motors
motor expansor1 = motor(PORT11, ratio18_1, false);
motor expansor2 = motor(PORT12, ratio18_1, true);
motor_group expansor = motor_group(expansor1, expansor2);

//Intake-Roller motor
motor intake_roller1 = motor(PORT7, ratio18_1, true);
motor intake_roller2 = motor(PORT10, ratio18_1, true);
motor_group intake_roller = motor_group(intake_roller1, intake_roller2);

//Flywheel motors
motor FlywheelDown = motor(PORT8, ratio18_1, true);
motor FlywheelUp = motor(PORT9, ratio18_1, true);
motor_group Flywheel = motor_group(FlywheelDown, FlywheelUp);

// Indexer piston
pneumatics Indexer = pneumatics(Brain.ThreeWirePort.A);

// Controller
controller Controller1 = controller(primary);
bool RemoteControlCodeEnabled = true;

/************* Control variables ***************/
bool DrivetrainLNeedsToBeStopped_Controller1 = true;
bool DrivetrainRNeedsToBeStopped_Controller1 = true;

// Main user controller code
int rc_auto_loop_function_Controller1(){
  while(true){
    if(RemoteControlCodeEnabled){
      //Smartdrive
      int drivetrainLeftSideSpeed  = Controller1.Axis3.position() - Controller1.Axis1.position();
      int drivetrainRightSideSpeed = Controller1.Axis3.position() + Controller1.Axis1.position();
      
      if (drivetrainLeftSideSpeed < DEADBAND && drivetrainLeftSideSpeed > -DEADBAND){
        if (DrivetrainLNeedsToBeStopped_Controller1){
          LeftMotors.stop();  
          DrivetrainLNeedsToBeStopped_Controller1 = false;  
        }
      } else{
        DrivetrainLNeedsToBeStopped_Controller1 = true;
      }

      if (drivetrainRightSideSpeed < DEADBAND && drivetrainRightSideSpeed > -DEADBAND){
        if (DrivetrainRNeedsToBeStopped_Controller1){
          RightMotors.stop();  
          DrivetrainRNeedsToBeStopped_Controller1 = false;  
        }
      } else{
        DrivetrainRNeedsToBeStopped_Controller1 = true;
      }
      
      if (DrivetrainLNeedsToBeStopped_Controller1){
        LeftMotors.setVelocity(drivetrainLeftSideSpeed, percent);
        LeftMotors.spin(forward);
      }
      if (DrivetrainRNeedsToBeStopped_Controller1){
        RightMotors.setVelocity(drivetrainRightSideSpeed, percent);
        RightMotors.spin(forward);
      }

      // Intake - roller
      // Use button A
      if(Controller1.ButtonA.pressing()) {
        intake_roller.setVelocity(INTAKE_VEL, percent);
        intake_roller.spin(forward);
      }
      // Use button L1
      else if (Controller1.ButtonL1.pressing()) {
        intake_roller.setVelocity(INTAKE_VEL, percent);
        intake_roller.spin(reverse);
      }
      else if (Controller1.ButtonB.pressing()){
        intake_roller.stop();
      }

      // Flywheel shoot
      // Use button L2
      if (Controller1.ButtonL2.pressing()){        
        Flywheel.spin(forward, FLYWHEEL_VEL, velocityUnits::pct);
        // Use button UP
        if (Controller1.ButtonUp.pressing()) {
          Flywheel.spin(forward, MAX_FLYWHEEL_VEL, velocityUnits::pct);
        } 
        else {
          Flywheel.spin(forward, FLYWHEEL_VEL, velocityUnits::pct);
        }
        // Use button R2
        if (Controller1.ButtonR2.pressing()){
          Indexer.open();
          wait(INDEXER_BACK, msec);
          Indexer.close();
          wait(INDEXER_GO, msec);
        }
      } else{
        Flywheel.stop();
      }

      // Expansor1 shoot
      // Use button X
      if(Controller1.ButtonX.pressing()){
        expansor1.spinToPosition(EXPANSOR_DEG, rotationUnits::deg);
      } else{
        expansor1.stop(brakeType::brake);
      }

      // Expansor2 shoot
      // Use button Y
      if(Controller1.ButtonY.pressing()){
        expansor2.spinToPosition(EXPANSOR_DEG, rotationUnits::deg);
      } else{
        expansor2.stop(brakeType::brake);
      }

    }
    wait(20,msec);    
  }
  return 0;
}

void vexcodeInit(void){
  inertialSensor.calibrate();
  while (inertialSensor.isCalibrating()) {
    wait(25, msec);
  }
  Indexer.close();
}
