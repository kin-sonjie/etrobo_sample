/******************************************************************************
 *  LineControllerPID.cpp (for LEGO Mindstorms EV3)
 *  Created on: 2022/07/15
 *  Implementation of the Class LineControllerPID
 *  Author: Seongjae Kim
 *****************************************************************************/

#include "stdio.h"
#include "LineControllerPID.h"

 // 定数定義
const float LineControllerPID::KP = 4;
const float LineControllerPID::KI = 0.1;
const float LineControllerPID::KD = 0.05;
const int8_t LineControllerPID::TERGET_VAL = 20;  // 光センサ目標値

//静的変数定義
uint64_t LineControllerPID::time[2] = {0, 0};
int16_t LineControllerPID::diff[2] = {0, 0};
float LineControllerPID::integral = 0;

/**
 * コンストラクタ
 * @param colorSensor カラーセンサ
 * @param clock クロック
 */
LineControllerPID::LineControllerPID(const ev3api::ColorSensor& colorSensor, const ev3api::Clock& clock)
    : mColorSensor(colorSensor),
      mClock(clock) {
}

/**
 * 制御量を計算する
 * @retval 制御量
 */
float LineControllerPID::calcPID() {
    time[0] = time[1];
    time[1] = mClock.now();
    uint32_t timeDelta = (time[1] - time[0]) + 1; // about 10000 micro seconds
    
    diff[0] = diff[1];
    diff[1] = mColorSensor.getBrightness() - TERGET_VAL;
    integral += (diff[1] + diff[0]) / 2.0 * (static_cast<float>(timeDelta) / 1000000);

    float p = KP * diff[1];
    float i = KI * integral;
    float d = KD * (diff[1] - diff[0]) / (static_cast<float>(timeDelta) / 1000000);

    //printf("%f %f %f %f\n", p, i, d, p+i+d);
    return p + i + d;
}
