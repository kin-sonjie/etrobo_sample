/******************************************************************************
 *  LineTracer.h (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/25
 *  Definition of the Class LineTracer
 *  Author: Kazuhiro Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#ifndef EV3_APP_LINETRACER_H_
#define EV3_APP_LINETRACER_H_

#include "LineControllerPID.h"
#include "Walker.h"

class LineTracer {
public:
    LineTracer(LineControllerPID* lineControllerPID,
               Walker* walker);

    void run();

private:
    LineControllerPID* mLineControllerPID;
    Walker* mWalker;
    bool mIsInitialized;
};

#endif  // EV3_APP_LINETRACER_H_
