include(GoogleTest)

# Define executable targets
add_executable(DirectedGraphTests directedGraph.cpp)
add_executable(UndirectedGraphTests undirectedGraph.cpp)

# TODO: make a loop with the targets as list elements instead of repeating the code below.

# Link targets
target_link_libraries(DirectedGraphTests PUBLIC
    GTest::gtest GTest::gtest_main
    Basis
)

target_include_directories(DirectedGraphTests PUBLIC
    ${PROJECT_SOURCE_DIR}/source/include
)

target_link_libraries(UndirectedGraphTests PUBLIC
    GTest::gtest GTest::gtest_main
    Basis
)

target_include_directories(UndirectedGraphTests PUBLIC
    ${PROJECT_SOURCE_DIR}/source/include
)

# Define tests
gtest_discover_tests(DirectedGraphTests)
gtest_discover_tests(UndirectedGraphTests)