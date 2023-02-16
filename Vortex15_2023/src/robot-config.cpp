#include "vex.h"
using namespace vex;
using signature = vision::signature;
using code = vision::code;

/*-------------------------------- Constants --------------------------------*/
int    DEADBAND       = 5;           //pct
double WHEEL_TRAVEL   = 84*3.14159;  //mm
double TRACK_WIDTH    = 290;         //mm
double WHEEL_BASE     = 230;         //mm
double INDEXER_GO     = 1500;         //pct
double INDEXER_BACK   = 200;         //ms            
double WAIT_UNTIL_LAUNCH = 3100;     //ms
double FLYWHEEL_VEL   = 95;          //pct
double INTAKE_VEL     = 70;          //pcd 

/*---------------------------- Devices definition ---------------------------*/
// Brain screen
brain Brain;

// Drivetrain motors 
motor LeftFrontMotor = motor(PORT1, ratio18_1, false);
motor LeftMiddleMotor = motor(PORT2, ratio18_1, false);
motor LeftBackMotor = motor(PORT3, ratio18_1, true);
motor RightFrontMotor = motor(PORT4, ratio18_1, false);
motor RightMiddleMotor = motor(PORT5, ratio18_1, false);
motor RightBackMotor = motor(PORT6, ratio18_1, true);
motor_group LeftMotors = motor_group(LeftFrontMotor, LeftMiddleMotor, LeftBackMotor);
motor_group RightMotors = motor_group(RightFrontMotor, RightMiddleMotor, RightBackMotor);

//Intake-Roller motor
motor intake_roller = motor(PORT7, ratio18_1, true);

//Flywheel motors
motor FlywheelDown = motor(PORT8, ratio18_1, false);
motor FlywheelUp = motor(PORT9, ratio18_1, true);
motor_group Flywheel = motor_group(FlywheelDown, FlywheelUp);

// Indexer piston
pneumatics Indexer = pneumatics(Brain.ThreeWirePort.A);

// Controller
controller Controller1 = controller(primary);
bool RemoteControlCodeEnabled = true;

/*---------------------------- Control variables ----------------------------*/
bool DrivetrainLNeedsToBeStopped_Controller1 = true;
bool DrivetrainRNeedsToBeStopped_Controller1 = true;

/*----------------------------- Atomic functions ----------------------------*/
/* TODO: Split main user controller code into smaller functions
void compute_chassis_vel(void){}
void drivetrain_run(void){}
void intake_roller_run(void){} 
void flywheel_game_shoot(void){} 
void expansion_run(void){}
*/

// Main user controller code
int rc_auto_loop_function_Controller1(){
  while(true){
    if(RemoteControlCodeEnabled){
      //Drivetrain
      int drivetrainLeftSideSpeed  = Controller1.Axis1.position() + Controller1.Axis3.position();
      int drivetrainRightSideSpeed = Controller1.Axis1.position() - Controller1.Axis3.position();
      if (drivetrainLeftSideSpeed < DEADBAND && drivetrainLeftSideSpeed > -DEADBAND){
        if (DrivetrainLNeedsToBeStopped_Controller1){
          LeftMotors.stop();  
          DrivetrainLNeedsToBeStopped_Controller1 = false;  
        }
      } 
      else{
        DrivetrainLNeedsToBeStopped_Controller1 = true;
      }
      if (drivetrainRightSideSpeed < DEADBAND && drivetrainRightSideSpeed > -DEADBAND){
        if (DrivetrainRNeedsToBeStopped_Controller1){
          RightMotors.stop();  
          DrivetrainRNeedsToBeStopped_Controller1 = false;  
        }
      } 
      else{
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

      //Intake - roller
      if(Controller1.ButtonA.pressing()) {
        intake_roller.setVelocity(INTAKE_VEL, percent);
        intake_roller.spin(forward);
      }
      else if (Controller1.ButtonB.pressing()) {
        intake_roller.setVelocity(INTAKE_VEL, percent);
        intake_roller.spin(reverse);     
      }
      else {
        intake_roller.stop();
      }

      // Flywheel shoot
      if (Controller1.ButtonR2.pressing()){        
        Flywheel.spin(forward, FLYWHEEL_VEL, velocityUnits::pct);
        wait(WAIT_UNTIL_LAUNCH, msec);
        for (int i = 0; i<3; i++){
          Indexer.open();
          wait(INDEXER_BACK, msec);
          Indexer.close();
          wait(INDEXER_GO, msec);
        }
      } else{
        Flywheel.stop();
      }
    }
    wait(20,msec);    
  }
  return 0;
}

void vexcodeInit(void) {
  // Nothing to initialize
}