//
// Created by hans on 19.09.2015.
//
// adds basic commands to the message (register node, send data, ...)

#ifndef SENSOR_ACTOR_NETWORK_LAYER_SESSION_H
#define SENSOR_ACTOR_NETWORK_LAYER_SESSION_H

#include <stdint.h>
#include "layer_interface.h"

enum class session_command : uint8_t    // underlying type
{
    announce,       // new node tells what it got
    registerNode,   // get new ID and integrate new node into network,

    sendData,       // handled in next layer, needs payload_length
    ping,           // to each node, possible to trigger a tree-search
    nwInfo,         // whoAmI, map of seen nodes (32bytes?)
    unregister,     // node has to reannounce

    suspend,        // send to sleep for a specific time (uC) --> needed?
    shutdown,       // send to sleep for a specific time (uC + RF)
};

enum class session_status : uint8_t
{
    shutdown,
    unregistered,
    announced,
    registered,
    subscribed,
    suspended
};


struct RegisterNodeHeader
{
    uint8_t newID;
    uint16_t updateInterval_s; // for Sensors, if 0 ... use default
    //uint8_t     hasToBe;
};

struct Unregister
{
    bool shutdown   : 1;
    bool reregister : 1;
};

struct Shutdown
{
    uint8_t time_sec;
};

struct Ping
{
    uint8_t senderID;
    uint8_t receiverID;
    uint8_t originatorID;    // who started this
    uint8_t roundtrip_ms;
    uint8_t magicNumber;     // to prevent endless loop, the one who did get this number before does not have to answer
    bool spanTree    : 1; // receiver sends own ping
    bool spanNetwork : 1;
};

typedef union
{
    uint8_t value[3];

    struct
    {
        session_command command;
        uint8_t     pLength;
        //uint8_t   payload[PAYLOAD_MAX]; // TODO: should this contain a pointer to the payload instead?
    } form;
} header_session;


class layer_session : public layer_interface
{
private:
    session_status status;
public:
    layer_session(void)
    {
        status = session_status::unregistered;
    }

    void handle_receive(stack_message *msg)
    {
        if (DEBUG) cout << "rSession ";
        // handle header and payload
        if (!is_top) p_upper_layer->handle_receive(msg);
        // handle tail
    }

    void handle_transmit(stack_message *msg)
    {
        if (DEBUG) cout << "tSession ";
        // handle header and payload
        msg->payload[msg->position] = 1;
        msg->size = ++msg->position;
        if (!is_top) p_upper_layer->handle_transmit(msg);
        // handle tail
    };

    session_status get_status(void)
    {
        return status;
    };
};

layer_session layerSession;

#endif //SENSOR_ACTOR_NETWORK_LAYER_SESSION_H
