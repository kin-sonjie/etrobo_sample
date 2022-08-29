#include "Driver.h"

/* 定数定義 */
const float Driver::kDeltaTime_ = 0.01; //実行周期 0.01[sec] (10ms)
/* 静的変数定義 */
int8_t Driver::latest_pwm_ = 0; //上位側から強制的にDriverを変更された際にできるだけ滑らかに推移するため

Driver::Driver() {
    MotorController Driver::motor_controller_ = new MotorController();
    AccelController Driver::accel_controller_ = new AccelController();
    resetDriveInfo();
    drive_state_ = INIT;
}

Driver::~Driver() {
    delete motor_controller_;
    delete accel_controller_;
}

void Driver::setDriveInfo(DriveInfo *drive_info) {
    target_pwm_ = drive_info->target_pwm;
    drive_time_ = drive_info->drive_time;
    accel_time_ = drive_info->accel_time;
    detail_info_ = drive_info->detail_info;
}

void Driver::resetDriveInfo() {
    target_pwm_ = 0;
    drive_time_ = 0.0;
    accel_time_ = 0.0;
    detail_info_ = NULL;
}

void Driver::Run() {
    switch (drive_state_) {
        case INIT:
            Init();
            break;
        case ACCEL:
            Accelerate();
            break;
        case CONST:
            Constant();
            break;
        case DECEL:
            Decelerate();
            break;
        case STOP:
            Stop();
            break;
        case END:
            End();
            break;
        case UNDEF:
            Undefined();
            break;
        default:
            break;
    }
}

void Driver::Init() {
    if (drive_time_ != 0.0) {
        if (latest_pwm_ == 0) {
            drive_state_ = ACCEL;
            accel_controller_->Init();
            accel_controller_->setControlRate(0, target_pwm_, accel_time_);
        } else if (latest_pwm_ < target_pwm_) {
            drive_state_ = ACCEL;
            accel_controller_->Init();
            accel_controller_->setControlRate(latest_pwm_, target_pwm_, accel_time_);
        } else {
            drive_state_ = CONST;
            constant_time_ = drive_time_ - (accel_time_ * 2);
            constant_time_count_ = 0; 
        }
    }
}

void Driver::Accelerate() {
    float control_val = accel_controller_->getControlVal();
    if (control_val > target_pwm_) {
        RunWithAMethod(target_pwm_);
        drive_state_ = CONST;
        constant_time_ = drive_time_ - (accel_time_ * 2);
        constant_time_count_ = 0;
    } else {
        RunWithAMethod(static_cast<int8_t>(control_val));
    }
}

void Driver::Constant() {
    constant_time_count_ += 1;
    RunWithAMethod(target_pwm_);
    if (constant_time_count_ * kDeltaTime_ > constant_time_) {
        drive_state_ = DECEL;
        accel_controller_->Init();
        accel_controller_->setControlRate(target_pwm_, 0, accel_time_);
    }
}

void Driver::Decelerate() {
    float control_val = accel_controller_->getControlVal();
    if (control_val < 0) {
        RunWithAMethod(0);
        drive_state_ = STOP;
    } else {
        RunWithAMethod(static_cast<int8_t>(control_val));
    }
}

void Driver::Stop() {
    motor_controller_->Stop();
    drive_state_ = END;
}

void Driver::End() {
    /* 終了処理(未定) ex)上位に通知とか? */
    drive_state_ = UNDEF;
}

void Driver::Undefined() {
    ;
}