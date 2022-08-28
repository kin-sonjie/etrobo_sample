#include "Tracer.h"
#include "Clock.h"
#include "stdio.h"

Tracer::Tracer():
  rightWheel(PORT_B),
  leftWheel(PORT_C),
  colorSensor(PORT_2),
  dataCollector() {
  /* None */
}

/**
 * @fn    Tracer::init()
 * @brief Tracerの初期化関数.
 */
void Tracer::init() {
  init_f("Tracer");
}

/**
 * @fn    Tracer::terminate()
 * @brief インスタンス変数leftMotorとrightMotorを停止する終了関数.
 */
void Tracer::terminate() {
  msg_f("Stopped.", 1);
  leftWheel.stop();
  rightWheel.stop();
}

/**
 * @fn    Tracer::run()
 * @brief pass...
 */
void Tracer::run() {
  msg_f("running...", 1);
  dataCollector.UpdateMvAveOfEachSensorValue();

  float turn = calc_prop_value();
  // int motorCount = leftWheel.getCount();

  // if ((motorCount <= 3600) || (motorCount >= 7300)) {
    leftWheel.setPWM(pwm - turn);
    rightWheel.setPWM(pwm + turn);
  // } else {
    // leftWheel.setPWM(pwm + turn);
    // rightWheel.setPWM(pwm - turn);
  // }
}

float Tracer::calc_prop_value() {
  const float Kp = 0.83;
  // const int target = 45;
  const int target = 110;
  const int bias = 0;

  uint8_t brightness = dataCollector.GetBrightness(LPF_FILTERED_DATA);
  int diff = brightness - target;

  return (Kp * diff + bias);
}

void Tracer::caribration(){
  int ref_val = colorSensor.getBrightness(); 
  int ref_max = ref_val;
  int ref_min = ref_val;
  int start_motorcount = leftWheel.getCount();  //測定開始時にのエンコーダ値
  int end_motorcount = start_motorcount + 300;  //測定終了時のエンコーダ値
  while( end_motorcount > leftWheel.getCount()){
    leftWheel.setPWM(30);
    rightWheel.setPWM(30);
    ref_val = colorSensor.getBrightness();
    if(ref_max < ref_val){ ref_max = ref_val; } //max更新
    if(ref_min > ref_val){ ref_min = ref_val; } //min更新
    clock.sleep(1000);
  }
  terminate();
  syslog(LOG_NOTICE,"ref_max=%d ref_min=%d/n",ref_max,ref_min);
  info.black_val = ref_min;
  info.white_val = ref_max;
  info.grey_val = (ref_min + ref_max) / 2;
  syslog(LOG_NOTICE,"black_val=%d white_val=%d grey_val=%d",info.black_val,info.white_val,info.grey_val);
}
