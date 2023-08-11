#include <iostream>
#include "rpcApplication.h"
#include "user.pb.h"
#include "rpcChannel.h"
#include "rpcController.h"

int main(int argc, char **argv)
{
    MyrpcApplication::Init(argc, argv);

    myRPC::UserServiceRpc_Stub stub(new MyrpcChannel());
    // rpc method的请求参数
    myRPC::LoginRequest request;
    request.set_name("zhangsan");
    request.set_pwd("123456");
    // rpc method响应
    myRPC::LoginResponse response;
    // 发起rpc方法的调用 实际上是调用了channel的CallMethod函数 是同步的
    stub.Login(nullptr, &request, &response, nullptr);
    if (0 == response.result().errcode())
    {
        std::cout << "rpc login response success:" << response.success() << std::endl;
    }
    else
    {
        std::cout << "rpc login response error : " << response.result().errmsg() << std::endl;
    }

    myRPC::RegisterRequest registerReq;
    registerReq.set_id(1001);
    registerReq.set_name("lisi");
    registerReq.set_pwd("123456789");
    myRPC::RegisterResponse registerResp;

    MyrpcController controller;
    stub.Register(&controller, &registerReq, &registerResp, nullptr);
    if (controller.Failed())
    {
        std::cout << controller.ErrorText() << std::endl;
    }
    else
    {
        if (0 == registerResp.result().errcode())
        {
            std::cout << "rpc Register response success:" << registerResp.success() << std::endl;
        }
        else
        {
            std::cout << "rpc Register response error : " << registerResp.result().errmsg() << std::endl;
        }
    }

    return 0;
}