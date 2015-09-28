

set(CMAKE_CXX_FLAGS "-fpermissive")


file(GLOB sources *.cxx)

set(zthread_STAT_SRCS
  ${sources}
)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)

add_library(zthread STATIC ${zthread_STAT_SRCS})
