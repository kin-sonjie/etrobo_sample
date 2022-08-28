#include "Locator.h"
#include <stdio.h>

//################ Locator型 メソッド定義 ################

Locator::Locator():left_wheel_counter_(PORT_C), right_wheel_counter_(PORT_B) {}

void Locator::Init() {
  right_wheel_counter_.reset();
  left_wheel_counter_.reset();
  location_ = new Location();
}

void Locator::Terminate() {
  if(location_!=NULL) delete location_;
}

void Locator::UpdateLocation() {

  int32_t right_count_latest = right_wheel_counter_.getCount();
  int32_t left_count_latest = left_wheel_counter_.getCount();

  float delta_rWheel = (right_count_latest - right_count_)*DIST_PER_COUNT; //右車輪の移動距離(mm)
  float delta_lWheel = (left_count_latest - left_count_)*DIST_PER_COUNT; //左車輪の移動距離(mm)
  float delta_theta  = (delta_rWheel-delta_lWheel)/RUNNER_WIDTH;  //車体の角度変化

  //車体の移動距離は左右輪の移動距離の平均で近似する。
  float delta_body   = (delta_rWheel + delta_lWheel)/2.0;       //車体の移動距離(mm)

  //delta_theta(車体の角度変化)が大きい場合、より正確な移動距離を算出する。
  if(fabs(delta_theta) > M_PI*10.0/180.0) { //ここでは10[degree]以上の角度変化があった場合とする
    delta_body = 2.0*(delta_body/delta_theta)*std::sin(delta_theta/2.0);       //車体の移動距離(mm)
  }

  //現在の走行体のx座標, y座標, 角度を更新する。
  float theta_latest = location_->GetTheta() + delta_theta;
  float x_latest     = location_->GetX()     + delta_body*std::cos(theta_latest+delta_theta/2.0);
  float y_latest     = location_->GetY()     + delta_body*std::sin(theta_latest+delta_theta/2.0);
  location_->SetLocation(x_latest , y_latest, theta_latest);

  //ロータリーエンコーダのカウント値を更新する。
  right_count_ = right_count_latest;
  left_count_ = left_count_latest;
}

void Locator::GetLocation(Location *location) {
  float x     = location_->GetX();
  float y     = location_->GetY();
  float theta = location_->GetTheta();
  location->SetLocation(x, y, theta);

#ifdef DEBUG_ENABLE
  char buff_log[128] = {0};
  sprintf(buff_log, "(x, y, theta) = %5.2f %5.2f %5.2f\n", x,y,theta);
  printf(buff_log);
#endif
}


//################ Location型 メソッド定義 ################
float Location::GetX() {
    return x_;
}

float Location::GetY() {
    return y_;
}

float Location::GetTheta() {
    return theta_;
}

void Location::SetLocation(float x, float y, float theta) {
    x_ = x;
    y_ = y;
    theta_ = theta;
}