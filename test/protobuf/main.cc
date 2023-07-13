#include <iostream>
#include <string>

#include "test.pb.h"

int main()
{
    testprotobuf::GetFriendListResponse flr;
    testprotobuf::ResultCode *rc = flr.mutable_result();
    rc->set_errorno(0);

    testprotobuf::User *user1 = flr.add_friendlist();
    user1->set_name("zhang san");
    user1->set_age(20);
    user1->set_sex(testprotobuf::User::MAN);
    std::cout << flr.friendlist_size() << '\n';

    testprotobuf::User *user2 = flr.add_friendlist();
    user2->set_name("li si");
    user2->set_age(21);
    user2->set_sex(testprotobuf::User::WOMAN);
    std::cout << flr.friendlist_size() << '\n';

    testprotobuf::User user = flr.friendlist(2);

    return 0;
}


int main2()
{
    testprotobuf::Response rsp;
    testprotobuf::ResultCode *rc = rsp.mutable_result();
    rc->set_errorno(1);
    rc->set_errormsg("失败！");
    std::cout << rc->errorno() << rc->errormsg() << '\n';
    std::string s;
    rsp.SerializeToString(&s);
    std::cout << s << '\n';

    testprotobuf::Response rspB;
    rspB.ParseFromString(s);
    testprotobuf::ResultCode rcB = rspB.result();
    std::cout << rcB.errorno() << ' ' << rcB.errormsg() << ' ';
    std::cout << rspB.success() << '\n';

    return 0;
}

int main1()
{
    testprotobuf::LoginRequest request;
    request.set_name("zhao");
    request.set_pwd("123456");
    std::string sendMessage;
    if (request.SerializeToString(&sendMessage))
    {
        std::cout << sendMessage << "\n";
    }
    testprotobuf::LoginRequest reqB;
    if (reqB.ParseFromString(sendMessage))
    {
        std::cout << reqB.name() << reqB.pwd() << '\n';
    }
    return 0;
}