#ifndef SENSOR_ACTOR_NETWORK_STACK_MESSAGE_H
#define SENSOR_ACTOR_NETWORK_STACK_MESSAGE_H

constexpr uint8_t PAYLOAD_MAX = 80;

struct stack_message
{
    uint8_t size;
    uint8_t position; // actual position
    uint8_t payload[PAYLOAD_MAX];
};

#endif //SENSOR_ACTOR_NETWORK_STACK_MESSAGE_H