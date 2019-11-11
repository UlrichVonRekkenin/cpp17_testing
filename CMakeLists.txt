cmake_minimum_required(VERSION 3.15)
project(cpp17learning)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(hell src/mtp_hell.cpp)
add_executable(bjorn src/bjorn.cpp)
add_executable(lambdas src/lambdas.cpp)
add_executable(lambdas_op src/lambdas_op.cpp)
add_executable(inh src/inhereited.cpp)
add_executable(tree src/tree.cpp)
