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
distance Distance12 = distance(PORT7);
motor armMotor = motor(PORT2, false);
motor clawMotor = motor(PORT8, false);
optical Optical8 = optical(PORT3);


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
  Optical8.setLight(ledState::on);
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
  while(Brain.Timer.value()<10)
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
  clawMotor.setPosition(0,degrees);
  clawMotor.spin(reverse, 50, percent);
  
  return;
}

void openClaw()
{
  clawMotor.setPosition(0,degrees);
  clawMotor.spin(forward,50, percent);
  while (clawMotor.position(degrees)<30)
  {}
  clawMotor.stop(); 
  return;
}

//make a function to detect colour and return -1 1 or 2
int whichColor()
{
  double hueValue = Optical8.hue();         // 0–359 degrees
  double brightness = Optical8.brightness(); // 0–100%
    
  int detectedColor = -1; // -1 = none, 1 = black, 2 = white
  Brain.Screen.clearScreen();

  // --- Color classification ---
  // Brown: medium brightness (15–50%) and warm hue (20–45°)
  // Beige/light off-white: higher brightness (> 55%)

  if ((hueValue > 30 && hueValue < 38.3) && (brightness >= 8.6 && brightness <= 50)) 
  {
    detectedColor = 1;
    Brain.Screen.print("Black");
  }
  else if (hueValue > 39 && brightness > 55.5) 
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

void moveArmForColor(int colorDetected, int& pieceCount) 
{
  // colorID: 1 = Black, 2 = White
    // React ONCE
    if(colorDetected == -1)
    {}
    else if (colorDetected == 1)
    {
      closeClaw();
      wait(1, seconds);  
      armMotor.spin(forward,30,percent);
      armMotor.spinToPosition(148, degrees);
      wait(2,seconds); //for arm to settle

      //check colour one more time to ensure dropped here , then add

double hueValue = Optical8.hue();         // 0–359 degrees
  double brightness = Optical8.brightness(); // 0–100%

     if ((hueValue > 30 && hueValue < 38.3) && (brightness >= 8.6 && brightness <= 50))
  {
    pieceCount = pieceCount+1;
    Brain.Screen.print("dropped");
  }
  else if (hueValue > 39 && brightness > 55.5) 
  {
    pieceCount = pieceCount+1;
        Brain.Screen.print("dropped");

  }
      openClaw();
      wait(1, seconds);
      armMotor.spin(reverse,10,percent);
      while(armMotor.position(degrees) > 0)
      {}
      armMotor.stop(brake);
    }



    else if (colorDetected == 2) // if white
    {
      closeClaw();
      wait(1, seconds);
      armMotor.spin(forward,30,percent);
      armMotor.spinToPosition(175, degrees);
      wait(2,seconds);
      //check colour one more time to ensure dropped here , then add

double hueValue = Optical8.hue();         // 0–359 degrees
  double brightness = Optical8.brightness(); // 0–100%

    if ((hueValue > 30 && hueValue < 38.3) && (brightness >= 8.6 && brightness <= 50)) 
  {
    pieceCount = pieceCount+1;
    Brain.Screen.print("dropped");
  }
  else if (hueValue > 39 && brightness > 55.5) 
  {
    pieceCount = pieceCount+1;
        Brain.Screen.print("dropped");

  }
      openClaw();
      wait(1, seconds);
      armMotor.spin(reverse,10,percent);
      while(armMotor.position(degrees) > 0)
      {}
      armMotor.stop(brake);
    }
    
        

      // Prevent re-triggering until ready again
      wait(2, seconds);
}


int totalCount(int currentCount) 
{
    return currentCount + 1;
}

void driveAlongRow (int &pieceCount, double distBenchmark) // drive and pick up
{

  int detectedColor = -1;
  // Brain.Screen.newLine();
  //   Brain.Screen.print("drive along row");
    Brain.Screen.newLine();
        Brain.Screen.newLine();

  // Brain.Screen.print("%.f", distBenchmark);
  if (Distance12.objectDistance(mm) < (12) || whichColor()!= -1) 
   {
    Brain.Screen.newLine();
    Brain.Screen.print("distance detected");
    MotorLeft.stop(brake);
    MotorRight.stop(brake);
                
                

    wait(2, seconds);            
    // Check color here
    
    detectedColor = whichColor();
    

                
    // Call function to sort the piece.
    moveArmForColor(detectedColor, pieceCount);


    // no longer drops piece into box, only adds 1 to count of pieces
                
    // Resume driving
    MotorLeft.spin(forward);
    MotorRight.spin(forward);
  }

}

void driveAndTurn(int motorPower, int &detectedColor, int &pieceCount) 
{
    MotorLeft.setVelocity(motorPower,percent);
    MotorRight.setVelocity(motorPower,percent);
     for (int row = 0; row < 10; row++)  // changed to 2 for testing, change back to 8
     {
  Brain.Screen.print("im moving!");
        MotorLeft.spin(forward);
        MotorRight.spin(forward);
          double distBenchmark = Distance12.objectDistance(mm); // benchmark at beginning of row
        
  //       // Reset the encoder at the start of each row to measure distance
        MotorLeft.setPosition(0, turns);

        while (MotorLeft.position(turns)*200 < 500) // ~25 inches to cm
        {
            driveAlongRow(pieceCount, distBenchmark);
        }
        Brain.Screen.newLine();
            Brain.Screen.print("Finished row");

        if (row < 10) {


          
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
           
           
           
           
           while (abs(BrainInertial.rotation(degrees)) < abs(71))
           {}
           MotorLeft.stop();
           MotorRight.stop();
          
           MotorLeft.setPosition(0, turns);

           MotorLeft.spin(forward);
           MotorRight.spin(forward);
           while((MotorLeft.position(turns) * 200) < 50) // move to next row, filler number
           {}
           MotorLeft.stop();
           MotorRight.stop();

           



          if (row % 2 == 0) { 
               BrainInertial.setRotation(0, degrees);
                 MotorLeft.spin(forward);
                 MotorRight.spin(reverse);
                
           } else { 
               BrainInertial.setRotation(0, degrees);
               MotorLeft.spin(reverse);
               MotorRight.spin(forward);
              
           }
           while (abs(BrainInertial.rotation(degrees)) < abs(69)) // 67 < x < 69
           {} // this can be a function lowkey



        
   MotorLeft.stop(brake);
   MotorRight.stop(brake); // girl....... (i forgor)
  //   }
}
     }}

int main() {

  vexcodeInit();
  // Begin project code

  int detectedColor = -1;
  int pieceCount = 0;
  //configureAllSensors();
    Optical8.setLightPower(100);
  Optical8.setLight(ledState::on);
  armMotor.setMaxTorque(100, percent);

  

  
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
// are we ready to comment this out...
// while(Brain.Timer.value() < 1000)
// {
// driveAlongRow(pieceCount);
// }
  driveAndTurn(30, detectedColor, pieceCount);

    
    // Brain.Screen.newLine();
    // Brain.Screen.print("Press Touch LED to exit.");
    Brain.Screen.newLine();
    Brain.Screen.print("Collected: %d", pieceCount);
    MotorRight.stop(brake);
    MotorLeft.stop(brake);
    waitTouch();


  Brain.programStop();
}
