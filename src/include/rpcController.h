#include <google/protobuf/service.h>
#include <string>

class MyrpcController : public google::protobuf::RpcController
{
public:
    MyrpcController();
    void Reset();
    bool Failed() const;
    std::string ErrorText() const;
    void SetFailed(const std::string &reason);
    
    // TODO
    void StartCancel();
    bool IsCanceled() const;
    void NotifyOnCancel(google::protobuf::Closure *callback);

private:
    bool failed_;
    std::string errText_;
};
