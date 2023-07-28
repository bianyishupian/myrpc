#pragma once

class myrpcApplication
{
public:
    static void Init(int argc, char **argv);
    static myrpcApplication &GetInstance();

private:
    myrpcApplication() {}
    myrpcApplication(const myrpcApplication &) = delete;
    myrpcApplication(myrpcApplication &&) = delete;
};