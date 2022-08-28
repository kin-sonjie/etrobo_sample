/**
 * @file    DataCollector.h
 * @brief   各入力デバイスのセンサ値のフィルタリングを行うヘッダ部
 * @author  Hiromichi Kojima
 * @date    2022/08/04
 */
#pragma once

#include "Motor.h"
#include "ColorSensor.h"
#include "SonarSensor.h"
#include "GyroSensor.h"

#include "Filter.h"

using namespace ev3api;

class DataCollector {
public:
  DataCollector();
  ~DataCollector();
  void UpdateMvAveOfEachSensorValue();

  int32_t GetRightMotorCount(DataOfType filtering_process);
  int32_t GetLeftMotorCount(DataOfType filtering_process);
  uint16_t GetBrightness(DataOfType filtering_process);
  rgb_raw_t GetRGB(DataOfType filtering_process);
  int16_t GetDistance(DataOfType filtering_process);
  int16_t GetAngle(DataOfType filtering_process);
  int16_t GetAnuglarVelocity(DataOfType filtering_process);

private:
  uint16_t ConvertRGBtoBrightness(rgb_raw_t rgb);

  Motor *right_motor_;
  Motor *left_motor_;
  ColorSensor *color_sensor_;
  SonarSensor *sonar_sensor_;
  GyroSensor *gyro_sensor_;

  Filter<int32_t> *filtering_right_motor_count_;
  Filter<int32_t> *filtering_left_motor_count_;
  Filter<uint16_t> *filtering_brightness_;
  Filter<uint16_t> *filtering_raw_r_;
  Filter<uint16_t> *filtering_raw_g_;
  Filter<uint16_t> *filtering_raw_b_;
  Filter<int16_t> *filtering_distance_;
  Filter<int16_t> *filtering_angle;
  Filter<int16_t> *filtering_angular_velocity_;
};
