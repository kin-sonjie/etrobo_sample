#pragma once

#include "ev3api.h"

enum RunningMethod {
    NAIVE_STRAIGHT,
    NAIVE_CURVE,
    NAIVE_ROTATE,
    //TRACE,
    //VIRTUAL_STRAIGHT,
    //VIRTUAL_CURVE,
    UNDEFINED
};

struct DriveInfo {
    RunningMethod method;
    int8_t target_pwm;
    float drive_time; //運転時間(全体)[sec]
    float accel_time; //加速(のみ)時間[sec]
    void *detail_info;
};

enum Direction {
    LEFT,
    RIGHT
};

/*
struct NaiveStraightInfo {
    詳細情報なし
};
*/

struct NaiveCurveInfo {
    Direction direction;
    float high_rate; //ベースPWMに対する高出力側の比率
    float low_rate; //ベースPWMに対する低出力側の比率
};

struct NaiveRotateInfo {
    Direction direction;
};

/*
struct TraceInfo {
    KPID kpid;
};
*/

class DriveInfoTable {
public:
    DriveInfo *getDriveInfo(uint8_t index);
private:
    static const DriveInfo drive_info_tbl[];
    static const NaiveCurveInfo naive_curve_info_tbl[];
    static const NaiveRotateInfo naive_rotate_info_tbl[];
};
