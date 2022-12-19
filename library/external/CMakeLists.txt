# Add all source files of external libraries
set(src_files_glad "${CMAKE_CURRENT_LIST_DIR}/glad/src/glad.c")
FILE(GLOB src_files_imgui "${CMAKE_CURRENT_LIST_DIR}/imgui/include/imgui/*.cpp")

set(src_files_external ${src_files_glad} ${src_files_imgui})

# Add glm and glad include folders
include_directories("${CMAKE_CURRENT_LIST_DIR}/glm/include")
include_directories("${CMAKE_CURRENT_LIST_DIR}/glad/include/")

# ImGui include folder
include_directories("${CMAKE_CURRENT_LIST_DIR}/imgui/include/")

# OpenBLAS include directories
include_directories("${CMAKE_CURRENT_LIST_DIR}/openblas/include")

# Enable IMGUI to work with GLAD
# add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLAD)

# Include GLFW and Assimp lib for Unix
if(UNIX)
    #expect GLFW3 already installed on the system
    find_package(glfw3 REQUIRED)
    find_package(PkgConfig REQUIRED)
    pkg_search_module(GLFW REQUIRED glfw3)
    include_directories(${GLFW_INCLUDE_DIRS})
    
    find_package(assimp REQUIRED)
    find_package(OpenBLAS REQUIRED)
endif()

# Include GLFW and Assimp precompiled lib for Windows/Visual Studio
if(MSVC)
    set(GLFW_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/glfw/include")
    include_directories(${GLFW_INCLUDE_DIRS})
    set(GLFW_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/glfw/lib/glfw3.lib")

    set(OpenBLAS_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/openblas/lib/openblas.lib")

    set(ASSIMP_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/assimp/include")
    include_directories(${ASSIMP_INCLUDE_DIRS})
    set(ASSIMP_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/assimp/lib/assimp-vc143-mtd.lib")
    set(ASSIMP_DLL "${CMAKE_CURRENT_LIST_DIR}/assimp/lib/assimp-vc143-mtd.dll")
endif()

