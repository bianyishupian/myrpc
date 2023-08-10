#include <mymuduo11/base/logger.h>

#include "rpcProvider.h"
#include "myrpcApplication.h"
#include "rpcheader.pb.h"

// 这里是框架提供给外部使用的，可以发布rpc方法的函数接口
void RpcProvider::NotifyService(google::protobuf::Service *service)
{
    ServiceInfo service_info;
    const google::protobuf::ServiceDescriptor *psd = service->GetDescriptor(); // 获取服务描述，protobuf提供的
    std::string service_name = psd->name();                                    // 获取服务名字
    int methodCount = psd->method_count();                                     // 获取服务中方法的数量

    LOG_INFO("service_name:%s", service_name.c_str());

    for (int i = 0; i < methodCount; ++i)
    {
        const google::protobuf::MethodDescriptor *pmd = psd->method(i); // 通过服务描述获取各个方法的描述
        std::string method_name = pmd->name();                          // 获取方法名字
        service_info.methodMap_.insert({method_name, pmd});             // 将方法名与方法描述插入到methodMap中
        LOG_INFO("method_name:%s", method_name.c_str());
    }
    service_info.service_ = service;
    serviceMap_.insert({service_name, service_info}); // 保存到serviceMap_中
}
void RpcProvider::Run()
{
    std::string ip = MyrpcApplication::GetInstance().GetConfig().load("rpcserverip");
    uint16_t port = atoi(MyrpcApplication::GetInstance().GetConfig().load("rpcserverport").c_str());
    InetAddress address(port, ip);
    // 创建TCPServer对象
    TcpServer server(&loop_, address, "RpcProvider");
    // 绑定回调
    server.setConnectionCallback(std::bind(&RpcProvider::onConnection, this, std::placeholders::_1));
    server.setMessageCallback(std::bind(&RpcProvider::onMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    // 设置线程数量
    server.setThreadNum(4);

    // server启动
    server.start();
    loop_.loop();
}

void RpcProvider::onConnection(const TcpConnectionPtr &conn)
{
    if (!conn->connected())
    {
        // 断开连接
        conn->shutdown();
    }
}
void RpcProvider::onMessage(const TcpConnectionPtr &conn, Buffer *buffer, Timestamp)
{
    std::string recv_buf = buffer->retrieveAllAsString();
    uint32_t header_size = 0;
    // 读取前4字节 将4字节的整数数据拷贝到header_size中
    recv_buf.copy((char *)&header_size, 4, 0);

    std::string header_str = recv_buf.substr(4, header_size);
    myRPC::rpcheader rpc_header;
    std::string service_name;
    std::string method_name;
    uint32_t args_size;
    if (rpc_header.ParseFromString(header_str))
    {
        // 反序列化成功
        service_name = rpc_header.service_name();
        method_name = rpc_header.method_name();
        args_size = rpc_header.args_size();
    }
    else
    {
        // 反序列化失败
        LOG_ERROR("header_str: %s parse error!\n", header_str.c_str());
        return;
    }

    std::string args_str = recv_buf.substr(4 + header_size, args_size);

    // 打印调试信息
    std::cout << "============================================" << std::endl;
    std::cout << "header_size: " << header_size << std::endl;
    std::cout << "rpc_header_str: " << header_str << std::endl;
    std::cout << "service_name: " << service_name << std::endl;
    std::cout << "method_name: " << method_name << std::endl;
    std::cout << "args_str: " << args_str << std::endl;
    std::cout << "============================================" << std::endl;

    auto it = serviceMap_.find(service_name);
    if (it == serviceMap_.end())
    {
        LOG_ERROR("%s is not exist!\n", service_name.c_str());
        return;
    }

    auto mit = it->second.methodMap_.find(method_name);
    if (mit == it->second.methodMap_.end())
    {
        LOG_ERROR("%s is not exist!\n", method_name.c_str());
        return;
    }
    // 获取service对象
    google::protobuf::Service *service = it->second.service_;
    // 获取method描述符
    const google::protobuf::MethodDescriptor *method = mit->second;

    google::protobuf::Message *request = service->GetRequestPrototype(method).New();
    if(!request->ParseFromString(args_str))
    {
        LOG_ERROR("request parse error, content:%s\n", args_str.c_str());
        return;
    }

    google::protobuf::Message *response = service->GetResponsePrototype(method).New();

    google::protobuf::Closure *done = google::protobuf::NewCallback<RpcProvider, const TcpConnectionPtr&,
                                                        google::protobuf::Message*>
                                                        (this, &RpcProvider::sendRpcResponse, conn, response);

    service->CallMethod(method, nullptr, request, response, done);
}
void RpcProvider::sendRpcResponse(const TcpConnectionPtr &conn, google::protobuf::Message *response)
{
    std::string response_str;
    if(response->SerializeToString(&response_str))
    {
        // 序列化成功
        conn->send(response_str);
    }
    else
    {
        // 序列化失败
        LOG_ERROR("serialize response_str error!\n");
    }
    conn->shutdown();   // 短连接
}