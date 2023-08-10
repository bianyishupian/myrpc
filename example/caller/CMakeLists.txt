set(SRC_LIST callerUserService.cc ../user.pb.cc)

add_executable(consumer ${SRC_LIST})
target_link_libraries(consumer myrpc protobuf)