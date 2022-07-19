/******************************************************************************
 *  SimpleCounter.cpp (for LEGO Mindstorms EV3)
 *  Created on: 2022/07/19
 *  Implementation of the Class SimpleCounter
 *  Author: Seongjae Kim
 *****************************************************************************/

#ifndef EV3_UNIT_SIMPLECOUNTER_H_
#define EV3_UNIT_SIMPLECOUNTER_H_

#include "Motor.h"

class SimpleCounter {
public:
    explicit SimpleCounter(ev3api::Motor& leftWheel,
                           ev3api::Motor& rightWheel);

    int32_t getCountLeft();
    int32_t getCountRight();
    void setCountLeft(int32_t count);
    void setCountRight(int32_t count);
    void start();
    void stop();
    bool isTimedOut() const;
    bool isStarted() const;

private:
    ev3api::Motor& mLeftWheel;
    ev3api::Motor& mRightWheel;
    int32_t mDelayCountLeft;
    int32_t mDelayCountRight;
    int32_t mStartCountLeft;
    int32_t mStartCountRight;
    int32_t mTargetCountLeft;
    int32_t mTargetCountRight;
};

#endif  // EV3_UNIT_SIMPLECOUNTER_H_
