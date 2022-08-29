#pragma once

#include "ev3api.h"

struct AccelInfo {
    int8_t start_pwm;
    int8_t target_pwm;
    float time; //[sec]
};

class AccelController {
public:
    AccelController();
    void Init();
    void setControlRate(int8_t start_pwm, int8_t target_pwm, float time);
    float getControlVal();
private:
    float RoundControlRate(float control_rate);
    static const float kDeltaTime_;
    static const float kRateAbsMax_;
    float control_val_;
    float control_rate_;
};