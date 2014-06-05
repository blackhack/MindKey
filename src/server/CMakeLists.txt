file(GLOB_RECURSE sources_server ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

set(server_SRCS
  ${server_SRCS}
  ${sources_server}
)

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/src/shared
)

add_executable(server ${server_SRCS})

target_link_libraries(server
  shared
  ${Boost_LIBRARIES}
)