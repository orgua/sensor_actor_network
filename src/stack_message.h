#ifndef SENSOR_ACTOR_NETWORK_STACK_MESSAGE_H
#define SENSOR_ACTOR_NETWORK_STACK_MESSAGE_H

#include <stdint.h>

constexpr uint8_t PAYLOAD_MAX = 80;

struct stack_message
{
    uint8_t size;
    uint8_t position; // actual reading/writing position
    uint8_t position_end; // pre-allocate space for tailer
    //uint8_t origin;
    uint8_t payload[PAYLOAD_MAX];


    void initialize(void)
    {
        size   = 0;
        //origin = 0;
        reset_positions();
    }

    void reset_positions(void)
    {
        position     = 0;
        position_end = 0;
    };

    void add_payload(const uint8_t value)
    {
        payload[position++] = value;
        size++;
    };

    uint8_t read_payload_head(void) // autoincrements position
    {
        return payload[position++];
    }

    uint8_t read_payload_tail(void) // autoincrements position_end
    {
        return payload[size - ++position_end];
    }
    
    void allocate_tail(const uint8_t bytes)
    {
        position_end += bytes;
    }
};


void print_message(stack_message &msg)
{
    cout << "Got " << static_cast<int>(msg.size) << " Byte: ";
    for (uint8_t ivar = 0; ivar < msg.size; ++ivar)
    {
        cout << static_cast<int>(msg.payload[ivar]) << " ";
    };
    //cout << endl;
};

#endif //SENSOR_ACTOR_NETWORK_STACK_MESSAGE_H