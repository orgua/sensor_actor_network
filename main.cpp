//
// Created by hans on 19.09.2015.
//
// test-environment

#include <iostream>
#include <stdint.h>

using namespace std;

#include "src/stack_management.h"
#include "src/layer_loopback.h"
#include "src/layer_datalink.h"     // CRC
#include "src/layer_network.h"      // control-flow (ack, req, ...)
#include "src/layer_session.h"      // basic commands to the message (register node, send data, ...)
#include "src/layer_application.h"  // handling of devices (sensors, actors)


int main()
{
    Stack.add_layer(&layerLoopback);
    Stack.add_layer(&layerDatalink);
    Stack.add_layer(&layerNetwork);
    Stack.add_layer(&layerSession);
    Stack.add_layer(&layerApplication);

    stack_message msg;
    msg.initialize();

    layerNetwork.config(0,0);
    Stack.handle_transmit(msg);
    Stack.poll(msg);

    print_message(msg);

    int * pInt;
    pInt =  reinterpret_cast<int*>(&msg); // dereference with *pInt

    cout << "Hello world! " << *pInt << endl;
    return 0;
}
