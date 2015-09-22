//
// Created by hans on 19.09.2015.
//
// add control-flow (ack, req, ...)

#ifndef SENSOR_ACTOR_NETWORK_LAYER_NETWORK_H
#define SENSOR_ACTOR_NETWORK_LAYER_NETWORK_H

#include <stdint.h>
#include "layer_interface.h"

typedef union
{
    uint8_t value;

    struct // bitfield
    {
        bool    wantsAck  : 1;
        bool    isAck     : 1;
        uint8_t msg_ID    : 6; // max 63 (0x3F)
    } form;
} Control;

class layer_network : public layer_interface
{
private:
    uint8_t  last_ID_received, last_ID_transmitted;
    uint8_t  mutex_ID; // ID of Packet that wants an ACK
    uint32_t mutex_time_ms;
    static constexpr uint32_t retry_ms = 1000; // TODO: make settable

public:

    Control control; // is public, so readable from outside

    layer_network()
    {
        control.form.wantsAck    = 0;
        control.form.isAck       = 0;
        control.form.msg_ID      = 0;
        last_ID_received         = 255;
        last_ID_transmitted      = 255;
        mutex_ID                 = 255;
        mutex_time_ms            = 0;
    };

    void handle_receive(stack_message *msg) // TODO: handle wantsACk --> send ACK, isACk --> don't send package again
    {
        if (DEBUG) cout << "rNetwork ";
        // handle header and payload
        control.value = msg->payload[msg->position];
        ++msg->position;
        // next layer
        if (!is_top) p_upper_layer->handle_receive(msg);
        // handle tail
        last_ID_received = control.form.msg_ID;
    };

    void handle_transmit(stack_message *msg)
    {
        if (DEBUG) cout << "tNetwork ";
        // handle header and payload
        msg->payload[msg->position] = control.value;
        msg->size = ++msg->position;
        // next layer
        if (!is_top) p_upper_layer->handle_transmit(msg);
        // handle tail
        last_ID_transmitted = ++(control.form.msg_ID);
    };

    void set_control(const uint8_t wantsAck = 0, const uint8_t isAck = 0)
    {
        control.form.wantsAck    = wantsAck;
        control.form.isAck       = isAck;
    };

};

layer_network layerNetwork;

#endif //SENSOR_ACTOR_NETWORK_LAYER_NETWORK_H
