//
// Created by hans on 19.09.2015.
//
// add control-flow (ack, req, ...)

#ifndef SENSOR_ACTOR_NETWORK_LAYER_NETWORK_H
#define SENSOR_ACTOR_NETWORK_LAYER_NETWORK_H

#include "layer_interface.h"

struct Control // bitfield
{
    bool wantsAck  : 1;
    bool isAck     : 1;
    bool isRequest : 1;
    bool reserved  : 1;  // has tbd
};
//constexpr uint8_t Control_size = sizeof(Control);
//#if (sizeof(Control) != 1)
//#error "BITFIELD is larger than 1 Byte"
//#endif

class layer_network : public layer_interface
{
   void handle_receive(stack_message *msg)
   {
       if (DEBUG)  cout << "rNetwork ";
       // handle header and payload
       if (!is_top) p_upper_layer->handle_receive(msg);
       // handle tail
   }

   void handle_transmit(stack_message *msg)
   {
       if (DEBUG)  cout << "tNetwork ";
       // handle header and payload
       msg->payload[msg->position++] = 1;
       msg->size++;
       if (!is_top) p_upper_layer->handle_transmit(msg);
       // handle tail
   };

};

layer_network layerNetwork;

#endif //SENSOR_ACTOR_NETWORK_LAYER_NETWORK_H
