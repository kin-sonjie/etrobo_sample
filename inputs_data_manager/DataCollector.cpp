/**
 * @author  Hiromichi Kojima
 * @date    2022/08/04
 */

#include "DataCollector.h"
#include "Motor.h"
#include "ColorSensor.h"
#include "SonarSensor.h"
#include "GyroSensor.h"
#include "t_syslog.h"

using namespace ev3api;


/**
 * @fn      DataCollector::DataCollector()
 * @brief   コンストラクタ. 各メンバ変数のインスタンス化.
 * @author  Hiromichi Kojima
 * @date    2022/08/04
   @note    各センサとそのポート
            right_motor_(PORT_B),
            left_motor_(PORT_C),
            color_sensor_(PORT_2),
            sonar_sensor_(PORT_3),
            gyro_sensor_(PORT_4)
 */
DataCollector::DataCollector() {
  right_motor_ = new Motor(PORT_B);
  left_motor_ = new Motor(PORT_C);
  color_sensor_ = new ColorSensor(PORT_2);
  sonar_sensor_ = new SonarSensor(PORT_3);
  gyro_sensor_ = new GyroSensor(PORT_4);

  filtering_right_motor_count_ = new Filter<int32_t>();
  filtering_left_motor_count_ = new Filter<int32_t>();
  filtering_brightness_ = new Filter<uint16_t>();
  filtering_raw_r_ = new Filter<uint16_t>();
  filtering_raw_g_ = new Filter<uint16_t>();
  filtering_raw_b_ = new Filter<uint16_t>();
  filtering_distance_ = new Filter<int16_t>();
  filtering_angle = new Filter<int16_t>();
  filtering_angular_velocity_ = new Filter<int16_t>();
}

/**
 * @fn      DataCollector::~DataCollector()
 * @brief   デストラクタ. 各メンバ変数の解放.
 * @author  Hiromichi Kojima
 * @date    2022/08/04
 */
DataCollector::~DataCollector() {
	delete right_motor_;
	delete left_motor_;
	delete color_sensor_;
	delete sonar_sensor_;
	delete gyro_sensor_;

  delete filtering_right_motor_count_;
  delete filtering_left_motor_count_;
  delete filtering_brightness_;
  delete filtering_raw_r_;
  delete filtering_raw_g_;
  delete filtering_raw_b_;
  delete filtering_distance_;
  delete filtering_angle;
  delete filtering_angular_velocity_;
}

/**
 * @fn      DataCollector::UpdateMvAveOfEachSensorValue()
 * @brief   各センサ値の移動合計値を更新.
 * @author  Hiromichi Kojima
 * @date    2022/08/08
 */
void DataCollector::UpdateMvAveOfEachSensorValue() {
  // Motor
  filtering_right_motor_count_->UpdateMovingAverage(right_motor_->getCount());
  filtering_left_motor_count_->UpdateMovingAverage(left_motor_->getCount());
  // Color
  rgb_raw_t current_rgb;
  color_sensor_->getRawColor(current_rgb);
  filtering_brightness_->UpdateMovingAverage(ConvertRGBtoBrightness(current_rgb));
  filtering_raw_r_->UpdateMovingAverage(current_rgb.r);
  filtering_raw_g_->UpdateMovingAverage(current_rgb.g);
  filtering_raw_b_->UpdateMovingAverage(current_rgb.b);

  // Sonar
  /* @note distanceがエラー値(-1)だった場合に, 最大値(200[cm])をフィルタにセットする.
   *      > https://afrel.co.jp/technology-info/spike/hardware/distancesensor/45916/
   */ 
  int16_t current_distance = sonar_sensor_->getDistance();
  if (0 <= current_distance) {
    filtering_distance_->UpdateMovingAverage(current_distance);
  } else {
    filtering_distance_->UpdateMovingAverage(200);
  }
  
  // Gyro
  filtering_angle->UpdateMovingAverage(gyro_sensor_->getAngle());
  filtering_angular_velocity_->UpdateMovingAverage(gyro_sensor_->getAnglerVelocity());
}

/**
 * @fn      DataCollector::GetRightMotorCount()
 * @brief   右モータのエンコーダカウント値の生データを返す.
 * @author  Hiromichi Kojima
 * @date    2022/08/17
 */
int32_t DataCollector::GetRightMotorCount(DataOfType filtering_process) {
  return filtering_right_motor_count_->GetData(filtering_process);
}

/**
 * @fn      DataCollector::GetLeftMotorCount()
 * @brief   左モータのエンコーダカウント値の生データを返す.
 * @author  Hiromichi Kojima
 * @date    2022/08/17
 */
int32_t DataCollector::GetLeftMotorCount(DataOfType filtering_process) {
  return filtering_left_motor_count_->GetData(filtering_process);
}


/**
 * @fn      DataCollector::GetBrightness()
 * @brief   明度(輝度)の生データを返す.
 * @author  Hiromichi Kojima
 * @date    2022/08/17
   @note    `brightness`はRGBから算出する
 */
uint16_t DataCollector::GetBrightness(DataOfType filtering_process) {
  return filtering_brightness_->GetData(filtering_process);
}

/**
 * @fn      DataCollector::GetRGB()
 * @brief   RGB値の生データを返す.
 * @author  Hiromichi Kojima
 * @date    2022/08/17
 */
rgb_raw_t DataCollector::GetRGB(DataOfType filtering_process) {
    rgb_raw_t rgb = {
      filtering_raw_r_->GetData(filtering_process),
      filtering_raw_g_->GetData(filtering_process),
      filtering_raw_b_->GetData(filtering_process),
    };
    return rgb;
}

/**
 * @fn      DataCollector::GetDistance()
 * @brief   正面方向にある物体との距離の生データを返す.
 * @author  Hiromichi Kojima
 * @date    2022/08/17
 */
int16_t DataCollector::GetDistance(DataOfType filtering_process) {
  return filtering_distance_->GetData(filtering_process);
}

/**
 * @fn      DataCollector::GetAngle()
 * @brief   旋回方向の角度の生データを返す.
 * @author  Hiromichi Kojima
 * @date    2022/08/17
 */
int16_t DataCollector::GetAngle(DataOfType filtering_process) {
  return filtering_angle->GetData(filtering_process);
}

/**
 * @fn      DataCollector::GetAnuglarVelocity()
 * @brief   旋回方向の角加速度の生データを返す.
 * @author  Hiromichi Kojima
 * @date    2022/08/17
 */
int16_t DataCollector::GetAnuglarVelocity(DataOfType filtering_process) {
  return filtering_angular_velocity_->GetData(filtering_process);
}

/**
 * @fn      DataCollector::ConvertRGBtoBrightness()
 * @brief   RGBから明度を算出.
 * @retval  0~65535(?)で表現された明度.
 * @author  Hiromichi Kojima
 * @date    2022/08/08
 * @date    2022/08/10 関数名の綴りミスを修正(`~~Brighness` -> `~~Brightness`)
 */
uint16_t DataCollector::ConvertRGBtoBrightness(rgb_raw_t rgb) {
  uint16_t max_element = rgb.r;
  
  if (max_element < rgb.g) {
    max_element = rgb.g;
  }
  if (max_element < rgb.b) {
    max_element = rgb.b;
  }

	return max_element;
}
