/******************************************************************************
 *  TracerToGoal.h (for LEGO Mindstorms EV3)
 *  Created on: 2022/07/15
 *  Implementation of the Class TracerToGoal
 *  Author: Seongjae Kim
 *****************************************************************************/

#ifndef EV3_APP_TracerToGoal_H_
#define EV3_APP_TracerToGoal_H_

#include "Starter.h"
#include "SimpleCounter.h"
#include "LineTracer.h"
#include "ScenarioTracer.h"

class TracerToGoal {
public:
    TracerToGoal(LineTracer* lineTracer,
                 ScenarioTracer* scenarioTracer,
                 const Starter* starter,
                 SimpleCounter* simpleCounter);

    void run();

private:
    enum State {
        UNDEFINED,
        WAITING_FOR_START,
        LINE_TRACING,
        SCENARIO_TRACING
    };

    static const int32_t LINE_TRACE_COUNT_1;
    static const int32_t SCENARIO_TRACE_COUNT_1;
    static const int32_t LINE_TRACE_COUNT_2;
    static const int32_t SCENARIO_TRACE_COUNT_2;
    static const int32_t LINE_TRACE_COUNT_3;
    static const int32_t MODE_COUNT_LIST[5];
    static const int8_t MAX_NUM_MODE;
    
    static int8_t iMode;

    LineTracer* mLineTracer;
    ScenarioTracer* mScenarioTracer;
    const Starter* mStarter;
    SimpleCounter* mSimpleCounter;
    State mState;

    void modeChangeAction();
    void execUndefined();
    void execWaitingForStart();
    void execLineTracing();
    void execScenarioTracing();
};

#endif  // EV3_APP_TRACERTOGOAL_H_
