#pragma once

#include "Driver.h"

class NaiveStraightDriver : public Driver {
private:
    void RunWithAMethod(int8_t pwm);
};

class NaiveCurveDriver : public Driver {
private:
    void RunWithAMethod(int8_t pwm);
};

class NaiveRotateDriver : public Driver {
private:
    void RunWithAMethod(int8_t pwm);
};

/*
class TraceDriver : public Driver {
private:
    void Run(int8_t pwm);
};
*/
