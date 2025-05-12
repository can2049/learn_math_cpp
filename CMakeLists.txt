cmake_minimum_required(VERSION 3.10)

project(learn_math)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE RelWithDebInfo)
# set(CMAKE_BUILD_TYPE Release)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})


message(STATUS "CMAKE_PREFIX_PATH = ${CMAKE_PREFIX_PATH}")
message(STATUS "CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}")


# OpenCV
# find_package(OpenCV REQUIRED)
# include_directories(${OpenCV_INCLUDE_DIRS})

# eigen
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIRS})

# Ceres
find_package(Ceres REQUIRED HINTS )
include_directories(${CERES_INCLUDE_DIRS})

# # glog
# find_package(Glog REQUIRED)
# include_directories(${GLOG_INCLUDE_DIRS})


# g2o
# find_package(G2O REQUIRED)
# include_directories(${G2O_INCLUDE_DIRS})


find_package(Sophus REQUIRED)
include_directories( ${Sophus_INCLUDE_DIRS} )
message(STATUS "Sophus_INCLUDE_DIRS = ${Sophus_INCLUDE_DIRS}")


add_subdirectory(gnuplot_demo)
add_subdirectory(use_ceres)
add_subdirectory(use_sophus)
