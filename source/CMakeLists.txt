add_library(Basis INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include/graph.hpp)

target_include_directories(Basis INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

