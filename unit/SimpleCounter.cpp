/******************************************************************************
 *  SimpleCounter.cpp (for LEGO Mindstorms EV3)
 *  Created on: 2022/07/19
 *  Implementation of the Class SimpleCounter
 *  Author: Seongjae Kim
 *****************************************************************************/

#include "SimpleCounter.h"

/**
 * コンストラクタ
 * @param leftWheel  左モータ
 * @param rightWheel 右モータ
 */
SimpleCounter::SimpleCounter(ev3api::Motor& leftWheel,
                             ev3api::Motor& rightWheel)
    : mLeftWheel(leftWheel),
      mRightWheel(rightWheel),
      mDelayCountLeft(0),
      mDelayCountRight(0),
      mStartCountLeft(0),
      mStartCountRight(0),
      mTargetCountLeft(0),
      mTargetCountRight(0) {
}

/**
 * カウンタ満期設定
 * @param delay 満期時間[ms]
 */
void SimpleCounter::setCountLeft(int32_t delay) {
    mDelayCountLeft = delay;
}

void SimpleCounter::setCountRight(int32_t delay) {
    mDelayCountRight = delay;
}

/**
 * カウンタ開始
 */
void SimpleCounter::start() {
    mStartCountLeft  = mLeftWheel.getCount();
    mStartCountRight = mRightWheel.getCount();
    mTargetCountLeft = mStartCountLeft + mDelayCountLeft;
    mTargetCountRight = mStartCountRight + mDelayCountRight;
}

/**
 * カウンタ停止
 */
void SimpleCounter::stop() {
    mStartCountLeft = 0;
    mStartCountRight = 0;
    mTargetCountLeft = 0;
    mTargetCountRight = 0;
}

/**
 * カウンタ満期したか否か
 * @retval true  カウンタ満期した
 * @retval false カウンタ満期していない
 */
bool SimpleCounter::isTimedOut() const {
    printf("getCount : %d targetCount : %d\n", mLeftWheel.getCount(), mTargetCountLeft);
    if ((mLeftWheel.getCount() >= mTargetCountLeft) && (mRightWheel.getCount() >= mTargetCountRight)) {
        return true;
    } else {
        return false;
    }
}

/**
 * カウンタ開始したか否か
 * @retval true  カウンタ開始した
 * @retval false カウンタ開始していない
 */
bool SimpleCounter::isStarted() const {
    if ((mStartCountLeft == 0) && (mStartCountRight == 0)) {
        return false;
    } else {
        return true;
    }
}
