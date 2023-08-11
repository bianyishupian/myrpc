set(SRC_LIST callerUserService.cc ../user.pb.cc)
set(SRC_LIST_2 callerFriendService.cc ../user.pb.cc)

add_executable(consumer ${SRC_LIST})
add_executable(consumer2 ${SRC_LIST_2})

target_link_libraries(consumer myrpc protobuf)
target_link_libraries(consumer2 myrpc protobuf)