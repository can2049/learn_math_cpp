

add_executable(ceresCurveFitting ceresCurveFitting.cc)
target_link_libraries(ceresCurveFitting PRIVATE ${CERES_LIBRARIES}  ceres )


add_executable(gaussNewton gaussNewton.cc)
