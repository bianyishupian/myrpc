#!/bin/bash

set -e

# 如果没有build目录 创建该目录
if [ ! -d `pwd`/build ]; then
    mkdir `pwd`/build
fi

rm -fr `pwd`/build/*
cd `pwd`/build &&
    cmake .. &&
    make

# 回到项目根目录
cd ..

# 把头文件拷贝到 /usr/include/myrpc
if [ ! -d /usr/include/myrpc ]; then
    mkdir /usr/include/myrpc
fi


cd `pwd`/src/include
for header in `ls *.h`
do
    cp $header /usr/include/myrpc
done

cd ../..

# .so库拷贝到 /usr/lib
cp `pwd`/lib/libmyrpc.so /usr/lib
# flash
ldconfig