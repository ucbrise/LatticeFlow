CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# CREATE_TEST(foo) creates a test named `foo` from the file `foo.cc`.
MACRO(CREATE_TEST NAME)
    ADD_EXECUTABLE(${NAME} ${NAME}.cc)
    ADD_TEST(${NAME} ${NAME})
    ADD_DEPENDENCIES(${NAME} googletest)
    TARGET_LINK_LIBRARIES(${NAME}
        ${GTEST_LIBS_DIR}/libgtest.a
        ${GTEST_LIBS_DIR}/libgtest_main.a
        pthread
    )
ENDMACRO(CREATE_TEST)

