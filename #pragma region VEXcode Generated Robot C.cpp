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
motor MotorRight = motor(PORT1, true);
motor MotorLeft = motor(PORT6, false);
touchled TouchLED9 = touchled(PORT9);
distance Distance12 = distance(PORT12);
motor armMotor = motor(PORT2, false);
motor clawMotor = motor(PORT3, false);
optical Optical8 = optical(PORT8);


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

void waitTouch() 
{
  while(!TouchLED9.pressing())
  {}
  while(TouchLED9.pressing())
  {}
}


bool touchedIn30S()
{
  Brain.Timer.reset();
  while(Brain.Timer.value()<30)
  {
    if(TouchLED9.pressing())
    {
      return true;
    }
  }
  return false;
}

void closeClaw()
{
  clawMotor.setPosition(0,turns);
//  bool piece = (distanceSensor.objectDistance(mm)<130);
//  if (piece)
  {
    clawMotor.spin(reverse, 50, percent);
    while (clawMotor.position(degrees)<-90)
    {}
    clawMotor.stop();
  }
 
  // else
  // {
  //   Brain.Screen.print("No piece detected");
  // }
}

void openClaw()
{
  clawMotor.setPosition(0,turns);
  clawMotor.spin(forward,50, percent);
  while (clawMotor.position(degrees)<90)
  {}
  clawMotor.stop(); 
}

int whichColour()
{
  Optical8.setLightPower(100);
  Optical8.setLight(ledState::on);

  double hueValue = Optical8.hue();         // 0–359 degrees
  double brightness = Optical8.brightness(); // 0–100%
    
  int detectedColour = 0; // 0 = none, 1 = black, 2 = white
  Brain.Screen.clearScreen();

  // Brown: medium brightness (15–50%) and warm hue (30–36°)
  // Off-White: higher brightness (> 55%)

    if ((hueValue > 30 && hueValue < 36) && (brightness >= 15 && brightness <= 50)) 
    {
      detectedColour = 1;
      Brain.Screen.print("Black");
    }
    else if (brightness > 55) 
    {
      detectedColour = 2;
      Brain.Screen.print("White");
    }
    else 
    {
      detectedColour = 0;
      Brain.Screen.print("no");
    }

    // --- Debug info ---
    Brain.Screen.newLine();
    Brain.Screen.print("Hue: %.1f", hueValue);

    wait(200, msec);

    return detectedColour;
}

void moveArmForColor(int detectedColor)
{
  // detectedColor: 1 = Black, 2 = White
  if (detectedColor == 1)
  {  // Black
    Brain.Screen.newLine();
    Brain.Screen.print("Box: Black");
    armMotor.spinToPosition(180, degrees, true); // Changed to blocking for simplicity
    openClaw(); // your teammate’s claw code
    armMotor.spinToPosition(0, degrees, true);
  }
  else if (detectedColor == 2)
  { // White
    Brain.Screen.newLine();
    Brain.Screen.print("Box: White");
    armMotor.spinToPosition(120, degrees, true); // Changed to blocking
    openClaw();
    armMotor.spinToPosition(0, degrees, true);
  }
}

int totalCount(int currentCount) 
{

    openClaw();  
    closeClaw();
    return currentCount + 1;
}

void driveAlongRow () // drive and pick up
{

  int detectedColor = -1;
  int pieceCount = 0;

  if (Distance12.objectDistance(mm) <= 130) 
  {
    MotorLeft.stop(brake);
    MotorRight.stop(brake);
    openClaw(); // Open claw 
                
                
    closeClaw(); // Grip the piece

                
    // Check color here
    detectedColor = whichColour();

                
    // Call function to sort the piece.
    moveArmForColor(detectedColor);

    pieceCount = totalCount(pieceCount);
    // currently drops piece into box then adds 1 to count of pieces
                
    // Resume driving
    MotorLeft.spin(forward);
    MotorRight.spin(forward);
  }

  MotorLeft.stop();
  MotorRight.stop();
}

