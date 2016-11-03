CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

ADD_LIBRARY(zmq_util
    connection_id.cc
    enveloped_message.cc
    hexdump.cc
    msg_util.cc
    zmq_util.cc
)
TARGET_LINK_LIBRARIES(zmq_util zmq)
