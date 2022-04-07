#ifndef ROBOT_h
#define ROBOT_h

#include <Encabulator.h>
#include <STATE.h>
#include <CytronMDD10.h>
#include "Autonomous/AUTONOMOUS.h"

class ROBOT
{
  public:
    ROBOT(DIGITAL_ENCABULATOR &rBrain);

    void Setup();
    void Loop();

    DIGITAL_ENCABULATOR &Brain;

    STATE State;

    //Motors - declare new ones here and in Robot.cpp
    CytronMDD10 DriveLeft;
    CytronMDD10 DriveRight;

    //Auton
    AUTONOMOUS Auton;
     
    //PS4
   

    //PIN Declarations
    static const int _DriveLeftPWM = 1;
    static const int _DriveLeftDir = 2;
    static const bool _DriveLeftReversed = true;
        
    static const int _DriveRightPWM = 3;
    static const int _DriveRightDir = 4;
    static const bool _DriveRightReversed = false;
      
    static const uint8_t _LEDBuiltIn = 13;
  

  private:
      long _NextDisplayMillis = 0;
      long _NextMotorControllerWriteMillis = 0;
      int DriveRightSpeed = 0;
      int DriveLeftSpeed = 0;
      int EnablePin = 39;
      bool PrecisionMode = false;
      int LargeRumble = 0;
      int SmallRumble = 0;
};

#endif