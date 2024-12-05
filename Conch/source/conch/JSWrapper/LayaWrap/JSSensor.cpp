//
// Created on 2024/11/30.
//
// Node APIs are not fully supported. To solve the compilation error of the interface cannot be found,
// please include "napi/native_api.h".

#include "JSSensor.h"
#include "util/Log.h"
#include "../../JCScriptRuntime.h"

using namespace laya;

constexpr int64_t SENSOR_SAMPLE_PERIOD = 200000000;
Sensor_Subscriber *accelerometer_g_user = nullptr;
Sensor_SubscriptionAttribute *accelerometer_sensor_attr = nullptr;
Sensor_SubscriptionId *accelerometer_sensor_id = nullptr;
Sensor_Subscriber *orientation_g_user = nullptr;
Sensor_SubscriptionAttribute *orientation_sensor_attr = nullptr;
Sensor_SubscriptionId *orientation_sensor_id = nullptr;

void JSSensor::enableSensor() {
    enableAccelerometer();
    enableOrientation();
}

void JSSensor::enableAccelerometer() {
    if (accelerometer_g_user != nullptr) {
        LOGI("Sensor Accelerometer is Listen");
        return;
    }
    accelerometer_g_user = OH_Sensor_CreateSubscriber();
    OH_SensorSubscriber_SetCallback(accelerometer_g_user, dispatchAccelerometer);
    accelerometer_sensor_attr = OH_Sensor_CreateSubscriptionAttribute();
    OH_SensorSubscriptionAttribute_SetSamplingInterval(accelerometer_sensor_attr, SENSOR_SAMPLE_PERIOD);
    // 加速度传感器
    accelerometer_sensor_id = OH_Sensor_CreateSubscriptionId();
    OH_SensorSubscriptionId_SetType(accelerometer_sensor_id, SENSOR_TYPE_ACCELEROMETER);
    // 订阅传感器
    OH_Sensor_Subscribe(accelerometer_sensor_id, accelerometer_sensor_attr, accelerometer_g_user);
    LOGI("Sensor Accelerometer is enable Listen");
}

void JSSensor::enableOrientation() {
    if (orientation_g_user != nullptr) {
        LOGI("Sensor Orientation is Listen");
        return;
    }
    orientation_g_user = OH_Sensor_CreateSubscriber();
    OH_SensorSubscriber_SetCallback(orientation_g_user, dispatchOrientation);
    orientation_sensor_attr = OH_Sensor_CreateSubscriptionAttribute();
    OH_SensorSubscriptionAttribute_SetSamplingInterval(orientation_sensor_attr, SENSOR_SAMPLE_PERIOD);
    // 旋转传感器
    orientation_sensor_id = OH_Sensor_CreateSubscriptionId();
    OH_SensorSubscriptionId_SetType(orientation_sensor_id, SENSOR_TYPE_ORIENTATION);
    // 订阅传感器
    OH_Sensor_Subscribe(orientation_sensor_id, orientation_sensor_attr, orientation_g_user);
    LOGI("Sensor Orientation is enable Listen");
}

void JSSensor::disableSensor() {
    disableAccelerometer();
    disableOrientation();
}

void JSSensor::disableAccelerometer() {
    if (accelerometer_g_user == nullptr) {
        LOGI("Sensor Accelerometer is not Listen");
        return;
    }
    if (accelerometer_sensor_id != nullptr) {
        OH_Sensor_Unsubscribe(accelerometer_sensor_id, accelerometer_g_user);
        OH_Sensor_DestroySubscriptionId(accelerometer_sensor_id);
    }
    if (accelerometer_sensor_attr != nullptr) {
        OH_Sensor_DestroySubscriptionAttribute(accelerometer_sensor_attr);
    }
    OH_Sensor_DestroySubscriber(accelerometer_g_user);
    accelerometer_g_user = nullptr;
    accelerometer_sensor_attr = nullptr;
    accelerometer_sensor_id = nullptr;
    LOGI("Sensor Accelerometer is disable Listen");
}

void JSSensor::disableOrientation() {
    if (orientation_g_user == nullptr) {
        LOGI("Sensor Orientation is not Listen");
        return;
    }
    if (orientation_sensor_id != nullptr) {
        OH_Sensor_Unsubscribe(orientation_sensor_id, orientation_g_user);
        OH_Sensor_DestroySubscriptionId(orientation_sensor_id);
    }
    if (orientation_sensor_attr != nullptr) {
        OH_Sensor_DestroySubscriptionAttribute(orientation_sensor_attr);
    }
    OH_Sensor_DestroySubscriber(orientation_g_user);
    orientation_g_user = nullptr;
    orientation_sensor_attr = nullptr;
    orientation_sensor_id = nullptr;
    LOGI("Sensor Orientation is disable Listen");
}

void JSSensor::dispatchAccelerometer(Sensor_Event *event) {
    if (event == nullptr) {
        LOGI("dispatchAccelerometer event is null");
        return;
    }
    float *data = nullptr;
    uint32_t length = 0;
    int32_t ret = OH_SensorEvent_GetData(event, &data, &length);
    if (ret != SENSOR_SUCCESS) {
        return;
    }
    
    DeviceMotionEvent e;
    e.nType = E_DEVICEMOTION;
    e.ax = 0;
    e.ay = 0;
    e.az = 0;
    e.agx = data[0];
    e.agy = data[1];
    e.agz = data[2];
    e.ra = 0;
    e.rb = 0;
    e.rg = 0;
    e.interval = 1;
    strncpy(e.type, "devicemotion", 256);
    JCScriptRuntime::s_JSRT->dispatchInputEvent(e);
}

void JSSensor::dispatchOrientation(Sensor_Event *event) {
    if (event == nullptr) {
        LOGI("dispatchOrientation event is null");
        return;
    }
    float *data = nullptr;
    uint32_t length = 0;
    int32_t ret = OH_SensorEvent_GetData(event, &data, &length);
    if (ret != SENSOR_SUCCESS) {
        return;
    }
    DeviceOrientationEvent e;
    e.nType = E_DEVICEORIENTATION;
    e.ra = data[0];
    e.rb = data[1];
    e.rg = data[2];
    strncpy(e.type, "deviceorientation", 256);
    JCScriptRuntime::s_JSRT->dispatchInputEvent(e);
}