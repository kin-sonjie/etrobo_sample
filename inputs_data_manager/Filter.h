/**
 * @file    Filter.h
 * @brief   フィルタ処理(移動平均)を担うクラスの実装部
 * @author  Hiromichi Kojima
 * @date    2022/08/04
 */
#pragma once

#include <inttypes.h>

/**
 * @enum DataOfType
 * データを取得する際の Raw/Filtered を区別する列挙体
 * @var DataOfType::RAW_DATA
 * 生データの取得時に使用
 * @var DataOfType::LPF_FILTERED_DATA
 * LPF(移動平均)でフィルタリングされた値の取得時に使用
 */
typedef enum {
  RAW_DATA,
  LPF_FILTERED_DATA,
} DataOfType;

template <class SensorValue>
class Filter {
public:
  Filter();

  void UpdateMovingAverage(SensorValue latest_value);
  SensorValue GetData(DataOfType filtering_process);
private:
  SensorValue GetMovingAverage();
  SensorValue GetRawData();
  int8_t NextOldestValueIndex();
  int8_t LatestValueIndex();

  static const int8_t kWindowSize = 3;

  SensorValue values_array_[kWindowSize];
  SensorValue moving_ave_;
  int8_t oldest_value_index_;
};
