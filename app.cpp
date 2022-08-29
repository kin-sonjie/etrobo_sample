#include "app.h"
#include "DriveManager.h"
#include "DriveInfoTable.h"
#include "DataLogger.h"
#include "Clock.h"
#include "Locator.h"

using namespace ev3api;

DriveManager drive_manager;
DriveInfoTable drive_info_table;
Clock clock;
DataLogger datalogger;
Locator locator;



void drive_manager_task(intptr_t exinf) { // <1>
  drive_manager.RunByDriver(); // <2>
  ext_tsk();
}

void datalogger_task(intptr_t exinf) {
#ifdef IS_LOGGER_ENABLED
#ifdef IS_RUNNER_ENABLED
  datalogger.UpdateEachValues();
  datalogger.PrettyPrint();
#else //走行機能無効(手動センサー値取得用)のとき、左ボタンによりログ出力の有/無を切り替える
  if (ev3_button_is_pressed(RIGHT_BUTTON)) {
    datalogger.UpdateEachValues();
    datalogger.PrettyPrint();
  }
#endif
#endif

  ext_tsk();
}

void locator_task(intptr_t exinf) { // <1>
  locator.UpdateLocation();

  Location* location = new Location();
  locator.GetLocation(location);
  ext_tsk();
}

void main_task(intptr_t unused) { // <1>
  const uint32_t duration = 100*1000; // <2>

#ifdef IS_RUNNER_ENABLED

  //動作終了のための左ボタン押下待ちループ
  while (!ev3_button_is_pressed(LEFT_BUTTON)) { // <1>
      clock.sleep(duration);   // <2>
  }

  //キャリブレーション開始(自動で一定距離走行し、輝度センサ値を補正する)
  //tracer.caribration();

  //通常走行開始のための右ボタン押下待ちループ
  //走行開始前に、走行体位置を手動でリセットする必要がある。
  while (!ev3_button_is_pressed(RIGHT_BUTTON)) { // <1>
      clock.sleep(duration);   // <2>
  }

  drive_manager.Init();
  drive_manager.setDriveInfo(drive_info_table.getDriveInfo(0));
  datalogger.Init();
  locator.Init();
  sta_cyc(DRIVE_MANAGER_CYC);
  sta_cyc(DATALOGGER_CYC);
  sta_cyc(LOCATOR_CYC);

  //動作終了のための左ボタン押下待ちループ
  while (!ev3_button_is_pressed(LEFT_BUTTON)) {
      clock.sleep(duration);
  }

  stp_cyc(DRIVE_MANAGER_CYC);
  sta_cyc(DATALOGGER_CYC);
  stp_cyc(LOCATOR_CYC);

  drive_manager.Terminate();
  datalogger.Terminate();
  locator.Terminate();
  syslog(LOG_NOTICE,"maintask_end");
  ext_tsk();

// 走行機能無効用の暫定実装。ロガーの動作切り替えに左右ボタン必要なため、ここではボタン利用できない。
#else
  drive_manager.Init();
  datalogger.Init();

  sta_cyc(DATALOGGER_CYC);
  
  //動作終了のトリガを走行体に与えられないため、終了処理を省略する。
#endif

}
