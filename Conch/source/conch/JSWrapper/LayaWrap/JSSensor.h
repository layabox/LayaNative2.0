//
// Created on 2024/11/30.
//
// Node APIs are not fully supported. To solve the compilation error of the interface cannot be found,
// please include "napi/native_api.h".

#ifndef OHOS_JSSENSOR_H
#define OHOS_JSSENSOR_H

#include "sensors/oh_sensor.h"

class JSSensor {
public:
    static void enableSensor();
    static void disableSensor();
    
private:
    static void enableAccelerometer();
    static void enableOrientation();
    static void disableAccelerometer();
    static void disableOrientation();
    static void dispatchAccelerometer(Sensor_Event *event);
    static void dispatchOrientation(Sensor_Event *event);
};

#endif //OHOS_JSSENSOR_H
