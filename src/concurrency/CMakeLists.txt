CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

ADD_LIBRARY(concurrency
    barrier.cc
    thread_pool.cc
)

CREATE_TEST(bounded_queue_test)
CREATE_TEST(queue_test)
CREATE_TEST(thread_pool_test)

TARGET_LINK_LIBRARIES(thread_pool_test concurrency)