void driveAndTurn(int motorPower, int &detectedColor) 
{
    MotorLeft.setVelocity(motorPower,percent);
    MotorRight.setVelocity(motorPower,percent);
    for (int row = 0; row < 8; row++) 
    {
        MotorLeft.spin(forward);
        MotorRight.spin(forward);
        
        // Reset the encoder at the start of each row to measure distance
        MotorLeft.setPosition(0, turns);

        while (MotorLeft.position(turns)*200< 400) // 16 inches to cm? 
        {
            driveAlongRow();
        }
    }
}

/*
void driveAndTurn(int motorPower, int &detectedColor) 
{
    MotorLeft.setVelocity(motorPower,percent);
    MotorRight.setVelocity(motorPower,percent);
    for (int row = 0; row < 8; row++) {
        MotorLeft.spin(forward);
        MotorRight.spin(forward);

        // Reset the encoder at the start of each row to measure distance
        MotorLeft.setPosition(0, turns);

        while ((MotorLeft.position*200) < 406) // 16 inches to cm? 
        {
            driveAlongRow();
        }

        if (row < 7) {
           BrainInertial.setRotation(0,degrees);
           if (row % 2 == 0) { 
               BrainInertial.setRotation(0, degrees);
                 MotorLeft.spin(forward);
                 MotorRight.spin(reverse);
                
           } else { 
               BrainInertial.setRotation(0, degrees);
               MotorLeft.spin(reverse);
               MotorRight.spin(forward);
              
           }
           while (abs(BrainInertial.rotation(degrees)) < abs(angle))
           {}
           MotorLeft.stop();
           MotorRight.stop();
          
           MotorLeft.setPosition(0, turns);

           MotorLeft.spin(forward);
           MotorRight.spin(forward);
           while((MotorLeft.position(turns) * 200) < ) // move to next row
           {}
           MotorLeft.stop();
           MotorRight.stop();

          if (row % 2 == 0) { // Right turn again
               BrainInertial.setRotation(0, degrees);
               MotorLeft.spin(forward); MotorRight.spin(reverse);
               while(BrainInertial.rotation() < 90) {}
           } else { // Left turn again
               BrainInertial.setRotation(0, degrees);
               MotorLeft.spin(reverse); MotorRight.spin(forward);
               while(BrainInertial.rotation() > -90) {}
           }
           MotorLeft.stop(); MotorRight.stop();
       }
   }
} // or *-1 to turn

*/

int main() {

  vexcodeInit();
  // Begin project code

  int detectedColor = -1;
  int pieceCount = 0;
  
  // Brain.Screen.setFont(mono15);

  // Brain.Screen.setCursor(1,1);
  // Brain.Screen.print("Press Touch LED");
  // Brain.Screen.setCursor(2,1);
  // Brain.Screen.print("to begin.");
  // waitTouch();
  // Brain.Screen.clearLine(1);

  // Brain.Screen.setCursor(1,1);
  // Brain.Screen.print("Touch LED Sensor");
  // Brain.Screen.setCursor(2,1);
  // Brain.Screen.print("if there are pieces");
  // Brain.Screen.setCursor(3,1);
  // Brain.Screen.print("remaining on board.");
  // bool askToRemove = touchedIn30S();
  // if(askToRemove)
  // {
  //   Brain.Screen.setCursor(4,1);
  //   Brain.Screen.print("Please sweep pieces");
  //   Brain.Screen.setCursor(5,1);
  //   Brain.Screen.print("onto table,");
  //   Brain.Screen.setCursor(6,1);
  //   Brain.Screen.print("and remove board.");
  // }
  // wait(5,seconds);
  // Brain.Screen.setCursor(4,1);
  // Brain.Screen.print("Please remove the");
  // Brain.Screen.setCursor(5,1);
  // Brain.Screen.print("board from table.");
  // wait(5,seconds);
  // Brain.Screen.clearScreen();

  waitTouch();

while(!TouchLED9.pressing())
{
  driveAndTurn(50, detectedColor);

    
    Brain.Screen.newLine();
    Brain.Screen.print("Press Touch LED to exit.");
    waitTouch();
}

  Brain.programStop();
}
