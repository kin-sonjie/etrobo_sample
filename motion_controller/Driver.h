#pragma once

#include "MotorController.h"
#include "AccelController.h"

/* Driver: 運転手 */
class Driver {
public:
    void Run(void *motion_info);
private:
    PWMs 
    void ControllAccel();
    MotorController motor_controller_
}