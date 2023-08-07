#pragma once

#include <unordered_map>
#include <string>

class RpcConfig
{
public:
    void loadConfigFile(const char *configFile);
    std::string load(const std::string &key);

private:
    std::unordered_map<std::string, std::string> configMap_;
};