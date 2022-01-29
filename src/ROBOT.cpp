#include "ROBOT.h"

ROBOT::ROBOT(DIGITAL_ENCABULATOR &rBrain) : Brain(rBrain),
     DriveLeft(_DriveLeftPWM, &Brain.PWM2, _DriveLeftDir, _DriveLeftReversed), 
     DriveRight(_DriveRightPWM, &Brain.PWM2, _DriveRightDir, _DriveRightReversed), 
    Auton(*this)
    
       

{
}

void ROBOT::Setup()
{   
    //Make sure you change this address! The characters are hex. 0-9, and a-f are all acceptable. ex: cf:03:6f:3d:4b:12 
    //You can't have the same address as anyone else, so dont use the default one! After you flash it the first time, you must run the command:
    // pio run -t erase 
    //Before changing it. Otherwise it won't take, and you will get very sad. 
    //I reccomend generating a random one for you to use to minimize chances of conflict 
    //https://miniwebtool.com/mac-address-generator/

    Brain.PS4.begin("cf:03:6f:3d:4b:12");
    Serial.println("PS4 Ready");

    pinMode(EnablePin, INPUT);
    pinMode(_LEDBuiltIn, OUTPUT);
    digitalWrite(_LEDBuiltIn, LOW);
}

void ROBOT::Loop()
{
    //Read The Controller
    DriveLeftSpeed = Brain.PS4Joystick(Brain.LeftHatY,10);
    DriveRightSpeed = Brain.PS4Joystick(Brain.RightHatY,10);
    
    if(PrecisionMode)
    {
        DriveLeftSpeed = (Brain.PS4Joystick(Brain.LeftHatY,10))/2;
        DriveRightSpeed = (Brain.PS4Joystick(Brain.RightHatY,10))/2;
    }

    if(Brain.ButtonClick(Brain.Circle))
    {
        PrecisionMode = !PrecisionMode;
        LargeRumble = 255; 
    }

    if(Brain.ButtonClick(Brain.Up))
    {
       Auton.LaunchQueued();
       LargeRumble = 100;
    }

    if((Brain.ButtonClick(Brain.Right)))
    {
        Auton.QueueNext();
        LargeRumble = 100;
    }

    if(Brain.ButtonClick(Brain.Down))
    {
        Auton.ToggleArmed();
        LargeRumble = 100;
    }

    if(Brain.ButtonClick(Brain.Left))
    {   
        Auton.QueuePrev();
        LargeRumble = 100;
    }

    if(Brain.ButtonClick(Brain.L3))
    {
        Serial.println("L3");
    }

    if(Brain.ButtonClick(Brain.Options))
    {
        Brain.ToggleWIFI();
    }

    //Read The Sensors
    State.EnableVal = digitalRead(EnablePin);

    //Write To Motor Controllers
    if (_NextMotorControllerWriteMillis < millis())
    {
        _NextMotorControllerWriteMillis = millis() + 20;
        DriveLeft.SetMotorSpeed(DriveLeftSpeed);
        DriveRight.SetMotorSpeed(DriveRightSpeed);
        Serial.print("Drive Right Speed"); 
        Serial.println(DriveRightSpeed);
    }

    //Write the Display and controller
    if (_NextDisplayMillis < millis())
    {
        _NextDisplayMillis = millis() + 250;

        if (Auton.IsArmed())
        {
            Brain.OLED.clearDisplay();
            Brain.OLED.setCursor(0, 0);
            Brain.OLED.setTextSize(2);
            Brain.OLED.print("ARMED ");                 
            Brain.OLED.setTextSize(1);
            Brain.OLED.print(Auton.QueuedProgramName());
            Brain.OLED.display();
        }
        else if(Auton.QueuedProgramName() != "")
        {
            Brain.OLED.clearDisplay();
            Brain.OLED.setCursor(0, 0);
            Brain.OLED.setTextSize(2);
            Brain.OLED.print(Auton.QueuedProgramName());
            Brain.OLED.display();
        }
        else
        {
            Brain.OLED.clearDisplay();
            Brain.OLED.setCursor(0, 0);
            Brain.OLED.setTextSize(1);
            Brain.OLED.println("Digital");
            Brain.OLED.println("Encabulator");
            Brain.OLED.println("Team: C3604");
            Brain.OLED.println("PS4 Battery Level");
            Brain.OLED.print(Brain.PS4.data.status.battery, DEC);
            Brain.OLED.println(" / 16");
            Brain.OLED.display();
        }

        //Rumble goes from 0 to 255
        Brain.PS4.setRumble(SmallRumble, LargeRumble);
        //Standard RGB Led, defaults to blue
        Brain.PS4.setLed(0,0,255);
        Brain.PS4.sendToController();
        //Change the rumble to zero after we write it
        LargeRumble = 0;
        SmallRumble = 0;
    }
}
