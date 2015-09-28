#set(CMAKE_CXX_FLAGS "-fpermissive -p -O2")
#set(CMAKE_CXX_FLAGS "-fpermissive -p -g -pthread -DCC_TARGET_PLATFORM=linux")
set(CMAKE_CXX_FLAGS "-fpermissive -g -Wall -pthread -DCC_TARGET_PLATFORM=linux")
#SET(CMAKE_BUILD_TYPE Release)
SET(CMAKE_BUILD_TYPE Debug)
file(GLOB_RECURSE sources_SERVER_SHARE ../../pitayaserver/*.cpp ../../pitayaserver/*.h)
file(GLOB_RECURSE sources_remove_SERVER_SHARE ../../pitayaserver/Lua* ../../pitayaserver/ScriptGlue*)

file(GLOB sources *.cpp *.h)

set(gameserver_STAT_SRCS
  ${sources}
  ${sources_SERVER_SHARE}
)

list(REMOVE_ITEM gameserver_STAT_SRCS ${sources_remove_SERVER_SHARE})

message(status " This is the server_share srcs:" ${sources_SERVER_SHARE})
message(status " This is the all srcs:" ${gameserver_STAT_SRCS})

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/../../pitayaserver
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../../dep/include
)
LINK_DIRECTORIES(
  /usr/local/lib
)
add_executable(backserver ${gameserver_STAT_SRCS})
target_link_libraries(backserver event)
target_link_libraries(backserver zthread)
target_link_libraries(backserver mysqlclient)
target_link_libraries(backserver miniini)
#target_link_libraries(backserver tolua++5.1)
#target_link_libraries(backserver lua5.1)
target_link_libraries(backserver event_pthreads)
target_link_libraries(backserver tcmalloc) 
