#ifndef SENSOR_ACTOR_NETWORK_STACK_MESSAGE_H
#define SENSOR_ACTOR_NETWORK_STACK_MESSAGE_H

#include <stdint.h>

constexpr uint8_t PAYLOAD_MAX = 80;

struct stack_message
{
    uint8_t size;
    uint8_t position; // actual position
    uint8_t payload[PAYLOAD_MAX];
};


void print_message(stack_message *msg)
{
    cout << "Got " << static_cast<int>(msg->size) << " Byte: ";
    for (uint8_t ivar = 0; ivar < msg->size; ++ivar)
    {
        cout << static_cast<int>(msg->payload[ivar]) << " ";
    };
    cout << endl;
};

#endif //SENSOR_ACTOR_NETWORK_STACK_MESSAGE_H