//
// Created by hans on 19.09.2015.
//
// Adds CRC-Sum as tail to message
//

#ifndef SENSOR_ACTOR_NETWORK_LAYER_DATALINK_H
#define SENSOR_ACTOR_NETWORK_LAYER_DATALINK_H

#include "layer_interface.h"

typedef union
{
    uint16_t value;
    struct
    {
        uint8_t   checksumH; // TODO: maybe reinterpret as uint16
        uint8_t   checksumL;
    } form;
} tailer_datalink;


class layer_datalink : public layer_interface
{
public:

    void handle_receive(stack_message *msg)
    {
        if (DEBUG)  cout << "rDatalink ";
        // Check CRC of Message



        if (!is_top) p_upper_layer->handle_receive(msg);
        // handle tail
    }

    void handle_transmit(stack_message *msg)
    {
        if (DEBUG)  cout << "tDatalink ";
        // handle header and payload
        msg->payload[msg->position] = 1;
        msg->size = ++msg->position;
        if (!is_top) p_upper_layer->handle_transmit(msg);
        // handle tail
    };

    void poll(stack_message *msg)
    {

    };

};

layer_datalink layerDatalink;


#endif //SENSOR_ACTOR_NETWORK_LAYER_DATALINK_H
