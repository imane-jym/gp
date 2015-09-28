#set(CMAKE_CXX_FLAGS "-fpermissive -p -O2")
set(CMAKE_CXX_FLAGS "-fpermissive -p -g -Wall -pthread -DCC_TARGET_PLATFORM=linux")
#SET(CMAKE_BUILD_TYPE Release)
SET(CMAKE_BUILD_TYPE Debug)
file(GLOB_RECURSE sources_SERVER_SHARE ../pitayaserver/framework/*.cpp ../pitayaserver/framework/*.h)
file(GLOB_RECURSE sources_SHARE ../../share/*.cpp ../../share/*.h)
file(GLOB sources_remove Lua*)
file(GLOB sources_remove_SERVER_SHARE ../pitayaserver/framework/Lua*)

file(GLOB sources *.cpp *.h)

set(gameserver_STAT_SRCS
  ${sources}
  ${sources_SERVER_SHARE}
  ${sources_SHARE}
)

list(REMOVE_ITEM gameserver_STAT_SRCS ${sources_remove})
list(REMOVE_ITEM gameserver_STAT_SRCS ${sources_remove_SERVER_SHARE})

message(status " This is the server_share srcs:" ${sources_SERVER_SHARE})
message(status " This is the share srcs:" ${sources_SHARE})
message(status " This is the all srcs:" ${gameserver_STAT_SRCS})

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/../pitayaserver/framework
  ${CMAKE_CURRENT_SOURCE_DIR}/../../share
  ${CMAKE_CURRENT_SOURCE_DIR}/../dep/include
  ${CMAKE_CURRENT_SOURCE_DIR}
  #  /usr/include/lua5.1
)
LINK_DIRECTORIES(
  /usr/local/lib
)
add_executable(gameserver ${gameserver_STAT_SRCS})
target_link_libraries(gameserver event)
target_link_libraries(gameserver zthread)
target_link_libraries(gameserver mysqlclient)
target_link_libraries(gameserver miniini)
#target_link_libraries(gameserver tolua++5.1)
#target_link_libraries(gameserver lua5.1)
target_link_libraries(gameserver event_pthreads)
target_link_libraries(gameserver tcmalloc) 
