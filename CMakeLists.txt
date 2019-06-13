cmake_minimum_required(VERISON 3.14)
project(RDAgent)

add_library(
    random
    random.h
    random.cpp
)

add_library(
    VectorMatrix
    VectorMatrix.h
    VectorMatrix.cpp
)

add_library(
    VisualObject
    VisualObject.h
    VisualObject.cpp
)

add_library(
    RDControl
    RDControl.h
    RDControl.cpp
)

add_library(
    RDControl
    RDControl.h
    RDControl.cpp
)

add_library(
    AgentInterface
    AgentInterface.h
    AgentInterface.cpp
)

add_library(
    TSearch
    TSearch.h
    TSearch.cpp
)

add_executable( RDTask RDTask.cpp)