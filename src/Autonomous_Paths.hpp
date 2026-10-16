#ifndef AUTON_HPP
#define AUTON_HPP

#include "globals.hpp"

/* ================================== Pure Pursuit ==================================
- Pure pursuit allows you to map out autonomous routes and export them into 
    a set of instructions that LemLibs can run
- It is an algorithm that ensures your robot stays on a given route
    - It works by having an imaginary circle around the robot
    - where that circle intersects the desired robot is the direction the robot drives

- This website allows you to do this: https://path.jerryio.com/
    - Left click adds a waypoint
    - left click (drag) moves waypoints

    - right click waypoint to delete
    - right click path to straighten    

    - black lines in the purple waypoints signal the robots direction
    - smaller purple circles are called controls, allowing you to adjust the shape of the path

- When you are satisfied, save the file as a .txt file
- Then, upload it into the static folder
- The example code below demonstrates how to run the path
- The function used for pure pursuit is chassis.follow()
    - the initial place you put your robot is crucial! 
    - Make sure you are consistant and accurate to the path you drew
- IMPORTANT: Run "pros mu" after editing/adding new paths

chassis.follow(
    asset& path         // Path name                                        ie: example_txt
    float 15            // lookahead value                                  ie: 15 (inches)
    int timeout         // Maximum timeout                                  ie: 5000 (milliseconds)
    bool forwards       // (Optional) should the robot face forwards?       ie: true (default is true) 
    bool async          // (Optional) Should code be non blocking?          ie: true (default is true)
);

chassis.waitUntil(
    float distance      // distance to wait         ie: 15 (inches)
);
- blocks the code until the specified distance (in inches) has been moved

chassis.waitUntilDone();
- blocks the code until the entire path is finished 

Practice: 
    1) Try running the example code below and see the route it takes.
    2) Try making forward = false and changing the lookahead. How does this affect the route?
    3) Look at the route you made in the last lesson, replicate it with pure pursuit and compare the differences
    4) Now create 2 paths and run one after another
    5) Now picking up a ball in between the 2 paths now!
*/


// Load the path thats saved in "example.txt"
ASSET(sampleRoute_txt);

void redAuton1() {
    chassis.setPose(-61.8, 15.7, 0);
    // lookahead distance: 15 inches
    // timeout: 2000 ms
    chassis.follow(sampleRoute_txt, 15, 15000);
    chassis.waitUntil(12);
    claw.move(100);

    chassis.waitUntilDone();
    claw.brake();
}



/*================================== Motion Chaining ==================================
- Motion chaining: Using multiple motion functions without delay or blocking code
- It is an alternative to pure pursuit, offering more manual control 
- Generally motion chaining is best for traveling long distances, like across the field

Pros of Pure Pursuit:
- can navigate complex turns smoothly
- comes with a easy to use GUI to build paths
- useful for prototyping and visualizing a route quickly

Pros of Motion Chaining:
- quicker to implement for straight forward turns and driving
- easier to debug and visually see route
- fine controls over pauses, delays, and exits  

Techniques of Motion Chaining
    1. Utilizing minimum speed
        - To prevent the robot from slowing down during motion commands, specify a minimum speed
    2. Early exit ranges
        - Having a minimum speed may make settling the robot more difficult
            - Why? Higher speeds mean you will overshoot your target and spend the whole timeout time adjusting
        - Solution: Add an early exit range
        - For turning commands, this is a an angle that within the target that will end the command early
        - For motion commands, it is inches from the target
    3. Cancelling commands
        - chassis.cancelMotion() cancels the current command in the queue
        - chassis.cancelAllMotion() cancels all motion commands in queue

Practice:
    1) Using motion chaining, have the robot drive to a set position and pick up a ball
        - Use cancelMotion or cancelAllMotion in combination with the distance sensor to detect if a ball has been picked up
    2) Use both motion chaining and pure pursuit to pick up a ball and score!
        - use pure pursuit to navigate to the ball then motion chaining to score
*/

// Example: Moving around a obstacle to the robot's right side
void redAuton2() {
    chassis.setPose(0, 0, 90);

    // move to intermediate position
    chassis.moveToPose(
        30,
        -20,
        90,
        2000,
        {.minSpeed=72, .earlyExitRange=8}
        // a minSpeed of 72 means that the chassis will slow down as
        // it approaches the target point, but it won't come to a full stop

        // an earlyExitRange of 8 means the movement will exit 8" away from
        // the target point
    );

    // go to target position
    chassis.moveToPose(60, 3, 0, 2000);
}

void blueAuton1() { 
}       
 

void blueAuton2() {
}


void exampleAuton() {
}

#endif