include(GoogleTest)

# Define executable targets
add_executable(GraphTests graphTest.cpp)

# Link targets
target_link_libraries(GraphTests PUBLIC
    GTest::gtest GTest::gtest_main
    Basis
)


target_include_directories(GraphTests PUBLIC
    ${PROJECT_SOURCE_DIR}/source/include
)

# Define tests
gtest_discover_tests(GraphTests)