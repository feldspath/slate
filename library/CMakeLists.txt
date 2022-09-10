#allows to include slate as #include "slate/slate.hpp"
include_directories(${CMAKE_CURRENT_LIST_DIR})

#include slate lib directory
include("${CMAKE_CURRENT_LIST_DIR}/slate/CMakeLists.txt")
#include Third party
include("${CMAKE_CURRENT_LIST_DIR}/external/CMakeLists.txt")

# Allow to sort and explore source directories as a tree structure in Visual Studio
if(MSVC)
    source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES ${src_files_vcl} ${src_files_third_party})
endif()