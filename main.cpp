#include <iostream>

using namespace std;

#include "src/messages.h"
#include "src/layer_application.h"

int main()
{
    Message msg;

    msg.control.isAck = 1;
    int * pInt;
    pInt =  reinterpret_cast<int*>(&msg); // dereference with *pInt

    cout << "Hello world! " << *pInt << endl;
    return 0;
}
