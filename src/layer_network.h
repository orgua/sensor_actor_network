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
        bool    hasData   : 1;
        uint8_t msg_ID    : 5; // max 31
    } form;
} Control;

class layer_network : public layer_interface
{
private:
    uint8_t  last_ID_received, last_ID_transmitted;
    uint8_t  mutex_ID; // ID of Packet that wants an ACK
    uint32_t mutex_time_ms;
    static constexpr uint32_t RETRY_MS = 1000; // TODO: make settable
    static constexpr uint8_t  MAX_ID   = 31;

public:

    Control control; // is public, so readable from outside

    layer_network()
    {
        control.form.wantsAck    = 0;
        control.form.isAck       = 0;
        control.form.hasData     = 0;
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
        control.value = msg->payload[msg->position++];
        last_ID_received = control.form.msg_ID;
        if (control.form.isAck && (mutex_ID <= MAX_ID) && (mutex_ID = control.form.msg_ID))
        {
            mutex_ID = 255;
            mutex_time_ms = 0;
            Stack.clear_has_pending_operations();
            // TODO: remove flag for pending operations
        }
        // next layer
        if (!control.form.hasData) return;
        if (!is_top) p_upper_layer->handle_receive(msg);
        // handle tail
    };

    void handle_transmit(stack_message *msg)
    {
        if (DEBUG) cout << "tNetwork ";
        // handle header and payload
        msg->payload[msg->position] = control.value;
        msg->size = ++msg->position;
        last_ID_transmitted = MAX_ID&(++control.form.msg_ID); // TODO: is this last_ID important in any way?
        if (control.form.wantsAck)      {
            mutex_ID = control.form.msg_ID;
            mutex_time_ms = 0; // TODO: include millis-timer
            Stack.set_has_pending_operations();
        };
        // next layer
        if (!is_top) p_upper_layer->handle_transmit(msg);

        // TODO: compare counter and update control.form.hasData
        // handle tail

    };

    void poll(stack_message *msg)
    {
    // todo: if time up resend message
    };

    void config(const uint8_t wantsAck = 0, const uint8_t isAck = 0)
    {
        control.form.wantsAck    = wantsAck;
        control.form.isAck       = isAck;
    };

};

layer_network layerNetwork;

#endif //SENSOR_ACTOR_NETWORK_LAYER_NETWORK_H
