# Add all source files of slate library
file(
    GLOB_RECURSE
    src_files_slate
    ${CMAKE_CURRENT_LIST_DIR}/*.[ch]pp
)

configure_file(${CMAKE_CURRENT_LIST_DIR}/helper_dirs.in ${CMAKE_CURRENT_LIST_DIR}/helper_dirs.hpp)