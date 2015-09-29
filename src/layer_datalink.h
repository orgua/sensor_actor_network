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

    void write_header(stack_message& msg)
    {
        if (DEBUG)  cout << "tDatalink ";
        msg.payload[msg.position] = 1;
        msg.size = ++msg.position;
    };

    void write_tailer(stack_message& msg)
    {
        // empty
    };

    void read_header(stack_message& msg)
    {
        if (DEBUG)  cout << "rDatalink ";
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

layer_datalink layerDatalink;


#endif //SENSOR_ACTOR_NETWORK_LAYER_DATALINK_H
