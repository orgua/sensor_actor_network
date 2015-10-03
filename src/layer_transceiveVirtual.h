//
// Created by hans on 01.10.2015.
//
// test-layer instead of a proper physical_layer

#ifndef SENSOR_ACTOR_NETWORK_LAYER_TRANSCEIVE_VIRTUAL_H
#define SENSOR_ACTOR_NETWORK_LAYER_TRANSCEIVE_VIRTUAL_H


#include "stack_management.h"
#include "layer_interface.h"

class layer_transceiveVirtual : public layer_interface
{
private:
    uint8_t             has_send_data;
    stack_management*   stackB;
    stack_message&      channel;


public:

    layer_transceiveVirtual(stack_management &_stack, stack_message& _channel) : layer_interface(_stack), has_send_data(0), stackB(nullptr), channel(_channel)
    {
        stack.add_layer(this);
    };

    void write_header(stack_message& msg)
    {
        if (DEBUG)  cout << "tVirtual ";
        msg.add_payload(22);
    };

    void write_tailer(stack_message& msg)
    {
        // inform the stack
        channel = msg;
        has_send_data = 1;
        stackB->set_has_pending_operations();
        if (DEBUG)  cout << "MsgSend ";
    };

    void read_header(stack_message& msg)
    {
        if (DEBUG)  cout << "rVirtual ";

        // fake a fresh unread message
        msg.reset_positions();
        msg.read_payload_head();
    };

    void read_tailer(stack_message& msg)
    {

    };


    void poll(stack_message& msg)
    {
        if (has_send_data && (!channel.size))
        {
            if (DEBUG)  cout << "MsgReadAck ";
            has_send_data = 0;
            return;
        }

        if ((!has_send_data) && (channel.size))
        {
            if (DEBUG)  cout << "MsgRead ";
            msg = channel;
            channel.initialize();
            if (stackB != nullptr)  stackB->set_has_pending_operations();
            stack.set_has_to_receive();
        }

    };

    void set_second_stack(stack_management* _stack)
    {
        stackB = _stack;
    }

};

#endif //SENSOR_ACTOR_NETWORK_LAYER_TRANSCEIVE_VIRTUAL_H
