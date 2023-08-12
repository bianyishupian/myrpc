#pragma once
#include <semaphore.h>
#include <zookeeper/zookeeper.h>
#include <string>

class ZkClient
{
public:
    ZkClient();
    ~ZkClient();
    // 启动，连接ZK服务器
    void Start();
    // 根据路径path创建znode节点
    void Create(const char *path, const char *data, int datalen, int state = 0);
    // 获取path路径的节点的值
    std::string GetData(const char *path);
private:
    zhandle_t *zhandle_;
};