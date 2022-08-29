#pragma once

#include "DerivedDriver.h"
#include "DriveInfoTable.h"

class DriveManager {
public:
    DriveManager();
    ~DriveManager();
    void Init();
    void Terminate();
    void setDriveMethod(DriveInfo *drive_info);
    void RunByDriver();
private:
    Driver *driver;
};
