#pragma once

#include "Motor.h"

#define PWM_ABS_MAX (100)

using namespace ev3api;

class MotorController {
public:
    MotorController();
    ~MotorController();
    void setBiases(int8_t left, int8_t right);
    void setPWMs(int8_t left, int8_t right);
    void Stop();
private:
    int8_t RoundPWM(int8_t);
    Motor *left_motor_;
    Motor *right_motor_;
    static int8_t left_bias_;
    static int8_t right_bias_;
};
