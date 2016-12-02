CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

ADD_EXECUTABLE(click_example_server
    driver.cc
    example_server.cc
    socket_send.cc
    timer_file_descriptor.cc
)
TARGET_LINK_LIBRARIES(click_example_server zmq_util)

ADD_EXECUTABLE(click_example_client example_client.cc)
TARGET_LINK_LIBRARIES(click_example_client zmq_util)
