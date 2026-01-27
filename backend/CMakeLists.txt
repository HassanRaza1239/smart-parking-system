cmake_minimum_required(VERSION 3.10)
project(NexusPark VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/ParkingSystem.cpp
    src/Zone.cpp
    src/Vehicle.cpp
    src/ParkingRequest.cpp
    src/AllocationEngine.cpp
    src/PathFinder.cpp
    src/RollbackManager.cpp
    src/Analytics.cpp
    src/ParkingArea.cpp
    src/ParkingSlot.cpp
    main.cpp
)

# Main executable
add_executable(nexuspark ${SOURCES})

# Set properties
set_target_properties(nexuspark PROPERTIES
    OUTPUT_NAME "nexuspark"
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Installation
install(TARGETS nexuspark
    RUNTIME DESTINATION bin
)

# Testing (optional)
enable_testing()

# Add tests if test files exist
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test_allocator.cpp")
    add_executable(test_allocator test_allocator.cpp ${SOURCES})
    add_test(NAME test_allocator COMMAND test_allocator)
endif()

# Create build directory instructions
message(STATUS "==============================================")
message(STATUS "NexusPark - Smart Parking System")
message(STATUS "To build:")
message(STATUS "  mkdir build && cd build")
message(STATUS "  cmake ..")
message(STATUS "  cmake --build .")
message(STATUS "==============================================")
