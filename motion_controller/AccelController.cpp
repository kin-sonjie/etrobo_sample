#include "AccelController.h"

/* 定数定義 */
const float AccelController::kDeltaTime_ = 0.01; //実行周期 0.01(10ms)

AccelController::AccelController(){
    Init();
}

void AccelController::Init() {
    control_val_ = 0.0;
    control_rate_ = 0.0;
}

void AccelController::setParameters(AccelInfo *accel_info) {
    control_rate_ = static_cast<float>
        (accel_info->target_val - accel_info->start_val) / accel_info->time;
}

float AccelController::getAccel() {
    control_val_ += left_control_rate_ * kDeltaTime_;
    return control_val_;
}