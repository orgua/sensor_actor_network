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

    void write_header(stack_message& msg)
    {
        if (DEBUG)  cout << "tLoopback ";
        msg.position = 0;
        msg.payload[msg.position] = 1;
        msg.size = ++msg.position;
    };

    void write_tailer(stack_message& msg)
    {
        // fake a new message and inform the stack
        msg.position = 0;
        Stack.set_received_message();
    };

    void read_header(stack_message& msg)
    {
        if (DEBUG)  cout << "rLoopback ";
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
