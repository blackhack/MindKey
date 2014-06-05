file(GLOB_RECURSE sources_capturer ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

set(capturer_SRCS
  ${capturer_SRCS}
  ${sources_capturer}
)

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/src/shared
)

add_executable(capturer ${capturer_SRCS})

set_target_properties(capturer PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")

target_link_libraries(capturer
  shared
  ${Boost_LIBRARIES}
)