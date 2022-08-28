#pragma once

#include "ev3api.h"

struct AccelInfo {
    int8_t start_val;
    int8_t target_val;
    float time; //[sec]
};

class AccelController {
public:
    AccelController();
    void Init();
    void setParameters(AccelInfo *accel_info);
    float getAccel();
private:
    static const float kDeltaTime_;
    float control_val_;
    float control_rate_;
};