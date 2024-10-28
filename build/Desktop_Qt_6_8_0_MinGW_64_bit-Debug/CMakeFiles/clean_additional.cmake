# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\VacationApp_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\VacationApp_autogen.dir\\ParseCache.txt"
  "VacationApp_autogen"
  )
endif()
