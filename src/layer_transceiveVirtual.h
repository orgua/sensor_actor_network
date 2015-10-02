//
// Created by hans on 01.10.2015.
//
// test-layer instead of a proper physical_layer

#ifndef SENSOR_ACTOR_NETWORK_LAYER_TRANSCEIVE_VIRTUAL_H
#define SENSOR_ACTOR_NETWORK_LAYER_TRANSCEIVE_VIRTUAL_H

#include "layer_interface.h"
#include "stack_management.h"

class layer_transceiveVirtual : public layer_interface
{
private:
    stack_message&      channel;
    stack_message       message; // TODO: is this needed?
    stack_management&   stack;
    uint8_t             has_send_data;

public:

    layer_transceiveVirtual(stack_management& _stack, stack_message& _channel) :  stack(_stack), channel(_channel)
    {
        has_send_data = 0;
        stack.set_has_pending_operations(); // TODO: workaround for now
    }

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
    };

    void read_header(stack_message& msg)
    {
        if (DEBUG)  cout << "rVirtual ";

        //msg = _message;
        // fake a fresh unread message
        msg.reset_positions();
        msg.read_payload_head();
    };

    void read_tailer(stack_message& msg)
    {

        message = msg;
        //stack.set_received_message();
    };


    void poll(stack_message& msg)
    {
        if (has_send_data && (channel.size == 0))
        {
            has_send_data = 0;
            return;
        }

        if ((!has_send_data) && (channel.size))
        {
            msg = channel;
            channel.initialize();
            stack.set_has_to_receive();
        }

    };

};

//layer_transceiveVirtual layerTransceiveVirtual;


#endif //SENSOR_ACTOR_NETWORK_LAYER_TRANSCEIVE_VIRTUAL_H
