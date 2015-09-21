//
// Created by hans on 21.09.2015.
//

#ifndef SENSOR_ACTOR_NETWORK_STACK_MANAGEMENT_H
#define SENSOR_ACTOR_NETWORK_STACK_MANAGEMENT_H

#include <stdint-gcc.h>
#include "stack_message.h"
#include "layer_interface.h"


enum class placement: uint8_t
{ top, bottom, above, below };


class stack_management
{
private:

    static const uint8_t max_layer = 10;
    uint8_t counter_layer;
    layer_interface *p_layer[max_layer];

public:
    stack_management() : counter_layer(0) { };
    virtual ~stack_management() { }

    void add_layer(layer_interface *p_interface)
    {
        if (!counter_layer)
        {
            p_interface->set_as_first_layer();
            p_interface->set_as_last_layer();
            p_layer[0] = p_interface;
        }
        else
        {
            p_layer[counter_layer-1]->set_upper_layer(p_interface);

            p_interface->set_lower_layer(p_layer[counter_layer-1]);
            p_interface->set_as_last_layer();

            p_layer[counter_layer] = p_interface;
        };

        if (++counter_layer >= max_layer) counter_layer = max_layer -1;
    };

    void handle_transmit(stack_message *msg)
    {
        if (DEBUG)  cout << "transmit: ";
        p_layer[0]->handle_transmit(msg);
        if (DEBUG)  cout << endl;
    };

    void handle_receive(stack_message *msg)
    {
        if (DEBUG)  cout << "receive: ";
        p_layer[0]->handle_receive(msg);
        if (DEBUG)  cout << endl;
    }

};

stack_management Stack;

#endif //SENSOR_ACTOR_NETWORK_STACK_MANAGEMENT_H
