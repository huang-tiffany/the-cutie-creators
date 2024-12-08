# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles/StaticGLEW_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/StaticGLEW_autogen.dir/ParseCache.txt"
  "CMakeFiles/the-cutie-creators_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/the-cutie-creators_autogen.dir/ParseCache.txt"
  "StaticGLEW_autogen"
  "freetype/CMakeFiles/freetype_autogen.dir/AutogenUsed.txt"
  "freetype/CMakeFiles/freetype_autogen.dir/ParseCache.txt"
  "freetype/freetype_autogen"
  "the-cutie-creators_autogen"
  )
endif()
