#include "MotorController.h"

/* 静的変数定義 */
int8_t MotorController::left_bias_ = 0;
int8_t MotorController::right_bias_ = 0;

/**
 * @fn      MotorController::MotorController
 * @brief   コンストラクタ
 */
MotorController::MotorController() {
    left_motor_ = new Motor(PORT_C);
    right_motor_ = new Motor(PORT_B);
}

/**
 * @fn      MotorController::~MotorController
 * @brief   デストラクタ
 */
MotorController::~MotorController() {
    delete left_motor_;
    delete right_motor_;
}

/**
 * @fn      MotorController::setBiases
 * @brief   バイアスを設定する
 * @param   left 左バイアス
 * @param   right 右バイアス
 */
void MotorController::setBiases(int8_t left, int8_t right) {
    left_bias_ = left;
    right_bias_ = right;
}

/**
 * @fn      MotorController::RoundPWM
 * @brief   -PWM_ABS_MAX <= pwm <= PWM_ABS_MAX に丸める
 * @param   pwm 丸める値
 * @retval  丸めた値
 */
int8_t MotorController::RoundPWM(int8_t pwm) {
    if (pwm > PWM_ABS_MAX) {
        return PWM_ABS_MAX;
    } else if (pwm < -PWM_ABS_MAX) {
        return -PWM_ABS_MAX;
    } else {
        return pwm;
    }
}

/**
 * @fn      MotorController::setPWMs
 * @brief   左右PWM値を設定する
 * @param   pwms 設定値へのポインタ
 */
void MotorController::setPWMs(int8_t left, int8_t right) {
    int8_t left_pwm = pwms->left + left_bias_;
    int8_t right_pwm = pwms->right + right_bias_;
    left_pwm = RoundPWM(left_pwm);
    right_pwm = RoundPWM(right_pwm);
    left_motor_->setPWM(left_pwm);
    right_motor_->setPWM(right_pwm);
}

/**
 * @fn      MotorController::Stop
 * @brief   モータを停止させる
 */
void MotorController::Stop() {
    left_motor_->Stop();
    right_motor_->Stop();
}
