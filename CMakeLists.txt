# Title
cmake_minimum_required(VERSION 3.10.0)
project(2r3 VERSION 0.1.0 LANGUAGES C CXX)

if (MSVC)
   add_compile_options("/utf-8")
endif()

# Set C++ Standard To 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Output Error Message, Such as Undefined Reference
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR})

# Set Exeutable Name
set(TARGET ${PROJECT_NAME}_${CMAKE_SYSTEM_NAME})


# Find SDL3
find_package(SDL3 REQUIRED)

# Add New Include Path
add_executable(${TARGET}
                        "src/main.cpp"
                        )


target_include_directories(${TARGET} PRIVATE)               


# Find libs
target_link_libraries(${TARGET} 
                        SDL3::SDL3
                        )