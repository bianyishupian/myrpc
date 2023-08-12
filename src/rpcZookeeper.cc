#include "include/rpcZookeeper.h"
#include "include/rpcApplication.h"

#include <iostream>

// watcher
void global_watcher(zhandle_t *zh, int type, int state, const char *path, void *watcherCtx)
{
    if(type == ZOO_SESSION_EVENT)
    {
        if(state == ZOO_CONNECTED_STATE)
        {
            sem_t *sem = (sem_t *)zoo_get_context(zh);
            sem_post(sem);
        }
    }
}

ZkClient::ZkClient() : zhandle_(nullptr)
{
}
ZkClient::~ZkClient()
{
    if(zhandle_ != nullptr)
    {
        zookeeper_close(zhandle_);
    }
}
// 启动，连接ZK服务器
void ZkClient::Start()
{
    std::string host = MyrpcApplication::GetConfig().load("zookeeperip");
    std::string port = MyrpcApplication::GetConfig().load("zookeeperport");
    std::string connstr = host + ':' + port;

    zhandle_ = zookeeper_init(connstr.c_str(), global_watcher, 30000, nullptr, nullptr, 0);
    
    if(zhandle_ == nullptr)
    {
        std::cout << "zookeeper_init error!" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    sem_t sem;
    sem_init(&sem, 0, 0);
    zoo_set_context(zhandle_, &sem);
    
    sem_wait(&sem);
    std::cout << "zookeeper_init success!" << std::endl;
}
// 根据路径path创建znode节点
void ZkClient::Create(const char *path, const char *data, int datalen, int state)
{
    char path_buffer[128];
    int bufferlen = sizeof(path_buffer);
    int flag;

    flag = zoo_exists(zhandle_, path, 0, nullptr);
    if(ZNONODE == flag)
    {
        flag = zoo_create(zhandle_, path, data, datalen, &ZOO_OPEN_ACL_UNSAFE, state, path_buffer, bufferlen);
        if(flag == ZOK)
        {
            std::cout << "znode create success... path:" << path << std::endl;
        }
        else
        {
            std::cout << "flag:" << flag << std::endl;
            std::cout << "znode create error... path:" << path << std::endl;
			exit(EXIT_FAILURE);
        }
    }
}
// 获取path路径的节点的值
std::string ZkClient::GetData(const char *path)
{
    char buffer[64];
    int bufferlen = sizeof(buffer);
    int flag = zoo_get(zhandle_, path, 0, buffer, &bufferlen, nullptr);
    if(flag != ZOK)
    {
        std::cout << "get znode error... path:" << path << std::endl;
        return "";
    }
    else
    {
        return buffer;
    }
}