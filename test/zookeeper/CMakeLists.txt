set(SRC_LIST zk_test.cc ../../src/rpcZookeeper.cc)
add_executable(test_zk ${SRC_LIST})
target_link_libraries(test_zk zookeeper_mt myrpc protobuf)