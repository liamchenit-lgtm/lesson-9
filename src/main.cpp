// header files
#include "globals.hpp" //contains motor definitions, other libraries, and global variables
#include "AutonSelector.hpp" // contains how to select a specific path
#include "Autonomous_Paths.hpp" // Contains the different autonomous paths

// creates autonSelector object
AutonSelector autonSelector;

void on_center_button() {}	

void initialize() {
    chassis.calibrate(); // calibrate sensors
    pros::screen::erase();
    pros::delay(1000);
    /*
    Sets up the autons and passes in the all the routes as a vector (similar to an array)
    Each autonomousRoute contains the team color, name, description, and the acutal path
    */
    autonSelector.setAutons(std::vector<autonomousRoute>{
        //autonomousRoute{"color", "name", "description", path from Autonomous_Paths.hpp},
        autonomousRoute{"red", "Red Auton 1", "1st Red Auton", redAuton1},
        autonomousRoute{"red", "Red Auton 2", "2nd Red Auton", redAuton2},
        autonomousRoute{"blue", "Blue Auton 1", "1st Blue Auton", blueAuton1},
        autonomousRoute{"blue", "Blue Auton 2", "2nd Blue Auton", blueAuton2}
    });
    autonSelector.setSkillsAuton(autonomousRoute{"red", "Skills", "Skills Auton", exampleAuton});
    // Starts the auton selector screen (NOT the actual auton yet)
    autonSelector.start();

    // ===================================================================
    // =========== Put standard initialization code below here ===========
    // ===================================================================
}

void disabled() {}
   
void competition_initialize() {}

void autonomous() {
    // Code to run during the autonomous period
    autonSelector.runAuton();
}
 
void opcontrol() {
	while (true) {

        // double rightX = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        // double leftY = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        // leftDT.move_velocity(leftY+rightX);
        // rightDT.move_velocity(leftY-rightX);
        
		pros::delay(20);
		
	}
}