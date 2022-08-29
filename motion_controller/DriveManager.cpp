#include "DriveManager.h"

DriveManager::DriveManager() {
    driver = NULL;
}

DriveManager::~DriveManager() {
    Init();
}

void DriveManager::Init() {
    if (driver != NULL) {
        delete driver;
    }
}

void DriveManager::Terminate() {
    if (driver != NULL) {
        driver->Stop();
        delete driver;
    }
}

void DriveManager::setDriveMethod(DriveInfo *drive_info) {
    if (drive_info == NULL) {
        ;
    } else {
        Init()
        switch (drive_info->method) {
            case NAIVE_STRAIGHT:
                driver = new NaiveStraightDriver();
                break;
            case NAIVE_CURVE:
                driver = new NaiveCurveDriver();
                break;
            case NAIVE_ROTATE:
                driver = new NaiveCurveDriver();
                break;
            case UNDEFINED:
                break;
            default:
                break;
        }
        driver->setDriveInfo(drive_info);
    }
}

void DriveManager::RunByDriver() {
    if (driver == NULL) {
        ;
    } else {
        driver->Run();
    }
}
