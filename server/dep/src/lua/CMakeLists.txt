set(CMAKE_C_FLAGS "-g -Wall -DLUA_USE_LINUX")
file(GLOB sources *.c)

set(lua_STAT_SRCS
  ${sources}
)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(lua STATIC ${lua_STAT_SRCS})
