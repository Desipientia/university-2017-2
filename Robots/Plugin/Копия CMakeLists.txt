add_library(model_socket SHARED model_socket.cc)
target_link_libraries(model_socket ${GAZEBO_LIBRARIES} ${Boost_LIBRARIES})