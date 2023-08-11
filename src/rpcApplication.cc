#include <iostream>
#include <string>
#include <unistd.h>

#include "rpcApplication.h"

RpcConfig MyrpcApplication::config_;

void MyrpcApplication::Init(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cout << "format: command -i <configfile>" << std::endl;
        exit(EXIT_FAILURE);
    }
    int c = 0;
    std::string config_file;
    while((c = getopt(argc,argv,"i:")) != -1)
    {
        switch (c)
        {
        case 'i':
            config_file = optarg;
            break;
        case '?':
            exit(EXIT_FAILURE);
        case ':':
            exit(EXIT_FAILURE);
        default:
            break;
        }
    }
    config_.loadConfigFile(config_file.c_str());

    // std::cout << "rpcserverip:" << config_.load("rpcserverip") << std::endl;
    // std::cout << "rpcserverport:" << config_.load("rpcserverport") << std::endl;
    // std::cout << "zookeeperip:" << config_.load("zookeeperip") << std::endl;
    // std::cout << "zookeeperport:" << config_.load("zookeeperport") << std::endl;
}
MyrpcApplication &MyrpcApplication::GetInstance()
{
    static MyrpcApplication app;
    return app;
}

RpcConfig &MyrpcApplication::GetConfig()
{
    return config_;
}