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
    static constexpr uint32_t RETRY_MS      = 1000; // TODO: make settable
    static constexpr uint8_t  MAX_ID        = 31;

    uint8_t   last_ID_received, last_ID_transmitted;
    uint8_t   mutex_ID; // ID of Packet that wants an ACK
    uint32_t  mutex_time_ms, mutex_timeout_ms;
    uint32_t& ref_time_ms;

    uint8_t header_position;

public:

    Control control; // is public, so readable from outside

    layer_network() : ref_time_ms(mutex_time_ms)
    {
        control.form.wantsAck    = 0;
        control.form.isAck       = 0;
        control.form.hasData     = 0;
        control.form.msg_ID      = 0;
        last_ID_received         = 255;
        last_ID_transmitted      = 255;
        mutex_ID                 = 255;
        mutex_time_ms            = 0;
        mutex_timeout_ms         = RETRY_MS; // standard-value
        header_position          = 0;
    };

    // TODO: handle wantsACk --> send ACK, isACk --> don't send package again

    void write_header(stack_message& msg)
    {
        if (DEBUG) cout << "tNetwork ";
        header_position = msg.position;
        msg.add_payload(0); // just a placeholder, set in tail-section

        last_ID_transmitted = MAX_ID&(++control.form.msg_ID); // TODO: is this last_ID important in any way?
        if (control.form.wantsAck)      {
            mutex_ID = control.form.msg_ID;
            mutex_time_ms = ref_time_ms + mutex_timeout_ms; // TODO: include millis-timer
            if (DEBUG) cout << "@t=" << mutex_time_ms << " ";
            Stack.set_has_pending_operations();
        };
    };

    void write_tailer(stack_message& msg)
    {
        // compare counter and update control.form.hasData
        if ((msg.position - header_position) > 1)   control.form.hasData = 1;
        else                                        control.form.hasData = 0;

        msg.payload[header_position] = (control.value);
    };

    void read_header(stack_message& msg)
    {
        if (DEBUG) cout << "rNetwork ";
        control.value = msg.read_payload_head();
        last_ID_received = control.form.msg_ID;
        if (control.form.isAck && (mutex_ID <= MAX_ID) && (mutex_ID = control.form.msg_ID))
        {
            mutex_ID = 255;
            mutex_time_ms = 0;
            Stack.clear_has_pending_operations();  // remove flag for pending operations
        }
        if (control.form.hasData)  go_up = ~is_top; // if not last layer, goto next
        else                       go_up = 0;       // prevent next layer
    };

    void read_tailer(stack_message& msg)
    {
        // empty
    };

    void poll(stack_message& msg)
    {
    // todo: if time up resend message
    };

    void initialize(const uint32_t& time_ms, const uint32_t timeout_ms = RETRY_MS) // TODO: no real reference created, maybe use shared_pointer<uint32_t>
    {
        ref_time_ms(time_ms);
        mutex_timeout_ms = timeout_ms;
    };

    void config(const uint8_t wantsAck = 0, const uint8_t isAck = 0)
    {
        control.form.wantsAck    = wantsAck;
        control.form.isAck       = isAck;
    };

};

layer_network layerNetwork;

#endif //SENSOR_ACTOR_NETWORK_LAYER_NETWORK_H
