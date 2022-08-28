/**
 * @file    Filter.cpp
 * @brief   フィルタ処理(移動平均)を担うクラスの実装部
 * @author  Hiromichi Kojima
 * @date    2022/08/04
   @note  テンプレートクラスの明示的インスタンス生成(リンカエラー回避)
 *        > [明示的なインスタンス化の注意]
 *          (https://pknight.hatenablog.com/entry/20100316/1268750644)
 *        > [C++ テンプレート　ヘッダと実装を別にした時に起こること]
 *          (https://qiita.com/i153/items/38f9688a9c80b2cb7da7)
 */

#include "Filter.h"
#include "t_syslog.h"


// @note 明示的インスタンス化
template class Filter<int32_t>;
template class Filter<int16_t>;
template class Filter<uint16_t>;

/**
 * @fn      Filter<SensorValue>::Filter()
 * @brief   コンストラクタ. 各メンバ変数の初期化
 * @author  Hiromichi Kojima
 * @date    2022/08/04
 * @date    2022/08/09  レビュー1対応(変数名の更新)
 * @date    2022/08/10  `moving_ave_`, `moving_array_`がローカル宣言になっていたのをメンバ初期化に改修.
 * @date    2022/08/10  `values_array_`の初期化方法を修正(実機でのエラー回避のため).
 * @date    2022/08/17  クラス名を`Filter`に変更
 */
template <class SensorValue>
Filter<SensorValue>::Filter(){
  int init_array_index = 0;

  for (init_array_index = 0; init_array_index < kWindowSize; init_array_index++) {
    values_array_[init_array_index] = 0;
  }
  moving_ave_ = 0;
  oldest_value_index_ = 0;
}

/**
 * @fn      Filter<SensorValue>::UpdateMovingAverage()
 * @brief   最新値を受け取り移動合計値を更新するメソッド.
 * @param   latest_value 最新値
 * @author  Hiromichi Kojima
 * @date    2022/08/04
 * @date    2022/08/09  レビュー1対応(変数名の更新・`oldest_value_index_`の操作隠蔽).
 * @date    2022/08/10  kWindowSizeで割った値を配列に格納することで, signed/unsignedの混在を解消.
 */
template <class SensorValue>
void Filter<SensorValue>::UpdateMovingAverage(SensorValue latest_value) {
  /* 移動平均値の更新
   *    1. 移動平均値を最古要素をkWindowSizeで割った値で減算
   *    2. 同位置に最新値をkWindowSizeで割った値をセット 
   *    3. 移動平均値に最新値をkWindowSizeで割った値を加算
   *    4. 最古要素が格納されていた位置に最新値を格納.
   */
  int8_t target_older_value_index = NextOldestValueIndex();

  SensorValue div_oldest_value = static_cast<SensorValue>(values_array_[target_older_value_index] / kWindowSize); // 最古値の抽出
  SensorValue div_latest_value = static_cast<SensorValue>(latest_value / kWindowSize);

  moving_ave_ = moving_ave_ - div_oldest_value + div_latest_value;
  values_array_[target_older_value_index] = latest_value;
}

/**
 * @fn      Filter<SensorValue>::GetMovingAverage()
 * @brief   移動合計値 / ウィンドウサイズ, すなわち移動平均値を返すメソッド.
 * @author  Hiromichi Kojima
 * @date    2022/08/08
 */
template <class SensorValue>
SensorValue Filter<SensorValue>::GetMovingAverage() {
  return moving_ave_;
}

/**
 * @fn      Filter<SensorValue>::GetRawData()
 * @brief   最新値の生データを返すメソッド
 * @author  Hiromichi Kojima
 * @date    2022/08/09  レビュー1対応(oldest_value_index_の隠蔽・生データ取得機能).
 */
template <class SensorValue>
SensorValue Filter<SensorValue>::GetRawData() {
  int8_t latest_index = LatestValueIndex();
  return values_array_[latest_index];
}

/*
 * @fn      Filter::NextOldestValueIndex()
 * @brief   次の最古値のインデックスを返す関数
 * @retval  0 ~ (kWindowSize-1)の値.
 * @author  Hiromichi Kojima
 * @date    2022/08/09  レビュー1対応(oldest_value_index_の隠蔽).
 */
template <class SensorValue>
int8_t Filter<SensorValue>::NextOldestValueIndex() {
  oldest_value_index_++;

  if (oldest_value_index_ >= kWindowSize) {
    oldest_value_index_ = 0;
  }
  return oldest_value_index_;
}

/**
 * @fn      Filter::LatestValueIndex()
 * @brief   最新値のインデックスを返す変数.
 * @retval  0 ~ (kWindowSize-1)の値.
 * @author  Hiromichi Kojima
 * @date    2022/08/09  レビュー1対応(oldest_value_index_の隠蔽・生データ取得機能).
 */
template <class SensorValue>
int8_t Filter<SensorValue>::LatestValueIndex() {
  int8_t latest_value_index = oldest_value_index_;
  latest_value_index--;

  if (latest_value_index <= 0) {
    latest_value_index = kWindowSize;
  }
  return latest_value_index;
}

/**
 * @fn      Filter::GetData()
 * @brief   最新値のインデックスを返す変数.
 * @retval  0 ~ (kWindowSize-1)の値.
 * @author  Hiromichi Kojima
 * @date    2022/08/17  レビュー2対応(データのRaw/Filtedをテンプレート化).
 */
template <class SensorValue>
SensorValue Filter<SensorValue>::GetData(DataOfType filtering_process) {
  switch (filtering_process) {
    case RAW_DATA:
      return GetRawData();
    case LPF_FILTERED_DATA:
      return GetMovingAverage();
    default:
      return 0;
  }
}
