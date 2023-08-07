#pragma once

#include "rpcConfig.h"

class MyrpcApplication
{
public:
    static void Init(int argc, char **argv);
    static MyrpcApplication &GetInstance();

private:
    MyrpcApplication() {}
    MyrpcApplication(const MyrpcApplication &) = delete;
    MyrpcApplication(MyrpcApplication &&) = delete;

    static RpcConfig config_;
};