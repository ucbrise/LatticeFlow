CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# CREATE_BENCHMARK(foo) creates a benchmark named `foo` from the file `foo.cc`.
MACRO(CREATE_BENCHMARK NAME)
    ADD_EXECUTABLE(${NAME} ${NAME}.cc)
    ADD_DEPENDENCIES(${NAME} googlebenchmark)
    TARGET_LINK_LIBRARIES(${NAME}
        ${GBENCH_LIBS_DIR}/libbenchmark.a
        pthread
    )
ENDMACRO(CREATE_BENCHMARK)
