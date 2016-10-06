CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

ADD_LIBRARY(concurrency
    barrier.cc
)

CREATE_TEST(queue_test)
CREATE_TEST(bounded_queue_test)
