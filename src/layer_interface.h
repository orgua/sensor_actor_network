//
// Created by hans on 19.09.2015.
//
// general interface of a network-layer to inherit from

#ifndef SENSOR_ACTOR_NETWORK_LAYER_INTERFACE_H
#define SENSOR_ACTOR_NETWORK_LAYER_INTERFACE_H

#include "stack_message.h"

class layer_interface
{
protected:
    layer_interface *p_upper_layer, *p_lower_layer;
    uint8_t is_top, is_bottom;
    // TODO: handle message more efficient --> static readpointer for header and tailer

public:
    layer_interface() : p_upper_layer(nullptr), p_lower_layer(nullptr), is_top(1), is_bottom(1) {};
    ~layer_interface() {};

    void set_upper_layer(layer_interface *p_layer)
    {
        p_upper_layer = p_layer;

        if (p_layer != nullptr)     is_top = 0;
        else                        is_top = 1;
    };

    layer_interface* get_upper_layer(void) { return p_upper_layer; }

    void set_lower_layer(layer_interface *p_layer)
    {
        p_lower_layer = p_layer;

        if (p_layer != nullptr)     is_bottom = 0;
        else                        is_bottom = 1;
    };

    layer_interface* get_lower_layer(void) { return p_lower_layer; }

    void set_as_last_layer(void)    // TODO: maybe not needed!
    {
        p_upper_layer   = nullptr;
        is_top          = 1;
    };

    uint8_t is_last_layer(void) { return is_top; }

    void set_as_first_layer(void)    // TODO: maybe not needed!
    {
        p_lower_layer   = nullptr;
        is_bottom       = 1;
    };

    uint8_t is_first_layer(void) { return is_bottom; }

    // layer_specific functions --> declare your own

    virtual void handle_receive(stack_message *msg)  = 0;
    virtual void handle_transmit(stack_message *msg) = 0;
//    {
//        /handle header and payload
//         if (!is_top) p_upper_layer->handle_transmit(msg);
//         handle tail
//    };
    // TODO: we need something like: maintenance or poll to care for undone tasks

    virtual void poll(stack_message *msg) = 0;
};

#endif //SENSOR_ACTOR_NETWORK_LAYER_INTERFACE_H