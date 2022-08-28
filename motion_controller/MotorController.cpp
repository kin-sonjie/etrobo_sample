#include "MotorController.h"

/**
 * @fn      MotorController::
 * @brief   
 */
MotorController::MotorController() {
    left_motor_ = new Motor(PORT_C);
    right_motor_ = new Motor(PORT_B);
    left_bias_ = 0;
    right_bias_ = 0;
}

/**
 * @fn      MotorController::
 * @brief   
 */
MotorController::~MotorController() {
    delete left_motor_;
    delete right_motor_;
}

/**
 * @fn      MotorController::
 * @brief   
 */
int8_t MotorController::RoundPWM(int8_t pwm) {
    if (left_pwm > PWM_ABS_MAX) {
        return PWM_ABS_MAX;
    } else if (left_pwm < -PWM_ABS_MAX) {
        return -PWM_ABS_MAX;
    } else {
        return pwm;
    }
}

/**
 * @fn      MotorController::
 * @brief   
 */
void MotorController::setPWMs(PWMs *pwms) {
    int8_t left_pwm = pwms->left + left_bias_;
    int8_t right_pwm = pwms->right + right_bias_;
    left_pwm = RoundPWM(left_pwm);
    right_pwm = RoundPWM(right_pwm);
    left_motor_->setPWM(left_pwm);
    right_motor_->setPWM(right_pwm);
}

/**
 * @fn      MotorController::
 * @brief   
 */
void MotorController::Stop() {
    left_motor_->Stop();
    right_motor_->Stop();
}

/**
 * @fn      MotorController::
 * @brief   
 */
void MotorController::setBiases(int8_t left, int8_t right) {
    left_bias_ = left;
    right_bias_ = right;
}