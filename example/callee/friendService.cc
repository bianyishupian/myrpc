#include <iostream>
#include <string>
#include <vector>
#include "friend.pb.h"
#include "rpcProvider.h"
#include "rpcApplication.h"

class FriendService : public myRPC::FriendServiceRpc
{
public:
    // 本地
    std::vector<std::string> GetFriendsList(uint32_t userid)
    {
        std::cout << "do GetFriendsList service! userid:" << userid << std::endl;
        std::vector<std::string> vec;
        vec.push_back("wang wu");
        vec.push_back("zhao liu");
        vec.push_back("sun qi");
        return vec;
    }
    // 重写虚函数
    void GetFriendsList(::google::protobuf::RpcController *controller,
                        const ::myRPC::GetFriendsListRequest *request,
                        ::myRPC::GetFriendsListResponse *response,
                        ::google::protobuf::Closure *done)
    {
        uint32_t id = request->id();
        std::vector<std::string> friendsList = GetFriendsList(id);
        response->mutable_result()->set_errcode(0);
        response->mutable_result()->set_errmsg("");
        for (std::string &name : friendsList)
        {
            std::string *p = response->add_friends();
            *p = name;
        }
        done->Run();
    }
};

int main(int argc, char **argv)
{
    MyrpcApplication::Init(argc, argv);
    RpcProvider provider;
    // 服务发布
    provider.NotifyService(new FriendService());
    // 启动
    provider.Run();

    return 0;
}