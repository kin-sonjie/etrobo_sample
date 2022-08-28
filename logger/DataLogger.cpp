#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include "DataLogger.h"
#include "Filter.h"
#include "DataCollector.h"

/**
 * @fn    DataLogger::DataLogger()
 * @brief コンストラクタ. 処理なし.
 */
DataLogger::DataLogger():
    data_controller_(){
  /* None */
}

/**
 * @fn    DataLogger::~DataLogger()
 * @brief デストラクタ. 処理なし.
  */
DataLogger::~DataLogger() {
  /* None */
}

void DataLogger::Init() {
  // ヘッダー文字列の出力
  OutputConsole(kHeaderString_);

  // current_time_ = 0;
  right_motor_count_ = 0;
  left_motor_count_ = 0;
  brightness_ = 0;
  raw_color_.r = 0;
  raw_color_.g = 0;
  raw_color_.b = 0;
  distance_ = 0;
  angular_velocity_ = 0;
  angle_ = 0;
}

/**
 * @fn    DataLogger::Terminate()
 * @brief
 */
void DataLogger::Terminate() {
  //nop
}

/**
 * @fn    DataLogger::PrettyPrint()
 * @brief 各センサ値を更新し, コンソールに出力するメソッド.
 */
void DataLogger::PrettyPrint() {
  UpdateLogString();
  OutputConsole(buff_log_);
}

/**
 * @fn    DataLogger::UpdateEachValues()
 * @brief 保持している各センサ値を更新.
 */
void DataLogger::UpdateEachValues() {
  /* @note カラーセンサについて
   *       > カラーセンサーのモードを切り替える際には、
   *       > 一定の時間(数十〜100msec程度)かかりますので、
   *       > 頻繁に切り替えることは避けた方が良いでしょう。
   *       とRasPikeにおけるEV3RTのAPI対応に記述あり.
   */
  // 各値の更新
  data_controller_.UpdateMvAveOfEachSensorValue();
  
  // 各値の移動平均値取得
  right_motor_count_ = data_controller_.GetRightMotorCount(RAW_DATA);
  left_motor_count_ = data_controller_.GetLeftMotorCount(RAW_DATA);
  brightness_ = data_controller_.GetBrightness(RAW_DATA);
  raw_color_ = data_controller_.GetRGB(RAW_DATA);
  distance_ = data_controller_.GetDistance(RAW_DATA);
  angular_velocity_ = data_controller_.GetAnuglarVelocity(RAW_DATA);
  angle_ = data_controller_.GetAngle(RAW_DATA);
}

/**
 * @fn    DataLogger::OutputConsole()
 * @brief コンソールに, ログ出力用文字列を出力する.
 * @param text 出力対象の文字列.
 */
void DataLogger::OutputConsole(const char *text) {
  /* @note LOGのレベルについて.
   *       実機実行時, ログレベル"LOG_INFO"の場合,
   *       標準出力に出力されないので, "LOG_NOTICE"を使用する.
   */
  if (strlen(text) < kBuffLogMaxSize_) {
    // コンソールに出力
    syslog(LOG_NOTICE, text);
  }
}

/**
 * @fn    DataLogger::updateLogString()
 * @brief ログ出力用文字列の更新.
 * @note  private.
 */
void DataLogger::UpdateLogString() {
  /* @bug unsigned long long intの変換指定子の挙動がおかしい(シミュレータ実行時).
   *      %llu および "%" PRIu64 "%" でuint64_tの変換指定をした際,
   *      %llu および "%" PRIu64 "%" の部分は問題なく動作するが,
   *      %llu および "%" PRIu64 "%" の1つ後ろの変換指定子の部分に
   *      `0`が入ってしまうことを確認した. 実機では確認されていない.
   *      ex1)
   *        sprintf(buffLog, "%llu, %ld", time, rightMotorCount);
   *      >>> 1234, 0
   *      ex2)
   *        sprintf(buffLog, "%llu, %ld, %ld", time, rightMotorCount);
   *        >>> 1234, 0, 56
   */

  /* @note `int32_t`の変換指定子について.
   *       (sim)`aka long int`なので %ld で変換指定.
   *       (real)`aka int`なので %d で変換指定.
   */
  snprintf(
    buff_log_,
    sizeof(buff_log_),
    "%d, %d, %u, %u, %u, %u, %d, %d, %d",
    // current_time_,
    right_motor_count_,
    left_motor_count_,
    brightness_,
    raw_color_.r,
    raw_color_.g,
    raw_color_.b,
    distance_,
    angular_velocity_,
    angle_
  );
}
