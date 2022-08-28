#pragma once

#include <stdio.h>
#include "DataCollector.h"

/* @note ログファイル出力について
 *       ログファイル名の重複を無くすために, ファイル名を
 *       "[YYYMMDD_HHMMSS]_log.csv"にしたいのだが,
 *       RTOSでUNIX時間の取得が難しそうなので一旦リダイレクトで行うことにした.
 *
 *       - リダイレクトでのログファイル出力方法
 *       (sim)  $ make app=Genkai2022 sim
 *              $ asp stdout launch > `date "+%Y%m%d_%H%M%S_log.csv"`
 *       (real) $ make img=Genkai
 *              $ make start > `date "+%Y%m%d_%H%M%S_log.csv"`
 */

/* コンパイルスイッチ */
#define IS_LOGGER_ENABLED //ロガー有効化マクロ。定義されているときはログ機能が動作する。
#define IS_RUNNER_ENABLED //走行機能有効化マクロ。定義されているときのみ走行機能が動作する。

class DataLogger {
public:
  DataLogger();
  ~DataLogger();
  void Init();
  void Terminate();
  void PrettyPrint();
  void UpdateEachValues();
  void OutputConsole(const char *text);

private:
  void UpdateLogString();

  // センサ値取得用
  DataCollector data_controller_;
  
  // 値保持用
  // uint64_t current_time_;       // タイマ
  int32_t right_motor_count_;   // モータ
  int32_t left_motor_count_;    // モータ
  uint16_t brightness_;           // カラーセンサ
  rgb_raw_t raw_color_;         // カラーセンサ
  int16_t distance_;            // ソナーセンサ
  int16_t angular_velocity_;    // ジャイロセンサ
  int16_t angle_;               // ジャイロセンサ

  // 出力用文字列バッファ
  const uint8_t kBuffLogMaxSize_ = 255;
  char buff_log_[255];
  // ログファイルのヘッダー文字列
  const char *kHeaderString_ = "right_motor_count, left_motor_count, brightness, raw_color_r, raw_color_g, raw_color_b, distance, angular_velocity, angle";
};
