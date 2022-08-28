#pragma once
#include "Clock.h"
#include "Motor.h"
#include "ColorSensor.h"
#include "DataLogger.h"
#include "DataCollector.h"
#include "util.h"
#include "Info.h"

using namespace ev3api;

class Tracer {
public:
  Tracer();
  void run();
  void init();
  void terminate();
  void caribration();
private:
  Motor leftWheel;
  Motor rightWheel;
  ColorSensor colorSensor;
  DataCollector dataCollector;
  Clock clock;
  Info info;
  const int8_t mThreshold = 20;
#ifndef MAKE_RASPIKE
  const int8_t pwm = (Motor::PWM_MAX) / 6;
#else
  const int8_t pwm = 60;
#endif

  float calc_prop_value();
};