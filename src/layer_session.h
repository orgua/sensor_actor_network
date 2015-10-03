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
    empty,          // nothing to do
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
    layer_session(stack_management &_stack) : layer_interface(_stack)
    {
        status = session_status::unregistered;
        stack.add_layer(this);
    }

    void write_header(stack_message& msg)
    {
        if (DEBUG) cout << "tSession ";
        msg.add_payload(42);
    };

    void write_tailer(stack_message& msg)
    {
        // empty
    };

    void read_header(stack_message& msg)
    {
        if (DEBUG) cout << "rSession ";
        msg.read_payload_head();
    };

    void read_tailer(stack_message& msg)
    {
        // empty
    };

    void poll(stack_message& msg)
    {
        // empty
    };

    session_status get_status(void)
    {
        return status;
    };
};

#endif //SENSOR_ACTOR_NETWORK_LAYER_SESSION_H
