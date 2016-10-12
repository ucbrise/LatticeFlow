CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

CREATE_TEST(cross_test)
CREATE_TEST(empty_test)
CREATE_TEST(exchange_test)
CREATE_TEST(filter_test)
CREATE_TEST(iterator_test)
CREATE_TEST(map_test)
CREATE_TEST(naive_equijoin_test)

TARGET_LINK_LIBRARIES(cross_test boost_context)
TARGET_LINK_LIBRARIES(naive_equijoin_test boost_context)

CREATE_BENCHMARK(filter_bench)
