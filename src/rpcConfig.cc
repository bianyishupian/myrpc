#include <iostream>
#include <string>

#include "rpcConfig.h"

// 去掉字符串前后的空格
void Trim(std::string &src_buf)
{
    int idx = src_buf.find_first_not_of(' ');
    if (idx != -1)
    {
        // 说明字符串前面有空格
        src_buf = src_buf.substr(idx, src_buf.size() - idx);
    }
    // 去掉字符串后面多余的空格
    idx = src_buf.find_last_not_of(' ');
    if (idx != -1)
    {
        // 说明字符串后面有空格
        src_buf = src_buf.substr(0, idx + 1);
    }
}

void RpcConfig::loadConfigFile(const char *configFile)
{
    FILE *file = fopen(configFile, "r");
    if (nullptr == file)
    {
        std::cout << configFile << " open failure!" << std::endl;
        exit(EXIT_FAILURE);
    }
    while (!feof(file))
    {
        char buf[512] = {0};
        fgets(buf, 512, file);
        std::string read_buf(buf);
        Trim(read_buf);
        if (read_buf[0] == '#' || read_buf.empty())
        {
            continue;
        }
        int idx = read_buf.find('=');
        if(idx == -1)
        {
            continue;
        }
        std::string key, value;
        key = read_buf.substr(0,idx);
        Trim(key);
        int endidx = read_buf.find('\n', idx);
        value = read_buf.substr(idx+1,endidx-idx-1);
        Trim(value);
        configMap_.insert({key,value});
    }
    fclose(file);
}
std::string RpcConfig::load(const std::string &key)
{
    auto it = configMap_.find(key);
    if(it == configMap_.end()) return "";
    return it->second;
}