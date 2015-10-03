//
// Created by hans on 19.09.2015.
//
// test-environment

#include <iostream>
#include <stdint.h>

using namespace std;

constexpr uint8_t DEBUG = 11;

#include "src/stack_management.h"
#include "src/layer_transceiveVirtual.h"
#include "src/layer_datalink.h"     // CRC
#include "src/layer_network.h"      // control-flow (ack, req, ...)
#include "src/layer_session.h"      // basic commands to the message (register node, send data, ...)
#include "src/layer_application.h"  // handling of devices (sensors, actors)


int main()
{
    uint32_t time_ms = 0; // test

    // initialize virtual channel
    stack_message    channel;

    // initialize first Stack
    stack_management        stackA;
    stack_message           msgA;
    layer_transceiveVirtual layerTransceiveVirtualA(stackA, channel);
    layer_datalink          layerDatalinkA(stackA);
    layer_network           layerNetworkA(stackA,time_ms);
    layer_session           layerSessionA(stackA);
    layer_application       layerApplicationA(stackA);

    // need a layerTransceiveVirtual and two Stacks for real comm
    stack_management        stackB;
    stack_message           msgB;
    layer_transceiveVirtual layerTransceiveVirtualB(stackB, channel);
    layer_datalink          layerDatalinkB(stackB);
    layer_network           layerNetworkB(stackB,time_ms);
    layer_session           layerSessionB(stackB);
    layer_application       layerApplicationB(stackB);

    // connect the two stacks
    layerTransceiveVirtualA.set_second_stack(&stackB);
    layerTransceiveVirtualB.set_second_stack(&stackA);

    layerNetworkA.config(1,0);
    stackA.handle_transmit(msgA);

    for (uint16_t ivar = 0; ivar < 3040; ++ivar)
    {
        time_ms++;
        stackA.poll(msgA);
        //if (ivar < 1200) channel.initialize(); // fake lost messages
        stackB.poll(msgB);
        if (ivar < 1200) channel.initialize(); // fake lost messages
    }

    cout << endl;

    int * pInt;
    pInt =  reinterpret_cast<int*>(&msgA); // dereference with *pInt

    cout << "Hello world! " << *pInt << endl;
    return 0;
}
