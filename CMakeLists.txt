cmake_minimum_required(VERSION 3.13)
project(Kaleidoscope)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_executable(kaleidoscope src/main.cpp src/codegen.cpp)

llvm_map_components_to_libnames(llvm_libs
  core
  orcjit
  native
  irreader
  passes
)

target_link_libraries(kaleidoscope PRIVATE ${llvm_libs})

if(UNIX)
  target_link_options(kaleidoscope PRIVATE -rdynamic)
endif()
