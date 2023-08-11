#include <iostream>
#include <string>
#include "user.pb.h"
#include "rpcProvider.h"
#include "rpcApplication.h"

class UserService : public myRPC::UserServiceRpc
{
public:
    bool Login(std::string name, std::string pwd)
    {
        std::cout << "doing local service: login" << std::endl;
        std::cout << "name:" << name << " pwd:" << pwd << std::endl;
        return true;
    }
    bool Register(uint32_t id, std::string name, std::string pwd)
    {
        std::cout << "doing local service: Register" << std::endl;
        std::cout << "id:" << id << " name:" << name << " pwd:" << pwd << std::endl;
        return true;
    }
    void Login(::google::protobuf::RpcController *controller,
               const ::myRPC::LoginRequest *request,
               ::myRPC::LoginResponse *response,
               ::google::protobuf::Closure *done)
    {
        std::string name = request->name();
        std::string pwd = request->pwd();

        bool login_result = Login(name, pwd);

        myRPC::ResultCode *code = response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg("");
        response->set_success(login_result);

        done->Run();
    }
    void Register(::google::protobuf::RpcController *controller,
                  const ::myRPC::RegisterRequest *request,
                  ::myRPC::RegisterResponse *response,
                  ::google::protobuf::Closure *done)
    {
        uint32_t id = request->id();
        std::string name = request->name();
        std::string pwd = request->pwd();

        bool ret = Register(id, name, pwd);

        response->mutable_result()->set_errcode(0);
        response->mutable_result()->set_errmsg("");
        response->set_success(ret);

        done->Run();
    }

};

int main(int argc, char **argv)
{
    MyrpcApplication::Init(argc, argv);

    RpcProvider provider;
    provider.NotifyService(new UserService());
    provider.Run();

    return 0;
}