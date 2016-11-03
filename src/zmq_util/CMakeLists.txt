CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

ADD_LIBRARY(zmq_util
    zmq_util.cc
    hexdump.cc
)
TARGET_LINK_LIBRARIES(zmq_util zmq)
