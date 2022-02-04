#include <Arduino.h>
#include <Wire.h>

#include <Encabulator.h>
#include <ROBOT.h>

DIGITAL_ENCABULATOR Brain("Wifi_Name", NULL);
ROBOT Robot(Brain);

void WatchdogTask(void *pvParameters);
void AutonTask(void *pvParameters);

//Arduino 'setup' function
void setup()
{
    Serial.begin(115200);
    //Init I2C
    Wire.begin(23, 22, 400000);

    Brain.Setup();
    Robot.Setup();

    Serial.print("Main Loop: Executing on core ");
    Serial.println(xPortGetCoreID());
    xTaskCreatePinnedToCore(WatchdogTask, "WatchdogTask", 10000, NULL, 1, NULL, 1);
    Serial.print("Watchdog Loop: Executing on core ");
    Serial.println(xPortGetCoreID());
    xTaskCreatePinnedToCore(AutonTask, "AutonTask", 10000, NULL, 1, NULL, 1);
    Serial.print("Auton Loop: Executing on core ");
    Serial.println(xPortGetCoreID());
}

//Arduino 'loop' function
void loop()
{
    Brain.Loop(); 
    Robot.Loop();

    //Let other tasks run
    delay(1);
    yield();
}

//A task is like creating another Arduino 'loop' function. This makes it so multiple things can run at once. 

//Dedicated thread for the watchdog that reboots the brain if it hangs, like from static shock
void WatchdogTask(void *pvParameters)
{
    Serial.print("WatchdogTask: Executing on core ");
    Serial.println(xPortGetCoreID());

        Brain.EnableWatchdog();
        while (true)
        {
            Brain.WatchdogLoop();
            delay(100);
        }
    }

//Task for Auton, so it can run and still take controller inputs
void AutonTask(void *pvParameters)
{
    Serial.print("AutonTask: Executing on core ");
    Serial.println(xPortGetCoreID());
    Robot.Auton.Setup();

    while(true)
    {
        Robot.Auton.Loop();
    }
}