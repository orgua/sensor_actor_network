//
// Created by hans on 19.09.2015.
//
// test-environment

#include <iostream>
#include <stdint.h>

using namespace std;

#include "src/stack_management.h"
#include "src/layer_loopback.h"
#include "src/layer_datalink.h"     // add CRC
#include "src/layer_network.h"      // add control-flow (ack, req, ...)
#include "src/layer_session.h"      // adds basic commands to the message (register node, send data, ...)
#include "src/layer_application.h"  // handling of devices (sensors, actors)


int main()
{
    stack_message msg;

    Stack.add_layer(&layerLoopback); // TODO: switch to reference instead of pointer?
    Stack.add_layer(&layerDatalink);
    Stack.add_layer(&layerNetwork);
    Stack.add_layer(&layerSession);
    Stack.add_layer(&layerApplication);

    msg.size = 1;
    Stack.handle_transmit(&msg);
    Stack.poll(&msg);

    print_message(&msg);

    int * pInt;
    pInt =  reinterpret_cast<int*>(&msg); // dereference with *pInt

    cout << "Hello world! " << *pInt << endl;
    return 0;
}
