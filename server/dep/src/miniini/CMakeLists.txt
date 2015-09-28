

#set(CMAKE_CXX_FLAGS "-fpermissive")


file(GLOB sources *.cpp)

set(miniini_STAT_SRCS
  ${sources}
)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/../../include/miniini
)

add_library(miniini STATIC ${miniini_STAT_SRCS})
