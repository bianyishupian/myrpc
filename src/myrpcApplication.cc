#include "include/myrpcApplication.h"

void myrpcApplication::Init(int argc, char **argv)
{

}
myrpcApplication &myrpcApplication::GetInstance()
{
    static myrpcApplication app;
    return app;
}