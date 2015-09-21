//
// Created by hans on 19.09.2015.
//
// add control-flow (ack, req, ...)

#ifndef SENSOR_ACTOR_NETWORK_LAYER_NETWORK_H
#define SENSOR_ACTOR_NETWORK_LAYER_NETWORK_H

#include "layer_interface.h"

typedef union
{
    struct // bitfield
    {
        bool wantsAck  : 1;
        bool isAck     : 1;
        bool isRequest : 1;
        bool reserved  : 1;  // has tbd
    } bit;

    uint8_t value;
} Control;

//constexpr uint8_t Control_size = sizeof(Control);
//#if (sizeof(Control) != 1)
//#error "BITFIELD is larger than 1 Byte"
//#endif

class layer_network : public layer_interface
{
public:

    Control control; // is public, so readable from outside

    layer_network()
    {
        control.bit.wantsAck    = 0;
        control.bit.isAck       = 0;
        control.bit.isRequest   = 0;
    };

    void handle_receive(stack_message *msg) // TODO: handle wantsACk --> send ACK, isACk --> don't send package again
    {
        if (DEBUG) cout << "rNetwork ";
        // handle header and payload
        control.value = msg->payload[msg->position];
        msg->size = ++msg->position;
        // next layer
        if (!is_top) p_upper_layer->handle_receive(msg);
        // handle tail
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
    };

    void set_control(const uint8_t wantsAck = 0, const uint8_t isAck = 0, const uint8_t isRequest = 1)
    {
        control.bit.wantsAck    = wantsAck;
        control.bit.isAck       = isAck;
        control.bit.isRequest   = isRequest;
    };

};

layer_network layerNetwork;

#endif //SENSOR_ACTOR_NETWORK_LAYER_NETWORK_H
