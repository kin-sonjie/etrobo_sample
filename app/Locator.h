#pragma once
#include "Motor.h"       // <1>
#include "util.h"        // <3>
#include <cmath>

using namespace ev3api;  // <4>

//#define DEBUG_ENABLE //デバッグ有効化オプション

//位置情報データの型を表現するクラス
class Location {
public:
    Location() {};
    float GetX();
    float GetY();
    float GetTheta();
    void SetLocation(float x, float y, float theta);
private:
    float x_ = 0;           //X座標。初期設定時、自身の右側を正とする。
    float y_ = 0;           //Y座標。初期設定時、自身の前方を正とする。
    float theta_ = M_PI/2;  //偏角。初期設定時、自身の右方を0[rad]とする。
};

//自己位置推定機能を利用するための抽象クラス
class Locator {  // <1>
public:
  Locator();
  void Init();                  //自己の位置、角度を初期化する。
  void UpdateLocation();       //内部保持している自己位置を更新する
  void GetLocation(Location* location); //受け取ったLocation型変数への参照先に対し、保持している現在の座標を格納する。
  void Terminate();             //利用しているリソースをすべて開放し、自己を抹殺する。
private: 
  const float DELTA_T  = 0.02; //制御周期 0.02(20ms)
  const float DIST_PER_COUNT = 100*M_PI/360; //ロータリーエンコーダ1カウント(=1degree)あたりに車輪が進む距離。
  const float RUNNER_WIDTH = 144.0; //走行体の横幅。暫定値
  Motor left_wheel_counter_;
  Motor right_wheel_counter_;
  Location* location_;   //現在位置を内部保持する。
  int32_t right_count_ = 0;
  int32_t left_count_ = 0;
};

