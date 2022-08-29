#include "DerivedDriver.h"

void NaiveStraightDriver::RunWithAMethod(int8_t pwm) {
    motor_controller_->setPWMs(pwm, pwm);
    latest_pwm_ = pwm;
}

void NaiveCurveDriver::RunWithAMethod(int8_t pwm) {
    int8_t left_pwm;
    int8_t right_pwm;
    if (detail_info_->direction == LEFT) {
        left_pwm = static_cast<int8_t>(pwm * detail_info_->low_rate);
        right_pwm = static_cast<int8_t>(pwm * detail_info_->high_rate);
        motor_controller_->setPWMs(left_pwm, right_pwm);
    } else {
        left_pwm = static_cast<int8_t>(pwm * detail_info_->high_rate);
        right_pwm = static_cast<int8_t>(pwm * detail_info_->low_rate);
        motor_controller_->setPWMs(left_pwm, right_pwm);
    }
    latest_pwm_ = pwm;
}

void NaiveRotateDriver::RunWithAMethod(int8_t pwm) {
    if (detail_info_->direction == LEFT) {
        motor_controller_->setPWMs(-pwm, pwm);
    } else {
        motor_controller_->setPWMs(pwm, -pwm);
    }
    latest_pwm_ = pwm;
}
