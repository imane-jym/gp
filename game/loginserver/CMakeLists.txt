set(CMAKE_CXX_FLAGS "-fpermissive -p -g -Wall -pthread -DCC_TARGET_PLATFORM=linux")
SET(CMAKE_BUILD_TYPE Debug)
file(GLOB_RECURSE sources_SERVER_SHARE ../pitayaserver/framework/*.cpp ../pitayaserver/framework/*.h)
file(GLOB_RECURSE sources_SHARE ../../share/*.cpp ../../share/*.h)
file(GLOB sources_remove_SERVER_SHARE ../pitayaserver/framework/Lua*)

file(GLOB sources *.cpp *.h)

set(loginserver_STAT_SRCS
  ${sources}
  ${sources_SERVER_SHARE}
  ${sources_SHARE}
)

list(REMOVE_ITEM loginserver_STAT_SRCS ${sources_remove_SERVER_SHARE})
message(status " This is the server_share srcs:" ${sources_SERVER_SHARE})
message(status " This is the share srcs:" ${sources_SHARE})

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/../pitayaserver/framework
  ${CMAKE_CURRENT_SOURCE_DIR}/../../share
  ${CMAKE_CURRENT_SOURCE_DIR}/../dep/include
  ${CMAKE_CURRENT_SOURCE_DIR}
  #/usr/include/lua5.1
)
LINK_DIRECTORIES(
  /usr/local/lib
)
add_executable(loginserver ${loginserver_STAT_SRCS})
target_link_libraries(loginserver event)
target_link_libraries(loginserver zthread)
target_link_libraries(loginserver mysqlclient)
target_link_libraries(loginserver miniini)
#target_link_libraries(loginserver tolua++5.1)
#target_link_libraries(loginserver lua5.1)
target_link_libraries(loginserver event_pthreads)
target_link_libraries(loginserver tcmalloc) 
