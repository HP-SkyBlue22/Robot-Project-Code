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
  Optical7.setLightPower(100);
  Optical7.setLight(ledState::on);

  double hueValue = Optical7.hue();         // 0–359 degrees
  double brightness = Optical7.brightness(); // 0–100%
    
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

int totalCount(int currentCount) {

    openClaw();  
    closeClaw();
    return currentCount + 1;
}

void driveAlongRow () // drive and pick up
{

  int detectedColor = -1;
  int pieceCount = 0;

  if (Distance12.objectDistance(mm) <= 1300) 
  {
    MotorLeft.stop();
    MotorRight.stop();
                
    // --- Pickup and Sort Sequence ---
    openClaw(); // Open claw 
                
                
    closeClaw(); // Grip the piece

                
    // Check color here

                
    // Call function to sort the piece.
    moveArmForColor(detectedColor);

    pieceCount = totalCount(pieceCount);
                
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



int main() {

  vexcodeInit();
  // Begin project code

  int detectedColor = -1;
  int pieceCount = 0;

    Brain.Screen.print("Press Touch LED to begin.");
    waitTouch();

      Brain.Screen.setCursor(2,1);
  Brain.Screen.print("Touch the LED Sensor if there’s checkers left on the board.");
  bool askToRemove = touchedIn30S();
  if(askToRemove)
  {
    Brain.Screen.setCursor(4,1);
    Brain.Screen.print("Please remove the pieces from the board.");
  }
  wait(5,seconds);
  Brain.Screen.setCursor(6,1);
  Brain.Screen.print("Please remove the board from the table.");


  driveAndTurn();

    
    Brain.Screen.newLine();
    Brain.Screen.print("Press Touch LED to exit.");
    
    waitTouch();

    return 0;
}
