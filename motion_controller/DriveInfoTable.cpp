#include "DriveInfoTable.h"

const NaiveCurveInfo DriveInfoTable::naive_curve_info_tbl[] = {
    { LEFT, 1.2, 0.8 },
    { RIGHT, 1.3, 0.7 },
};

const NaiveRotateInfo DriveInfoTable::naive_rotate_info_tbl[] = {
    { LEFT },
    { RIGHT },
};

const DriveInfo DriveInfoTable::drive_info_tbl[] = {
    {
        NAIVE_STRAIGHT,
        50,
        5.0,
        1.0,
        NULL
    },
    {
        NAIVE_CURVE,
        -60,
        5.0,
        1.0,
        &naive_curve_info_tbl[0]
    },
    {
        .method = NAIVE_ROTATE,
        .target_pwm = 60,
        .drive_time = 5.0,
        .accel_time = 1.0,
        .detail_info = &naive_rotate_info_tbl[1]
    },
};

DriveInfo* DriveInfoTable::getDriveInfo(uint8_t index) {
    return &(drive_info_tbl[index]);
}
