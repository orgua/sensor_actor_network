//
// Created by hans on 21.09.2015.
//

#ifndef SENSOR_ACTOR_NETWORK_STACK_MANAGEMENT_H
#define SENSOR_ACTOR_NETWORK_STACK_MANAGEMENT_H

constexpr uint8_t DEBUG = 1;

#include <stdint-gcc.h>
#include "stack_message.h"
#include "layer_interface.h"


enum class placement : uint8_t
{
    top, bottom, above, below
};


class stack_management
{
private:

    static const uint8_t max_layer = 10;
    uint8_t counter_layer;
    layer_interface *p_layer[max_layer];
    bool has_pending_operations, received_message, has_to_transmit;


public:
    stack_management() : counter_layer(0), has_pending_operations(0), received_message(0), has_to_transmit(0)
    { };

    virtual ~stack_management()
    { }

    void add_layer(layer_interface *p_interface)
    {
        if (!counter_layer) {
            p_interface->set_as_first_layer();
            p_interface->set_as_last_layer();
            p_layer[0] = p_interface;
        }
        else {
            p_layer[counter_layer - 1]->set_upper_layer(p_interface);

            p_interface->set_lower_layer(p_layer[counter_layer - 1]);
            p_interface->set_as_last_layer();

            p_layer[counter_layer] = p_interface;
        };

        if (++counter_layer >= max_layer) counter_layer = max_layer - 1;
    };

    void handle_receive(stack_message& msg)
    {
        if (DEBUG) cout << "received: ";
        p_layer[0]->handle_receive(msg);
        if (DEBUG) cout << endl;

        received_message = 0;
    };

    void set_received_message(void)
    {
        received_message = 1;
    };

    void handle_transmit(stack_message& msg)
    {
        if (DEBUG) cout << "transmit: ";
        p_layer[0]->handle_transmit(msg);
        if (DEBUG) cout << endl;

        has_to_transmit = 0;
    };

    void set_has_to_transmit(void)
    {
        has_to_transmit = 1;
    };

    void poll(stack_message& msg)
    {
        if (has_pending_operations) p_layer[0]->poll(msg);
        if (has_to_transmit)        handle_transmit(msg);
        if (received_message)       handle_receive(msg);
    };

    void set_has_pending_operations(void)
    {
        has_pending_operations = 1;
    };

    void clear_has_pending_operations(void)
    {
        has_pending_operations = 0;
    }

};

stack_management Stack;

#endif //SENSOR_ACTOR_NETWORK_STACK_MANAGEMENT_H
