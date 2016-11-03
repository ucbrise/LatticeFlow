CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

CREATE_NAMED_TEST(click_counter_test counter_test.cc)
CREATE_NAMED_TEST(click_dedup_test dedup_test.cc)
CREATE_NAMED_TEST(click_drop_test drop_test.cc)
CREATE_NAMED_TEST(click_dup_test dup_test.cc)
CREATE_NAMED_TEST(click_filter_test filter_test.cc)
CREATE_NAMED_TEST(click_group_by_test group_by_test.cc)
CREATE_NAMED_TEST(click_id_test id_test.cc)
CREATE_NAMED_TEST(click_if_test if_test.cc)
CREATE_NAMED_TEST(click_join_test join_test.cc)
CREATE_NAMED_TEST(click_lambda_test lambda_test.cc)
# CREATE_NAMED_TEST(click_map_test map_test.cc)
CREATE_NAMED_TEST(click_queue_test queue_test.cc)
CREATE_NAMED_TEST(click_register_test register_test.cc)
CREATE_NAMED_TEST(click_round_robin_test round_robin_test.cc)
CREATE_NAMED_TEST(click_tee_test tee_test.cc)

ADD_EXECUTABLE(click_example_server
    driver.cc
    example_server.cc
    socket_recv.cc
)
TARGET_LINK_LIBRARIES(click_example_server zmq_util)
