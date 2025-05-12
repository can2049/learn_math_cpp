message(STATUS "Sophus_LIBRARIES = ${Sophus_LIBRARIES}")

add_executable(so_demo so_demo.cc)
target_link_libraries(so_demo Sophus::Sophus)

add_executable(se_demo se_demo.cc)
target_link_libraries(se_demo Sophus::Sophus)
