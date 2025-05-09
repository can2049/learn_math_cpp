#!/bin/bash

SCRIPT_DIR=$(
    cd $(dirname $0)
    pwd
)

INSTALL_PATH=~/data/third_party_install/
JOB_NUM=16

SCRIPT_DIR=$(
    cd $(dirname $0)
    pwd
)

# 解析命令行参数
while getopts "i:j:" opt; do
    case $opt in
    i)
        INSTALL_PATH=$OPTARG
        ;;
    j)
        JOB_NUM=$OPTARG
        ;;
    *)
        echo "Usage: $0 [-i INSTALL_PATH] [-j JOB_NUM]"
        exit 1
        ;;
    esac
done

export CMAKE_MODULE_PATH=${INSTALL_PATH}:$CMAKE_MODULE_PATH
export CMAKE_PREFIX_PATH=${INSTALL_PATH}:$CMAKE_PREFIX_PATH

mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo \
    -DCMAKE_INSTALL_PREFIX=${INSTALL_PATH} \
    -DCMAKE_PREFIX_PATH=${INSTALL_PATH} \
    -DCMAKE_MODULE_PATH=${INSTALL_PATH} \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    -DCeres_DIR=${INSTALL_PATH}/lib/cmake/Ceres

make -j${JOB_NUM} # VERBOSE=0
