#pragma once

#include "ev3api.h"

#define PWM_ABS_MAX (100)

using namespace ev3api;

struct PWMs {
    int8_t left;
    int8_t right;
};

class MotorController {
public:
    MotorController();
    ~MotorController();
    void setPWMs(PWMs *pwms);
    void Stop();
    void setBiases(int8_t left, int8_t right);
private:
    int8_t RoundPWM(int8_t);
    Motor *left_motor_;
    Motor *right_motor_;
    int8_t left_bias_;
    int8_t right_bias_;
}