//
// Created by hans on 19.09.2015.
//

#ifndef SENSOR_ACTOR_NETWORK_LAYER_APPLICATION_H
#define SENSOR_ACTOR_NETWORK_LAYER_APPLICATION_H

#include <stdint-gcc.h>
#include "layer_interface.h"

enum class application_command : uint8_t
{
    announce,       // send my _Devices
    subscribe,      // same es register? tbd. maybe specialize

    sendRawData,
    sendSensor,
    sendTrigger,
    sendActor,      // normally to Node
    sendUndefined,
    sendTime,        // prefer as broadcast, so every node can sync
    sendInfo,
    sendPingData
};

struct AnnounceHeader
{
    uint8_t lengthSensorList;   // has to follow after the header
    uint8_t lengthTriggerList;  // same
    uint8_t lengthActorList;    // same
};
//constexpr size_t  AnnounceHeaderSize = sizeof(AnnounceHeader);

class layer_application : public layer_interface
{

    void write_header(stack_message& msg)
    {
        if (DEBUG) cout << "tApplication ";
        msg.payload[msg.position] = 1;
        msg.size = ++msg.position;
    };

    void write_tailer(stack_message& msg)
    {
        // empty
    };

    void read_header(stack_message& msg)
    {
        if (DEBUG) cout << "rApplication ";
    };

    void read_tailer(stack_message& msg)
    {
        // empty
    };

    void poll(stack_message& msg)
    {
        // empty
    };

};

layer_application layerApplication;


struct SendRawData
{
    // something to declare here?
};

struct SendSensor
{
    uint16_t included;   // bitfield
};

struct SendTrigger
{
    uint16_t included;
};

struct SendActor
{
    uint16_t included;
};

struct SendTime
{
    uint32_t unixtime_sec;
    uint32_t unixtime_ms;
};

/// Devices are divided into groups:  sensors, triggers, actors, undefined
// --> every sensor needs it's own definition-struct. should contain datatype, update-rate, unit,
enum class _Devices : uint8_t
{
    sensor_generic = 0,  // all sensors grouped

    battery_voltage_V,
    battery_voltage_mV,

    battery_current_A,
    battery_current_mA,
    battery_current_uA,

    rssi_dBm,

    time_unix_sec,
    time_unix_ms,

    adc_8b,
    adc_16b,

    acceleration_mg_x,
    acceleration_mg_y,
    acceleration_mg_z,

    barometer_pressure_pascal,
    barometer_pressure_mBar,
    barometer_pressure_uBar,
    barometer_altitude_m,
    barometer_altitude_cm,

    gyro_dps_x,
    gyro_dps_y,
    gyro_dps_z,
    gyro_mdps_x,
    gyro_mdps_y,
    gyro_mdps_z,

    magnetometer_uT_x,
    magnetometer_uT_y,
    magnetometer_uT_z,

    temperature_C,
    temperature_mC,

    airhumidity_relative,

    light_lux_value,
    light_lux_exp,

    light_intensity_clear,
    light_intensity_red,
    light_intensity_green,
    light_intensity_blue,


    trigger_generic = 64, //16384, //  all trigger grouped, bool values, can be from sensors... get send immediately

    switch_trigger,
    button_trigger,

    actor_generic = 128, //32768, // all actors grouped

    light_switch,
    motor_switch,
    motor_pwm,

    undefined_generic = 192 //49152,

};

#endif // SENSOR_ACTOR_NETWORK_LAYER_APPLICATION_H
