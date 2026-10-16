#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include "main.h"
#include <string>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include "lemlib/api.hpp"
#include "sensor_loc.cpp"

/*----------------------Defines----------------------*/
inline std::string autonomousPath = "none"; // Default autonomous path
inline std::string teamColor = "none"; // Default team color

// Constants
constexpr double PI = 3.141592;

/*---Controller---*/
inline pros::Controller master(pros::E_CONTROLLER_MASTER);
inline pros::Controller slave(pros::E_CONTROLLER_PARTNER);

/*------------------Global Variables------------------*/
// provides a cleaner way to access controller buttons
namespace buttons {
    constexpr auto R1 = pros::E_CONTROLLER_DIGITAL_R1;
    constexpr auto R2 = pros::E_CONTROLLER_DIGITAL_R2;
    constexpr auto L1 = pros::E_CONTROLLER_DIGITAL_L1;
    constexpr auto L2 = pros::E_CONTROLLER_DIGITAL_L2;
    constexpr auto A = pros::E_CONTROLLER_DIGITAL_A;
    constexpr auto B = pros::E_CONTROLLER_DIGITAL_B;
    constexpr auto Y = pros::E_CONTROLLER_DIGITAL_Y;
    constexpr auto X = pros::E_CONTROLLER_DIGITAL_X;
    constexpr auto UP = pros::E_CONTROLLER_DIGITAL_UP;
    constexpr auto DOWN = pros::E_CONTROLLER_DIGITAL_DOWN;
    constexpr auto LEFT = pros::E_CONTROLLER_DIGITAL_LEFT;
    constexpr auto RIGHT = pros::E_CONTROLLER_DIGITAL_RIGHT;
}

// provides a cleaner way to access motor brake modes
const pros::motor_brake_mode_e_t brake = pros::E_MOTOR_BRAKE_BRAKE; // quickly stops the motor but does not hold position
const pros::motor_brake_mode_e_t coast = pros::E_MOTOR_BRAKE_COAST; // spins freely when stopped
const pros::motor_brake_mode_e_t hold = pros::E_MOTOR_BRAKE_HOLD;   // holds the motor in place when stopped





//==============================================================================================================
//====================================== Start of LemLibs Configuration ========================================
//==============================================================================================================

/*-------------Define all configurations-------------*/
// Standard motor definitions, include the port and gearset
/*
Red Gearset: 100 RPM
Green Gearset: 200 RPM
Blue Gearset: 600 RPM
*/
inline pros::MotorGroup leftDT({1}, pros::v5::MotorGears::green); 
inline pros::MotorGroup rightDT({-5}, pros::v5::MotorGears::green); 
inline pros::Motor arm({4}, pros::v5::MotorGears::green); 
inline pros::Motor claw({7}, pros::v5::MotorGears::green); 
inline pros::Imu IMU(10);
inline pros::Distance distance (2);


// ================== Chassis Configuration ==================

// Drivetrain Settings
lemlib::Drivetrain drivetrain(&leftDT,                      // left motor group
                              &rightDT,                     // right motor group
                              11.75,                        // Track Width
                              lemlib::Omniwheel::NEW_4,     // Wheel Diameter
                              200,                          // Drivetrain rpm 
                              2                             // Horizontal Drift (default 2)
);


// ================== Odometry Configuration ==================

// Odometry Settings
lemlib::OdomSensors sensors(nullptr,    // vertical tracking wheel 1
                            nullptr,    // vertical tracking wheel 2
                            nullptr,    // horizontal tracking wheel 1
                            nullptr,    // horizontal tracking wheel 2,
                            &IMU        // inertial sensor
);



// ================== PID Configuration ==================

/* ]================== PIDs Basics ===================

Basics: 
    - Controls your robot's motion based on previous and possibly future values
    rather than just current values

Vocabulary
- Overshoot:
    - When the robot travels further than intended, either due to momentum or other causes
- Settling time: 
    - Refers to the time the robot takes to adjust its position if it overshoots the target
- Steady-state Error
    - The difference in location between the target and the final position after settling


===================== Tuning PID ====================
The output of a PID controller depends on three components:

- P (Proportional Value) 
    - based on the difference between the current and target value
    - Increasing P makes the robot accelerate more aggressively to reach its target.
- I (Integral Value) 
    - the sum of all the PAST error values
    - The longer the robot has not reached its target, the stronger the adjustments will be made
    - Increasing I will cause the robot to reach its target faster, but risks overshooting.
- D: (Derivative Value) 
    - based on the error's rate of change (derivative) to predict the pattern of adjustments
    - Lowering D causes the robot to slow down more as it approaches the target.

1) Turn all parameters to 0 except kP and kD
2) Follow these instructions:
    Does it oscillate?
        Yes -> increase kD
        No -> record kD and kP, increase kP
    Repeat until increasing kD cant prevent oscillation

    Using This code:
    chassis.setPose(0, 0, 0);
    chassis.turnToHeading(90, 100000);

3) This next step is optional to improve accuracy further. 
    - Turn the robot at various angles between 10 and 180 degrees
    - Each time record the steady-state error (difference between target and actual IMU reading)
    - Fill in the small and large error range
    - The anti-windup is the average of all the error times 150%
    - The timeout describes how long it will spend correcting itself when it is within the error range before giving up

4) Maximum acceleration (slew) describes the maximum angular acceleration the robot can have. 
    Leave this at 0 for most cases. Only increase in extreme situations such as overworked motors.

5) Repeat this process for the lateral PID, instead of turning, move the robot forwards. 
    If you would like to tune the error range, move between 5 and 48 inches.

6) If your robot tends to slip or tip, the slew for the lateral PID should be tuned
    - This is especially important if you do not have tracking wheels -> Internal motor encoders are prone to slippage
    - How to tune slew:
        - Set the slew to 127, and run the code below. Decrease the slew until there is no slippage and/or tipping.
            // set position to x:0, y:0, heading:0
            chassis.setPose(0, 0, 0);
            // move 48" forwards
            chassis.moveToPoint(0, 48, 10000);
*/

// Angular PID Controller
lemlib::ControllerSettings angular_controller(2,    // proportional gain (kP)                       * IMPORTANT
                                              0,    // integral gain (kI)                           * If needed
                                              10,   // derivative gain (kD)                         * IMPORTANT
                                              3,    // anti windup                                  * Optional       
                                              1,    // small error range, in degrees                
                                              100,  // small error range timeout, in milliseconds   
                                              3,    // large error range, in degrees                
                                              500,  // large error range timeout, in milliseconds
                                              0     // maximum acceleration (slew)                  * Safe to ignore
);

lemlib::ControllerSettings lateral_controller(10,   // proportional gain (kP)                       * IMPORTANT
                                              0,    // integral gain (kI)                           * If Needed
                                              3,    // derivative gain (kD)                         * IMPORTANT         
                                              3,    // anti windup                                  * Optional
                                              1,    // small error range, in inches
                                              100,  // small error range timeout, in milliseconds
                                              3,    // large error range, in inches
                                              500,  // large error range timeout, in milliseconds
                                              20    // maximum acceleration (slew)                  * If robot slips or tips
);



// Finally, create the chassis
lemlib::Chassis chassis(drivetrain, // drivetrain settings
                        lateral_controller, // lateral PID settings
                        angular_controller, // angular PID settings
                        sensors // odometry sensors
);


#endif