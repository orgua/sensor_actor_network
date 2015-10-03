//
// Created by hans on 19.09.2015.
//
// DO NOT USE ANYMORE, VIRTUAL-Transceiver is more appropriate

#ifndef SENSOR_ACTOR_NETWORK_LAYER_LOOPBACK_H
#define SENSOR_ACTOR_NETWORK_LAYER_LOOPBACK_H

#include "stack_management.h"
#include "layer_interface.h"

class layer_transceiveLoopback : public layer_interface
{
private:
    stack_message _message;

public:

    layer_transceiveLoopback(stack_management &_stack) : layer_interface(_stack)
    {
        stack.add_layer(this);
    };

    void write_header(stack_message& msg)
    {
        if (DEBUG)  cout << "tLoopback ";
        msg.add_payload(22);
    };

    void write_tailer(stack_message& msg)
    {
        // inform the stack
        _message = msg;
        Stack.set_has_to_receive();
    };

    void read_header(stack_message& msg)
    {
        if (DEBUG)  cout << "rLoopback ";

        msg = _message;
        // fake a fresh unread message
        msg.reset_positions();
        msg.read_payload_head();
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

#endif //SENSOR_ACTOR_NETWORK_LAYER_LOOPBACK_H
