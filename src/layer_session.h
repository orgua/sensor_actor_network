//
// Created by hans on 19.09.2015.
//
// adds basic commands to the message (register node, send data, ...)

#ifndef SENSOR_ACTOR_NETWORK_LAYER_SESSION_H
#define SENSOR_ACTOR_NETWORK_LAYER_SESSION_H

#include "layer_interface.h"

enum class Command : uint8_t    // underlying type
{
    announce,       // new node tells what it got
    registerNode,   // get new ID and integrate new node into network,
    unregister,     // node has to reannounce
    //subscribe,      // same es register? tbd. maybe specialize
            shutdown,      // send to sleep for a specific time (�C + RF)
    //suspend,         // send to sleep for a specific time (�C) --> needed?

    ping,           // to each node, possible to trigger a tree-search
    sendRawData,
    sendSensor,
    sendTrigger,
    sendActor,      // normally to Node
    // sendUndefined,
            sendTime        // prefer as broadcast, so every node can sync
};

struct header_session
{
    Command   command;
    uint8_t   pLength;
    //uint8_t   payload[PAYLOAD_MAX]; // TODO: should this contain a pointer to the payload instead?
};

class layer_session : public layer_interface
{
    void handle_receive(stack_message *msg)
    {
        if (DEBUG)  cout << "rSession ";
        // handle header and payload
        if (!is_top) p_upper_layer->handle_receive(msg);
        // handle tail
    }

    void handle_transmit(stack_message *msg)
    {
        if (DEBUG)  cout << "tSession ";
        // handle header and payload
        msg->payload[msg->position++] = 1;
        msg->size++;
        if (!is_top) p_upper_layer->handle_transmit(msg);
        // handle tail
    };

};

layer_session layerSession;

#endif //SENSOR_ACTOR_NETWORK_LAYER_SESSION_H
