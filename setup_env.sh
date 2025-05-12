#!/bin/bash

INSTALL_PATH=~/data/third_party_install/
DOWNLOAD_DIR=~/data/third_party_download/
JOB_NUM=16

SCRIPT_DIR=$(
    cd $(dirname $0)
    pwd
)

# 解析命令行参数
while getopts "i:d:j:" opt; do
    case $opt in
    i)
        INSTALL_PATH=$OPTARG
        ;;
    d)
        DOWNLOAD_DIR=$OPTARG
        ;;
    j)
        JOB_NUM=$OPTARG
        ;;
    *)
        echo "Usage: $0 [-i INSTALL_PATH] [-d DOWNLOAD_DIR] [-j JOB_NUM]"
        exit 1
        ;;
    esac
done

export CMAKE_COMMON_OPTIONS=" -DBUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON -DCMAKE_CXX_STANDARD=17 -DCMAKE_INSTALL_PREFIX=${INSTALL_PATH} -DCMAKE_PREFIX_PATH=${INSTALL_PATH} -DCMAKE_EXPORT_COMPILE_COMMANDS=ON "

mkdir -p ${INSTALL_PATH}
mkdir -p ${DOWNLOAD_DIR}

if [ -f /etc/bash.bashrc ]; then
    source /etc/bash.bashrc
fi

function download_install_ceres() {
    echo "===== download_install_ceres ====="
    cd ${DOWNLOAD_DIR}
    local tag=2.2.0
    # if dir not exist, clone it
    if [ ! -d ceres-solver ]; then
        git clone --depth 1 -b ${tag} https://gitee.com/mirrors/ceres-solver.git
        cd ceres-solver
    else
        cd ceres-solver
        git fetch --depth 1 origin tag ${tag}
        git reset --hard tags/${tag}
    fi
    mkdir -p build
    cd build
    cmake ${CMAKE_COMMON_OPTIONS} ..
    make install -j${JOB_NUM}
}

function download_install_eigen() {
    echo "===== download_install_eigen ====="
    local tag=3.3.9
    cd ${DOWNLOAD_DIR}
    # if dir not exist, clone it
    if [ ! -d eigen ]; then
        git clone --depth 1 -b ${tag} https://gitlab.com/libeigen/eigen.git
        cd eigen
    else
        cd eigen
        git fetch --depth 1 origin tag ${tag}
        git reset --hard tags/${tag}
    fi
    mkdir -p build
    cd build
    cmake ${CMAKE_COMMON_OPTIONS} ..
    make install -j${JOB_NUM}
}

function download_install_sophus() {
    echo "===== download_install_sophus ====="
    local tag=1.22.10
    cd ${DOWNLOAD_DIR}
    # if dir not exist, clone it
    if [ ! -d Sophus ]; then
        git clone --depth 1 -b ${tag} https://github.com/strasdat/Sophus.git
        cd Sophus
    else
        cd Sophus
        git fetch --depth 1 origin tag ${tag}
        git reset --hard tags/${tag}
    fi
    mkdir -p build
    cd build
    cmake ${CMAKE_COMMON_OPTIONS} ..
    make install -j${JOB_NUM}
}

################

download_install_eigen
download_install_ceres
download_install_sophus
