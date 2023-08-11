#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>

#include "rpcChannel.h"
#include "rpcheader.pb.h"
#include "rpcApplication.h"
#include "rpcController.h"

void MyrpcChannel::CallMethod(const google::protobuf::MethodDescriptor *method,
                              google::protobuf::RpcController *controller,
                              const google::protobuf::Message *request,
                              google::protobuf::Message *response,
                              google::protobuf::Closure *done)
{
    const google::protobuf::ServiceDescriptor *sd = method->service();
    std::string service_name = sd->name();
    std::string method_name = method->name();

    // 获取参数长度
    uint32_t args_size = 0;
    std::string args_str;
    if (request->SerializeToString(&args_str))
    {
        args_size = args_str.size();
    }
    else
    {
        controller->SetFailed("request serialize error!");
        return;
    }

    myRPC::rpcheader rpcHeader;
    rpcHeader.set_service_name(service_name);
    rpcHeader.set_method_name(method_name);
    rpcHeader.set_args_size(args_size);

    uint32_t header_size = 0;
    std::string header_str;
    // header序列化
    if (rpcHeader.SerializeToString(&header_str))
    {
        header_size = header_str.size();
    }
    else
    {
        controller->SetFailed("rpcHeader serialize error!");
        return;
    }

    // send_str : header_size + header_str + args_str
    std::string send_str; // 要发送的数据
    send_str.insert(0, std::string((char *)&header_size, 4));
    send_str += header_str;
    send_str += args_str;

    // 打印调试信息
    std::cout << "============================================" << std::endl;
    std::cout << "header_size: " << header_size << std::endl; 
    std::cout << "header_str: " << header_str << std::endl; 
    std::cout << "service_name: " << service_name << std::endl; 
    std::cout << "method_name: " << method_name << std::endl; 
    std::cout << "args_str: " << args_str << std::endl; 
    std::cout << "============================================" << std::endl;

    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == clientfd)
    {
        char errtest[512] = {0};
        sprintf(errtest, "create socket error! errno:%d", errno);
        controller->SetFailed(errtest);
        return;
    }

    std::string ip = MyrpcApplication::GetInstance().GetConfig().load("rpcserverip");
    uint16_t port = atoi(MyrpcApplication::GetInstance().GetConfig().load("rpcserverport").c_str());

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str());

    // connect
    if(-1 == connect(clientfd, (struct sockaddr *)&server_addr, sizeof(server_addr)))
    {
        close(clientfd);
        char errtest[512] = {0};
        sprintf(errtest, "connect socket error! errno:%d", errno);
        controller->SetFailed(errtest);
        return;
    }
    // send
    if(-1 == send(clientfd, send_str.c_str(), send_str.size(), 0))
    {
        char errtest[512] = {0};
        sprintf(errtest, "send socket error! errno:%d", errno);
        controller->SetFailed(errtest);
        close(clientfd);
        return;
    }
    // recv
    char recv_buf[1024] = {0};
    int recv_size = 0;
    if(-1 == (recv_size = recv(clientfd, recv_buf, 1024, 0)))
    {
        char errtest[512] = {0};
        sprintf(errtest, "recv socket error! errno:%d", errno);
        controller->SetFailed(errtest);
        close(clientfd);
        return;
    }
    // 反序列化
    // std::string response_str(recv_buf);
    // if(!response->ParseFromString(response_str)) 会出现问题，string遇到\0就结束了
    if(!response->ParseFromArray(recv_buf, recv_size))
    {
        char errtest[512] = {0};
        sprintf(errtest, "parse error! errno:%d", errno);
        controller->SetFailed(errtest);
        close(clientfd);
        return;
    }
    close(clientfd);
}