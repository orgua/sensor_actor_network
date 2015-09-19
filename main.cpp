#include <iostream>

using namespace std;

#include "messages.h"
#include "ApplicationLayer.h"

int main()
{
    Message msg;

    msg.control.isAck = 1;
    int * pInt;
    pInt =  reinterpret_cast<int*>(&msg); // dereference with *pInt

    cout << "Hello world! " << *pInt << endl;
    return 0;
}
