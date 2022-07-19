/******************************************************************************
 *  LineControllerPID.h (for LEGO Mindstorms EV3)
 *  Created on: 2022/07/15
 *  Implementation of the Class LineControllerPID
 *  Author: Seongjae Kim
 *****************************************************************************/

#ifndef EV3_UNIT_LINECONTROLLERPID_H_
#define EV3_UNIT_LINECONTROLLERPID_H_

#include "ColorSensor.h"
#include "Clock.h"

// 定義
class LineControllerPID {
public:
    explicit LineControllerPID(const ev3api::ColorSensor& colorSensor, const ev3api::Clock& clock);

    bool isOnLine() const;
    float calcPID();

private:
    static const float KP;
    static const float KI;
    static const float KD;
    static const int8_t TERGET_VAL;

    static uint64_t time[2];
    static int16_t diff[2];
    static float integral;
    static bool isInitializedPID;

    const ev3api::ColorSensor& mColorSensor;
    const ev3api::Clock& mClock;

};

#endif //EV3_UNIT_LINECONTROLLERPID_H_