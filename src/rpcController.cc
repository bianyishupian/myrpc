#include "rpcController.h"

MyrpcController::MyrpcController()
{
    failed_ = false;
    errText_ = "";
}
void MyrpcController::Reset()
{
    failed_ = false;
    errText_ = "";
}
bool MyrpcController::Failed() const
{
    return failed_;

}
std::string MyrpcController::ErrorText() const
{
    return errText_;
}
void MyrpcController::SetFailed(const std::string &reason)
{
    failed_ = true;
    errText_ = reason;
}

// TODO
void MyrpcController::StartCancel()
{
}
bool MyrpcController::IsCanceled() const
{
    return false;
}
void MyrpcController::NotifyOnCancel(google::protobuf::Closure *callback)
{
}