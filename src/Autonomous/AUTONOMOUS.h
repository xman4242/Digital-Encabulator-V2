#pragma once

#include <Arduino.h>
#include <Encabulator_AUTON.h>

    class ROBOT;

    class AUTONOMOUS : public ENCABULATOR_AUTON
    {

    public:
    AUTONOMOUS(ROBOT &refRobot);
    void Loop();


    private:
    ROBOT &Robot;

    void ExecAuton1();
    void ExecAuton2();
    void ExecAuton3();
    void ExecAuton4();

    };


