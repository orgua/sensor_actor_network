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
    void handle_receive(stack_message *msg)
    {
        if (DEBUG)  cout << "rLoopback ";
        // handle header and payload
        if (!is_top) p_upper_layer->handle_receive(msg);
        // handle tail
    }

    void handle_transmit(stack_message *msg)
    {
        if (DEBUG)  cout << "tLoopback ";
        msg->position = 0;
        // handle header and payload
        msg->payload[msg->position] = 1;
        msg->size = ++msg->position;
        if (!is_top) p_upper_layer->handle_transmit(msg);
        // handle tail
        if (DEBUG)  cout << endl;

        msg->position = 0;
        Stack.handle_receive(msg);
    };

};

layer_loopback layerLoopback;


#endif //SENSOR_ACTOR_NETWORK_LAYER_LOOPBACK_H
