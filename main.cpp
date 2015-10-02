//
// Created by hans on 19.09.2015.
//
// test-environment

#include <iostream>
#include <stdint.h>

using namespace std;

#include "src/stack_management.h"
#include "src/layer_transceiveVirtual.h"
#include "src/layer_datalink.h"     // CRC
#include "src/layer_network.h"      // control-flow (ack, req, ...)
#include "src/layer_session.h"      // basic commands to the message (register node, send data, ...)
#include "src/layer_application.h"  // handling of devices (sensors, actors)


int main()
{
    // initialize virtual channel
    stack_message    channel;

    // initialize first Stack
    layer_transceiveVirtual layerTransceiveVirtual(Stack, channel);

    Stack.add_layer(&layerTransceiveVirtual);
    Stack.add_layer(&layerDatalink);
    Stack.add_layer(&layerNetwork);
    Stack.add_layer(&layerSession);
    Stack.add_layer(&layerApplication);

    // need a layerTransceiveVirtual and two Stacks for real comm
    stack_management Stack2;

    layer_transceiveVirtual layerTransceiveVirtual2(Stack2, channel);
    layer_datalink          layerDatalink2;
    layer_network           layerNetwork2;
    layer_session           layerSession2;
    layer_application       layerApplication2;

    Stack2.add_layer(&layerTransceiveVirtual2);
    Stack2.add_layer(&layerDatalink2);
    Stack2.add_layer(&layerNetwork2);
    Stack2.add_layer(&layerSession2);
    Stack2.add_layer(&layerApplication2);



    uint32_t time_ms = 200; // test
    layerNetwork.initialize(&time_ms);
    layerNetwork2.initialize(&time_ms);
    time_ms = 333;

    stack_message msg,msg2;
    msg.initialize();
    msg2.initialize();

    layerNetwork.config(1,0);
    Stack.handle_transmit(msg);

    while (Stack.poll(msg) || Stack2.poll(msg2)) {};



    int * pInt;
    pInt =  reinterpret_cast<int*>(&msg); // dereference with *pInt

    cout << "Hello world! " << *pInt << endl;
    return 0;
}
