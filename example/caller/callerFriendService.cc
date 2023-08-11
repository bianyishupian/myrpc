#include <iostream>
#include "rpcApplication.h"
#include "friend.pb.h"
#include "rpcChannel.h"
#include "rpcController.h"

int main(int argc, char **argv)
{
    MyrpcApplication::Init(argc, argv);

    myRPC::FriendServiceRpc_Stub stub(new MyrpcChannel());
    // rpc method的请求参数
    myRPC::GetFriendsListRequest request;
    request.set_id(1001);
    // rpc method响应
    myRPC::GetFriendsListResponse response;
    // 发起rpc方法的调用 实际上是调用了channel的CallMethod函数 是同步的
    MyrpcController controller;
    stub.GetFriendsList(&controller, &request, &response, nullptr);

    if (controller.Failed())
    {
        std::cout << controller.ErrorText() << std::endl;
    }
    else
    {
        if (0 == response.result().errcode())
        {
            std::cout << "rpc GetFriendsList response success!" << std::endl;
            int size = response.friends_size();
            for (int i = 0; i < size; ++i)
            {
                std::cout << "index:" << i << " name:" << response.friends(i) << std::endl;
            }
        }
        else
        {
            std::cout << "rpc GetFriendsList response error : " << response.result().errmsg() << std::endl;
        }
    }

    return 0;
}