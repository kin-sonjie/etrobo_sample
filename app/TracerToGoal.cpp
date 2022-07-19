/******************************************************************************
 *  TracerToGoal.cpp (for LEGO Mindstorms EV3)
 *  Created on: 2022/07/15
 *  Implementation of the Class TracerToGoal
 *  Author: Songjae Kim
 *****************************************************************************/

#include "TracerToGoal.h"

// 定数定義
const int32_t TracerToGoal::LINE_TRACE_COUNT_1 = 2900;      /* 最初の8の字交差点まで */
const int32_t TracerToGoal::SCENARIO_TRACE_COUNT_1 = 100;  /* 最初の8の字交差点　app.cpp の gScenes の合計値 */
const int32_t TracerToGoal::LINE_TRACE_COUNT_2 = 7200;      /* 8の字片側一週分*/
const int32_t TracerToGoal::SCENARIO_TRACE_COUNT_2 = 100;  /* 次の8の字交差点　app.cpp の gScenes の合計値 */
const int32_t TracerToGoal::LINE_TRACE_COUNT_3 = 15000;     /* 8の字交差点からゴールまで */

const int32_t TracerToGoal::MODE_COUNT_LIST[5] = {LINE_TRACE_COUNT_1, SCENARIO_TRACE_COUNT_1, LINE_TRACE_COUNT_2, SCENARIO_TRACE_COUNT_2, LINE_TRACE_COUNT_3};
const int8_t TracerToGoal::MAX_NUM_MODE = 5;

// 静的変数定義
int8_t TracerToGoal::iMode = 0;

/**
 * コンストラクタ
 * @param lineTracer      ライントレーサ
 * @param scenarioTracer  シナリオトレーサ
 * @param starter         スタータ  
 */
TracerToGoal::TracerToGoal(LineTracer* lineTracer,
                           ScenarioTracer* scenarioTracer,
                           const Starter* starter,
                           SimpleCounter* simpleCounter)
    : mLineTracer(lineTracer),
      mScenarioTracer(scenarioTracer),
      mStarter(starter),
      mSimpleCounter(simpleCounter),
      mState(UNDEFINED) {
}

/**
 * ゴールまで走行する
 */
void TracerToGoal::run() {
    switch (mState) {
    case UNDEFINED:
        execUndefined();
        break;
    case WAITING_FOR_START:
        execWaitingForStart();
        break;
    case LINE_TRACING:
        execLineTracing();
        break;
    case SCENARIO_TRACING:
        execScenarioTracing();
        break;
    default:
        break;
    }
}

/**
 * シーン変更処理
 */
void TracerToGoal::modeChangeAction() {
    if (iMode == MAX_NUM_MODE) {
        mState = UNDEFINED;
        return;
    }
    mSimpleCounter->setCountLeft(MODE_COUNT_LIST[iMode]);
    mSimpleCounter->start();
    iMode++;
}

/**
 * 未定義状態の処理
 */
void TracerToGoal::execUndefined() {
    mState = WAITING_FOR_START;
}

/**
 * 開始待ち状態の処理
 */
void TracerToGoal::execWaitingForStart() {
    if (mStarter->isPushed()) {
        mState = LINE_TRACING;

        modeChangeAction();
    }
}

/**
 * ライントレース状態の処理
 */
void TracerToGoal::execLineTracing() {
    mLineTracer->run();

    if (mSimpleCounter->isTimedOut()) {
        mSimpleCounter->stop();

        mState = SCENARIO_TRACING;

        modeChangeAction();
    }
}

/**
 * シナリオトレース状態の処理
 */
void TracerToGoal::execScenarioTracing() {
    mScenarioTracer->run();

    printf("TracerToGoal\n");
    if (mSimpleCounter->isTimedOut()) {
        mSimpleCounter->stop();

        mState = LINE_TRACING;

        modeChangeAction();
    }
}
