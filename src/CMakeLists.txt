add_subdirectory(shared)
add_subdirectory(server)

if(WIN32)
add_subdirectory(capturer)
endif()