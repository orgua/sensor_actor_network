cmake_minimum_required(VERSION 3.3)
project(sensor_actor_network)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

set(SOURCE_FILES
        main.cpp
        src/layer_application.h
        src/stack_message.h
        src/layer_application.cpp
        src/layer_interface.h
        src/layer_datalink.h
        src/layer_session.h
        src/layer_network.h
        src/layer_transceiveLoopback.h
        src/stack_management.h

        )

add_executable(sensor_actor_network ${SOURCE_FILES}
        src/layer_interface.h
        src/layer_transceiveLoopback.h
        src/stack_management.h)