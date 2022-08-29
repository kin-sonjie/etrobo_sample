#include "AccelController.h"

/* 定数定義 */
const float AccelController::kDeltaTime_ = 0.01; //実行周期 0.01[sec] (10ms)
const float AccelController::kRateAbsMax_ = 30; //走行体が滑らない最大制御率 [pwm/sec]

/**
 * @fn      AccelController::AccelController
 * @brief   コンストラクタ
 */
AccelController::AccelController(){
    Init();
}

/**
 * @fn      AccelController::Init
 * @brief   メンバ変数初期化
 */
void AccelController::Init() {
    control_val_ = 0.0;
    control_rate_ = 0.0;
}

/**
 * @fn      AccelController::RoundControlRate
 * @brief   -kRateAbsMax_ <= control_rate <= kRateAbsMax_ に丸める
 * @param   control_rate 丸める値
 * @retval  丸めた値
 */
float AccelController::RoundControlRate(float control_rate) {
    if (control_rate > kRateAbsMax_) {
        return kRateAbsMax_;
    } else if (control_rate < -kRateAbsMax_) {
        return -kRateAbsMax_;
    } else {
        return control_rate;
    }
}

/**
 * @fn      AccelController::setControlRate
 * @brief   時間に対する制御量を設定する
 * @param   start_pwm 制御前pwm
 * @param   target_pwm ターゲットpwm
 * @param   time 制御時間[sec]
 */
void AccelController::setControlRate(int8_t start_pwm, int8_t target_pwm, float time) {
    float diff = static_cast<float>(target_pwm - start_pwm);
    if (time == 0.0) {
        control_rate_ = diff / kDeltaTime_;
    } else {
        control_rate_ = diff / time;
        control_rate_ = RoundControlRate(control_rate_);
    }
}

/**
 * @fn      AccelController::getControlVal
 * @brief   制御量取得
 * @retval  制御量
 */
float AccelController::getControlVal() {
    control_val_ += control_rate_ * kDeltaTime_;
    return control_val_;
}