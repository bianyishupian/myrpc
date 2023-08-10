#pragma once
#include <string>
#include <functional>
#include <unordered_map>
#include <mymuduo11/net/tcpserver.h>
#include <mymuduo11/net/eventloop.h>
#include <google/protobuf/service.h>
#include <google/protobuf/descriptor.h>

class RpcProvider
{
public:
    // 这里是框架提供给外部使用的，可以发布rpc方法的函数接口
    void NotifyService(google::protobuf::Service *service);
    // 启动rpc服务节点，开始提供rpc远程网络调用服务
    void Run();
private:
    EventLoop loop_;
    struct ServiceInfo
    {
        google::protobuf::Service *service_;
        std::unordered_map<std::string, const google::protobuf::MethodDescriptor*> methodMap_;
    };
    std::unordered_map<std::string, ServiceInfo> serviceMap_;
    void onConnection(const TcpConnectionPtr&);
    void onMessage(const TcpConnectionPtr&, Buffer*, Timestamp);
    void sendRpcResponse(const TcpConnectionPtr&, google::protobuf::Message*);
};