#pragma region VEXcode Generated Robot Configuration
// Make sure all required headers are included.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>


#include "vex.h"

using namespace vex;

// Brain should be defined by default
brain Brain;


// START IQ MACROS
#define waitUntil(condition)                                                   \
  do {                                                                         \
    wait(5, msec);                                                             \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)
// END IQ MACROS


// Robot configuration code.
inertial BrainInertial = inertial();
motor MotorLeft = motor(PORT1, false);
motor MotorRight = motor(PORT6, true);
optical Optical3 = optical(PORT3);
distance Distance12 = distance(PORT12);
motor clawMotor = motor(PORT8, false);
touchled TouchLED9 = touchled(PORT9);
motor armMotor = motor(PORT2, false);


// generating and setting random seed
void initializeRandomSeed(){
  wait(100,msec);
  double xAxis = BrainInertial.acceleration(xaxis) * 1000;
  double yAxis = BrainInertial.acceleration(yaxis) * 1000;
  double zAxis = BrainInertial.acceleration(zaxis) * 1000;
  // Combine these values into a single integer
  int seed = int(
    xAxis + yAxis + zAxis
  );
  // Set the seed
  srand(seed); 
}

// Converts a color to a string
const char* convertColorToString(color col) {
  if (col == colorType::red) return "red";
  else if (col == colorType::green) return "green";
  else if (col == colorType::blue) return "blue";
  else if (col == colorType::white) return "white";
  else if (col == colorType::yellow) return "yellow";
  else if (col == colorType::orange) return "orange";
  else if (col == colorType::purple) return "purple";
  else if (col == colorType::cyan) return "cyan";
  else if (col == colorType::black) return "black";
  else if (col == colorType::transparent) return "transparent";
  else if (col == colorType::red_violet) return "red_violet";
  else if (col == colorType::violet) return "violet";
  else if (col == colorType::blue_violet) return "blue_violet";
  else if (col == colorType::blue_green) return "blue_green";
  else if (col == colorType::yellow_green) return "yellow_green";
  else if (col == colorType::yellow_orange) return "yellow_orange";
  else if (col == colorType::red_orange) return "red_orange";
  else if (col == colorType::none) return "none";
  else return "unknown";
}


// Convert colorType to string
const char* convertColorToString(colorType col) {
  if (col == colorType::red) return "red";
  else if (col == colorType::green) return "green";
  else if (col == colorType::blue) return "blue";
  else if (col == colorType::white) return "white";
  else if (col == colorType::yellow) return "yellow";
  else if (col == colorType::orange) return "orange";
  else if (col == colorType::purple) return "purple";
  else if (col == colorType::cyan) return "cyan";
  else if (col == colorType::black) return "black";
  else if (col == colorType::transparent) return "transparent";
  else if (col == colorType::red_violet) return "red_violet";
  else if (col == colorType::violet) return "violet";
  else if (col == colorType::blue_violet) return "blue_violet";
  else if (col == colorType::blue_green) return "blue_green";
  else if (col == colorType::yellow_green) return "yellow_green";
  else if (col == colorType::yellow_orange) return "yellow_orange";
  else if (col == colorType::red_orange) return "red_orange";
  else if (col == colorType::none) return "none";
  else return "unknown";
}


void vexcodeInit() {

  // Initializing random seed.
  initializeRandomSeed(); 
}

#pragma endregion VEXcode Generated Robot Configuration

//----------------------------------------------------------------------------
//                                                                            
//    Module:       main.cpp                                                  
//    Author:       {author}                                                  
//    Created:      {date}                                                    
//    Description:  IQ project                                                
//                                                                            
//----------------------------------------------------------------------------

// Include the IQ Library
#include "iq_cpp.h"

// Allows for easier use of the VEX Library
using namespace vex;

void configureAllSensors(){
  BrainInertial.calibrate();
  wait(2,seconds);
  BrainInertial.setHeading(0,degrees);
  BrainInertial.setRotation(0,degrees);
  MotorLeft.setPosition(0,turns);
  MotorRight.setPosition(0,turns);
  Brain.Screen.clearScreen();
  Brain.Screen.setFont(mono15);
  Optical3.setLight(ledState::on);
}

