#pragma once

#include "MotorController.h"
#include "AccelController.h"
#include "DriveInfoTable.h"

enum DriveState {
    INIT,
    ACCEL,
    CONST,
    DECEL,
    STOP,
    END,
    UNDEF
};

/* abstract class Driver: 運転手*/
class Driver {
public:
    Driver();
    ~Driver();
    void setDriveInfo(DriveInfo *drive_info);
    void resetDriveInfo();
    void Run();
    void Stop();
private:
    void Init();
    void Accelerate();
    void Constant();
    void Decelerate();
    void End();
    void Undefined();
    virtual void RunWithAMethod(int8_t pwm) = 0;
    MotorController *motor_controller_;
    AccelController *accel_controller_;
    static const float kDeltaTime_;
    static int8_t latest_pwm_;
    int8_t target_pwm_;
    float drive_time_;
    float accel_time_;
    void *detail_info_;
    DriveState drive_state_;
    float constant_time_;
    int8_t constant_time_count_;
}