//
// Created by hans on 19.09.2015.
//
// general interface of a network-layer to inherit from

#ifndef SENSOR_ACTOR_NETWORK_LAYER_INTERFACE_H
#define SENSOR_ACTOR_NETWORK_LAYER_INTERFACE_H

#include "stack_message.h"

class stack_management;

class layer_interface
{
protected:
    layer_interface *p_upper_layer, *p_lower_layer;
    uint8_t is_top, is_bottom, go_up;
    // TODO: handle message more efficient --> static readpointer for header and tailer
    stack_management& stack;

public:
    layer_interface(stack_management &_stack) : p_upper_layer(nullptr), p_lower_layer(nullptr), is_top(1), is_bottom(1), go_up(0), stack(_stack) {};

    ~layer_interface() {};

    void set_upper_layer(layer_interface *p_layer)
    {
        p_upper_layer = p_layer;

        if (p_layer != nullptr)
        {
            is_top  = 0;
            go_up   = 1;
        }
        else
        {
            is_top = 1;
            go_up  = 0;
        };
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
        go_up           = 0;
    };

    uint8_t is_last_layer(void) { return is_top; }

    void set_as_first_layer(void)    // TODO: maybe not needed!
    {
        p_lower_layer   = nullptr;
        is_bottom       = 1;
    };

    uint8_t is_first_layer(void) { return is_bottom; }

    // layer control

    void handle_receive(stack_message& msg)
    {
        if (DEBUG)  cout << endl << "   ";
        read_header(msg);
        if (go_up) p_upper_layer->handle_receive(msg);
        read_tailer(msg);
    };

    void handle_transmit(stack_message& msg)
    {
        if (DEBUG)  cout << endl << "   ";
        write_header(msg);
        if (go_up) p_upper_layer->handle_transmit(msg);
        write_tailer(msg);
   };

    // layer_specific functions --> declare your own

    virtual void write_header(stack_message& msg) = 0;
    virtual void write_tailer(stack_message& msg) = 0;
    virtual void read_header(stack_message& msg) = 0;   // if this msg has a tailer, you should preallocate it in position_end ( +=tailer_size )
    virtual void read_tailer(stack_message& msg) = 0;


    // TODO: we need something like: maintenance or poll to care for undone tasks

    // TODO: make it possible to send message directly from layer (transmit(), handle_transmit())
    //  transmit --> if no top layer then work down, otherwise go up and call transmit

    virtual void poll(stack_message& msg) = 0;
};

#endif //SENSOR_ACTOR_NETWORK_LAYER_INTERFACE_H