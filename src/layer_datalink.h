//
// Created by hans on 19.09.2015.
//
// Adds CRC-Sum as tail to message
//

#ifndef SENSOR_ACTOR_NETWORK_LAYER_DATALINK_H
#define SENSOR_ACTOR_NETWORK_LAYER_DATALINK_H

#include "layer_interface.h"

struct tailer_datalink
{
uint8_t   checksumH; // TODO: maybe reinterpret as uint16
uint8_t   checksumL;
};

class layer_datalink : public layer_interface
{
    void handle_receive(stack_message *msg)
    {
        if (DEBUG)  cout << "rDatalink ";
        // handle header and payload
        if (!is_top) p_upper_layer->handle_receive(msg);
        // handle tail
    }

    void handle_transmit(stack_message *msg)
    {
        if (DEBUG)  cout << "tDatalink ";
        // handle header and payload
        msg->payload[msg->position++] = 1;
        msg->size++;
        if (!is_top) p_upper_layer->handle_transmit(msg);
        // handle tail
    };

};

layer_datalink layerDatalink;


#endif //SENSOR_ACTOR_NETWORK_LAYER_DATALINK_H
