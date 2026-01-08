# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\MultiScope_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\MultiScope_autogen.dir\\ParseCache.txt"
  "MultiScope_autogen"
  )
endif()
