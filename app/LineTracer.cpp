/******************************************************************************
 *  LineTracer.cpp (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/26
 *  Implementation of the Class LineTracer
 *  Author: Kazuhiro Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#include "LineTracer.h"

/**
 * コンストラクタ
 * @param lineControllerPID    ライン制御
 * @param walker 走行
 */
LineTracer::LineTracer(LineControllerPID* lineControllerPID,
                       Walker* walker)
    : mLineControllerPID(lineControllerPID),
      mWalker(walker),
      mIsInitialized(false) {
}

/**
 * ライントレースする
 */
void LineTracer::run() {
    if (mIsInitialized == false) {
        mWalker->init();
        mIsInitialized = true;
    }

    mWalker->setCommand(Walker::NORMAL, -1);

    // PID制御
    int controlValue = mLineControllerPID->calcPID();

    // 走行を行う
    mWalker->run(controlValue);
}
