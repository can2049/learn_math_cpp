

add_executable(ceresCurveFitting ceresCurveFitting.cc)
target_link_libraries(ceresCurveFitting PRIVATE ${CERES_LIBRARIES}  ceres )


add_executable(gaussNewton gaussNewton.cc)

add_executable(hello_ceres hello_ceres.cc)
target_link_libraries(hello_ceres PRIVATE ${CERES_LIBRARIES}  ceres )

add_executable(nonlinear_circle_fit nonlinear_circle_fit.cc)
target_link_libraries(nonlinear_circle_fit PRIVATE ${CERES_LIBRARIES}  ceres )
