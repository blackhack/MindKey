if(WIN32)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  add_definitions(-D_BUILD_DIRECTIVE=\\"$(ConfigurationName)\\")
elseif(UNIX)
  add_definitions(-D_BUILD_DIRECTIVE='"${CMAKE_BUILD_TYPE}"')

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  message(STATUS "Enabled c++11 support")
endif()
