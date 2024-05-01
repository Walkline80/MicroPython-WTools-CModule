# Create an INTERFACE library for our C module.
add_library(usermod_wtools INTERFACE)

# Add our source files to the lib
target_sources(usermod_wtools INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/modwtools.c
    ${CMAKE_CURRENT_LIST_DIR}/classes/arraytool.c
    ${CMAKE_CURRENT_LIST_DIR}/classes/colortool.c
)

# Add the current directory as an include directory.
target_include_directories(usermod_wtools INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/classes
)

# Link our INTERFACE library to the usermod target.
target_link_libraries(usermod INTERFACE usermod_wtools)
