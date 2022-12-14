
cmake_minimum_required(VERSION 3.2)

# List the files of the current local project 
#    Default behavior: Automatically add all hpp and cpp files from src/ directory
#    You may want to change this definition in case of specific file structure
file(GLOB_RECURSE src_files ${CMAKE_CURRENT_LIST_DIR}/src/*.[ch]pp)

# Generate the executable_name from the current directory name
get_filename_component(executable_name ${CMAKE_CURRENT_LIST_DIR} NAME)
# Another possibility is to set your own name: set(executable_name your_own_name) 
message(STATUS "Configure steps to build executable file [${executable_name}]")
project(${executable_name})

# Add current src/ directory
include_directories("src")

# Include files from the library (slate as well as external dependencies)
include("library/CMakeLists.txt")


# Add all files to create executable
#  @src_files: the local file for this project
#  @src_files_slate: all files of the slate library
#  @src_files_external: all third party libraries compiled with the project
add_executable(${executable_name} ${src_files_slate} ${src_files_external} ${src_files})
set_property(TARGET ${executable_name} PROPERTY CXX_STANDARD 17)


# Set Compiler for Unix system
if(UNIX)
   set(CMAKE_CXX_COMPILER g++)                      # Can switch to clang++ if prefered
   add_definitions(-g -O2 -std=c++17 -Wall -Wextra -pthread) # Can adapt compiler flags if needed
   add_definitions(-Wno-sign-compare -Wno-type-limits) # Remove some warnings
endif()

# Set Compiler for Windows/Visual Studio
if(MSVC)
    add_definitions(/MP /W4 /wd4244 /wd4127 /wd4267)   # Parallel build (/MP)
    source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${src_files})  # Allow to explore source directories as a tree in Visual Studio
    add_custom_command(TARGET ${executable_name} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${ASSIMP_DLL} $<TARGET_FILE_DIR:${executable_name}>) # Copy Assimp DLL to executable location
endif()

# Link libraries
target_link_libraries(${executable_name} ${GLFW_LIBRARIES} ${ASSIMP_LIBRARIES})

# Link options for Unix
if(UNIX)
   target_link_libraries(${executable_name} dl pthread) #dlopen is required by Glad on Unix
endif()