CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

ADD_LIBRARY(click
    driver.cc
    example_server.cc
    socket_send.cc
    timer_file_descriptor.cc
)

ADD_EXECUTABLE(click_example_server example_server.cc)
TARGET_LINK_LIBRARIES(click_example_server click zmq_util)

ADD_EXECUTABLE(click_example_client example_client.cc)
TARGET_LINK_LIBRARIES(click_example_client click zmq_util)
