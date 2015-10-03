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
    uint32_t& time_ms;

    uint8_t header_position;
    uint8_t has_unsend_data;

public:

    Control control; // is public, so readable from outside
    Control received;

    layer_network(stack_management &_stack, uint32_t& _time_ms) : layer_interface(_stack), time_ms(_time_ms)
    {
        last_ID_received         = 255;
        last_ID_transmitted      = 255;
        mutex_ID                 = 255;
        mutex_time_ms            = 0;
        mutex_timeout_ms         = RETRY_MS; // standard-value
        header_position          = 0;
        has_unsend_data          = 0;

        control.form.wantsAck    = 0;
        control.form.isAck       = 0;
        control.form.hasData     = 0;
        control.form.msg_ID      = 0;
        received.form.wantsAck   = 0;
        received.form.isAck      = 0;
        received.form.hasData    = 0;
        received.form.msg_ID     = 0;

        stack.add_layer(this);
    };

    // TODO: handle wantsACk --> send ACK, isACk --> don't send package again

    void write_header(stack_message& msg)
    {
        if (DEBUG) cout << "tNetwork ";
        header_position = msg.position;
        msg.add_payload(0); // just a placeholder, set in tail-section

        if ((mutex_ID == 255) && (!control.form.isAck))
        {
            last_ID_transmitted = MAX_ID & (++control.form.msg_ID); // TODO: is this last_ID important in any way?
        };
        if (control.form.wantsAck)
        {
            mutex_ID = control.form.msg_ID;
            mutex_time_ms = time_ms + mutex_timeout_ms; // use time from millis-timer
            stack.set_has_pending_operations();
            if (DEBUG) cout << "setMutex(" << static_cast<int>(control.form.msg_ID) << ")";
            if (DEBUG) cout << "@t=" << mutex_time_ms << " ";
        };
    };

    void write_tailer(stack_message& msg)
    {
        // compare counter and update control.form.hasData
        if ((msg.position - header_position) > 1)   control.form.hasData = 1;
        else                                        control.form.hasData = 0;

        msg.payload[header_position] = (control.value);
        // TODO: clear struct at the end?
    };

    void read_header(stack_message& msg)
    {
        if (DEBUG) cout << "rNetwork ";
        received.value = msg.read_payload_head();
        last_ID_received = received.form.msg_ID;

        if (received.form.isAck && (mutex_ID <= MAX_ID) && (mutex_ID == received.form.msg_ID))
        {
            if (DEBUG) cout << "clearMutex ";
            mutex_ID = 255;     // clear mutex
            mutex_time_ms = 0;
            stack.clear_has_pending_operations();  // remove flag for pending operations
        };

        if (received.form.hasData)  go_up = ~is_top; // if not last layer, goto next
        else                        go_up = 0;       // prevent next layer

        if (received.form.wantsAck)
        {
            if (DEBUG) cout << "scheduleACK(" << static_cast<int>(received.form.msg_ID) << ") ";
            control.form.wantsAck   = 0;
            control.form.isAck      = 1;
            control.form.msg_ID     = received.form.msg_ID;
            has_unsend_data         = 1;
            stack.set_has_pending_operations();
        };
    };

    void read_tailer(stack_message& msg)
    {
        // empty
    };

    void poll(stack_message& msg)
    {
        // todo: if time up resend message
        if ((mutex_ID <= MAX_ID) && (mutex_time_ms <= time_ms))
        {
            if (DEBUG) cout << "retransmit ";
            stack.handle_transmit(msg);
            return;
        };
        // if wants ack, send ack
        if (control.form.isAck && has_unsend_data)
        {
            if (DEBUG) cout << "sendACK ";
            stack_message _msg;
            _msg.initialize();
            has_unsend_data         = 0;
            stack.handle_transmit(_msg);
        }
    };

    void set_timeout(const uint32_t timeout_ms = RETRY_MS) // TODO: reference is const when created, maybe use shared_pointer<uint32_t> or init with class
    {
        mutex_timeout_ms    = timeout_ms;
    };

    void config(const uint8_t wantsAck = 0, const uint8_t isAck = 0)
    {
        control.form.wantsAck    = wantsAck;
        control.form.isAck       = isAck;
    };

};

#endif //SENSOR_ACTOR_NETWORK_LAYER_NETWORK_H