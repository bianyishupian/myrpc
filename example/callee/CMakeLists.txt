set(SRC_LIST userService.cc ../user.pb.cc)
set(SRC_LIST_2 friendService.cc ../friend.pb.cc)

add_executable(provider ${SRC_LIST})
add_executable(provider2 ${SRC_LIST_2})
target_link_libraries(provider myrpc protobuf)
target_link_libraries(provider2 myrpc protobuf)