//rotate robot to a certain angle at a given power
void rotateRobot (int angle, int motorPower){
  BrainInertial.setRotation(0,degrees);
  wait(50, msec);

  MotorLeft.setVelocity(motorPower,percent);
  MotorRight.setVelocity(motorPower,percent);
  //for positive angle, rotates clockwise
  if (angle>0) {
    MotorLeft.spin(forward);
    MotorRight.spin(reverse); 
  }
  //for negative angle, rotates counterclockwise
  else {
    MotorLeft.spin(reverse);
    MotorRight.spin(forward);
  }
  while (abs(BrainInertial.rotation(degrees))<abs(angle)) {
  }

  MotorLeft.stop();
  MotorRight.stop();

  BrainInertial.setRotation(0,degrees);
  MotorLeft.resetPosition();
  MotorRight.resetPosition();
}

int waitTouch()
{  
  while(!TouchLED9.pressing())
  {}
  
  while(TouchLED9.pressing())
  {}
  return 1; // <-- return something to mark that a press occurred
}

void closeClaw()
{
  clawMotor.setPosition(0,degrees);
  clawMotor.spin(reverse, 50, percent);
  return;
}

void openClaw()
{
  clawMotor.setPosition(0,degrees);
  clawMotor.spin(forward,50, percent);
  while (clawMotor.position(degrees)<10)
  {}
  clawMotor.stop(); 
  return;
}

//make a function to detect colour and return -1 1 or 2
int whichColor()
{
  double hueValue = Optical3.hue();         // 0–359 degrees
  double brightness = Optical3.brightness(); // 0–100%
    
  int detectedColor = -1; // -1 = none, 1 = black, 2 = white
  Brain.Screen.clearScreen();

  // --- Color classification ---
  // Brown: medium brightness (15–50%) and warm hue (20–45°)
  // Beige/light off-white: higher brightness (> 55%)

  if ((hueValue > 30 && hueValue <= 38) && (brightness >= 10 && brightness <= 50)) 
  {
    detectedColor = 1;
    Brain.Screen.print("Black");
  }
  else if ((hueValue > 38 && hueValue <= 40) && brightness > 55.5) 
  {
    detectedColor = 2;
    Brain.Screen.print("White");
  }
  else 
  {
    detectedColor = -1;
    Brain.Screen.print("no");
  }
  Brain.Screen.newLine();
  Brain.Screen.print("Hue: %.1f", hueValue);
  

  wait(200, msec);

  return detectedColor;
}

void moveArmForColor(int colorDetected) 
{
  // colorID: 1 = Black, 2 = White
    // React ONCE
    if(colorDetected == -1)
    {}
    else if (colorDetected == 1)
    {
      closeClaw();
      wait(1, seconds);  
      armMotor.spin(forward,20,percent);
      armMotor.spinToPosition(143, degrees);
      wait(1,seconds); //for arm to settle
      openClaw();
      wait(1, seconds);
      armMotor.spin(reverse,16,percent);
      while(armMotor.position(degrees) > 0)
      {}
      armMotor.stop(brake);
    }
    else if (colorDetected == 2)
    {
      closeClaw();
      wait(1, seconds);
      armMotor.spin(forward,25,percent);
      armMotor.spinToPosition(175, degrees);
      wait(1,seconds);
      openClaw();
      wait(1, seconds);
      armMotor.spin(reverse,16,percent);
      while(armMotor.position(degrees) > 0)
      {}
      armMotor.stop(brake);
    }
      // Prevent re-triggering until ready again
      wait(2, seconds);
}

int main() 
{
  // Turn on optical sensor LED for consistent lighting
  Optical3.setLightPower(100);
  Optical3.setLight(ledState::on);
  armMotor.setMaxTorque(100, percent);

  wait(1, seconds); // Allow time for stabilization

  while (!TouchLED9.pressing()) 
  {
    whichColor();
    int colorDetected = whichColor();
    moveArmForColor(colorDetected);
  }
  Brain.programStop();
}
