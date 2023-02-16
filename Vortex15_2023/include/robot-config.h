using namespace vex;

/*----------------------------- VEXcode devices -----------------------------*/
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

//Intake-roller
extern motor intake_roller;

//Indexer
extern pneumatics Indexer;

//Flywheel
extern motor FlywheelDown;
extern motor FlywheelUp;
extern motor_group Flywheel;

//Expansion
extern motor Expansion;

//Controller
extern controller Controller1;

//Main functions
extern int rc_auto_loop_function_Controller1();
void vexcodeInit(void);
