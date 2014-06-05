file(GLOB_RECURSE sources_Shared ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

set(Shared_SRCS
  ${Shared_SRCS}
  ${sources_Shared}
)

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/src/capturer
  ${CMAKE_SOURCE_DIR}/src/server
)

add_library(shared STATIC ${Shared_SRCS})