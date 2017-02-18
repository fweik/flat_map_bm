cmake_minimum_required(VERSION 3.0)

project(flat_map_bm)

enable_language(CXX)

set(PROJECT_VERSION 0.1-dev)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Write compile commands to file, for various tools...
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# We need at least c++11
include(RequireCXX11)

find_package(benchmark REQUIRED)
include_directories(${benchmark_INCLUDE_DIR})

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

function(BENCHMARK NAME SRC)
  add_executable(${NAME} "${SRC}")
  target_link_libraries(${NAME} ${benchmark_LIBRARIES} ${Boost_LIBRARIES})
endfunction()

benchmark(flat_map_bm "flat_map_bm.cpp")
