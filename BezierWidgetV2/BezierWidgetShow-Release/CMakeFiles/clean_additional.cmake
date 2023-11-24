# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "BezierWidgetShow_autogen"
  "CMakeFiles\\BezierWidgetShow_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\BezierWidgetShow_autogen.dir\\ParseCache.txt"
  )
endif()
