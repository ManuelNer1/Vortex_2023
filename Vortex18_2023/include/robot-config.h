/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       robot-config.h                                            */
/*    Author:       VORTEX Robotics                                           */
/*                  For more information contact A01706424@tec.mx             */
/*    Created:      18-feb-2023                                               */
/*    Description:  Default header for V5 projects                            */
/*                                                                            */
/*----------------------------------------------------------------------------*/

using namespace vex;

/*****************  VEXcode devices *******************/
//Brain
extern brain Brain;

//Drivetrain
extern motor LeftFrontMotor;
extern motor LeftMiddleMotor;
extern motor LeftBackMotor;
extern motor RightFrontMotor;
extern motor RightMiddleMotor;
extern motor RightBackMotor;
extern motor_group LeftMotors;
extern motor_group RightMotors;
extern smartdrive Drive;

//Inertial sensor
extern inertial inertialSensor;

//Expansor
extern motor expasor1;
extern motor expansor2; 
extern motor_group expansor;

//Intake-roller
extern motor intake_roller1;
extern motor intake_roller2;
extern motor_group intake_roller;

//Indexer
extern pneumatics Indexer;

//Flywheel
extern motor FlywheelDown;
extern motor FlywheelUp;
extern motor_group Flywheel;

//Controller
extern controller Controller1;

//Main functions
extern int rc_auto_loop_function_Controller1();
extern void vexcodeInit(void);
