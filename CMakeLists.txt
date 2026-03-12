cmake_minimum_required(VERSION 3.14)
project(StopPow VERSION 0.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find GSL (portable, no hardcoded paths)
find_package(GSL REQUIRED)

# Library sources
set(LIB_SOURCES
    src/StopPow.cpp
    src/StopPow_Plasma.cpp
    src/StopPow_PartialIoniz.cpp
    src/StopPow_LP.cpp
    src/StopPow_BetheBloch.cpp
    src/StopPow_SRIM.cpp
    src/StopPow_AZ.cpp
    src/StopPow_Mehlhorn.cpp
    src/StopPow_Grabowski.cpp
    src/StopPow_Zimmerman.cpp
    src/StopPow_BPS.cpp
    src/StopPow_Fit.cpp
    src/PlotGen.cpp
    src/AtomicData.cpp
    src/Spectrum.cpp
    src/Fit.cpp
)

# Static library
add_library(StopPow STATIC ${LIB_SOURCES})
target_include_directories(StopPow PUBLIC src)
target_link_libraries(StopPow PUBLIC GSL::gsl GSL::gslcblas)

# Example executable
add_executable(StopPow_example examples/c++/main.cpp)
target_link_libraries(StopPow_example PRIVATE StopPow)

# Tests
enable_testing()

# Tests need to run from the test/ directory for data file paths
foreach(i RANGE 0 8)
    add_executable(test${i} test/test${i}.cpp)
    target_link_libraries(test${i} PRIVATE StopPow)
    add_test(
        NAME test${i}
        COMMAND test${i}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/test
    )
endforeach()
