#include <iostream>
#include "../../src/include/rpcZookeeper.h"

int main()
{
    ZkClient zkc;
    zkc.Start();
    char data[128] = "hello zk";
    zkc.Create("/test", data, sizeof(data));
    std::cout << zkc.GetData("/test") << std::endl;


    return 0;
}