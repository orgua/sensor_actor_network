//
// Created by hans on 19.09.2015.
//
// test-layer instead of a proper physical_layer

#ifndef SENSOR_ACTOR_NETWORK_LAYER_LOOPBACK_H
#define SENSOR_ACTOR_NETWORK_LAYER_LOOPBACK_H

#include "layer_interface.h"
#include "stack_management.h"

class layer_loopback : public layer_interface
{
private:
    stack_message _message;

    void write_header(stack_message& msg)
    {
        if (DEBUG)  cout << "tLoopback ";
        msg.add_payload(22);
    };

    void write_tailer(stack_message& msg)
    {
        // inform the stack
        _message = msg;
        Stack.set_received_message();
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

layer_loopback layerLoopback;


#endif //SENSOR_ACTOR_NETWORK_LAYER_LOOPBACK_H
