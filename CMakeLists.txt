

find_package(cmakego COMPONENTS yaml REQUIRED)
add_definitions(--std=c++11)
add_executable(test test.cpp)
target_link_libraries(test p::yaml)