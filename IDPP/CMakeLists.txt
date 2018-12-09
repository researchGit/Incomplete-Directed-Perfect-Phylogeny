cmake_minimum_required(VERSION 3.12)
project(IDPP)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(IDPP
        idp_A_fast.cpp
        idp_A_fast.h
        idp_depthFirstTraversal.cpp
        idp_depthFirstTraversal.h
        idp_generator.cpp
        idp_generator.h
        idp_hdt.cpp
        idp_hdt.h
        idp_inputReader.cpp
        idp_inputReader.h
        idp_inputSetup.cpp
        idp_inputSetup.h
        idp_matrixTransform.cpp
        idp_matrixTransform.h
        idp_structure.h
        idp_treap.cpp
        idp_treap.h
        main.cpp)
