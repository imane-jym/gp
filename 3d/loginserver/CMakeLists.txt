set(CMAKE_CXX_FLAGS "-fpermissive -p -g -O2 -Wall -pthread -DCC_TARGET_PLATFORM=linux -Wl,-rpath=../../libc/")
#set(CMAKE_CXX_FLAGS "-fpermissive -p -g -Wl,-rpath=../../libc/")
SET(CMAKE_BUILD_TYPE Release)
#SET(CMAKE_BUILD_TYPE Debug)
file(GLOB_RECURSE sources_SERVER_SHARE ../pitayaserver/framework/[^.]*.cpp ../pitayaserver/framework/[^.]*.h ../pitayaserver/util/[^.]*.cpp ../pitayaserver/util/[^.]*.h)
file(GLOB_RECURSE sources_SHARE ../share/*.cpp ../share/*.h)
file(GLOB sources_remove_SERVER_SHARE ../pitayaserver/framework/Lua* ../pitayaserver/framework/ScriptGlue*)

file(GLOB sources *.cpp *.h)

set(loginserver_STAT_SRCS
  ${sources}
  ${sources_SERVER_SHARE}
  ${sources_SHARE}
 # ${sources_SHARE_DATA}
)

list(REMOVE_ITEM loginserver_STAT_SRCS ${sources_remove_SERVER_SHARE})
#message(status " This is the server_share srcs:" ${sources_SERVER_SHARE})
#message(status " This is the share srcs:" ${sources_SHARE})
#message(status " This is the share data srcs:" ${sources_SHARE_DATA})

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/../pitayaserver/framework
  ${CMAKE_CURRENT_SOURCE_DIR}/../pitayaserver/util
  ${CMAKE_CURRENT_SOURCE_DIR}/../share
  ${CMAKE_CURRENT_SOURCE_DIR}/../dep/include
  ${CMAKE_CURRENT_SOURCE_DIR}
  #/usr/include/lua5.1
  /usr/local/include/libmongoc-1.0
  /usr/local/include/libbson-1.0
)
LINK_DIRECTORIES(
  /usr/local/lib
)

#SET(EXECUTABLE_OUTPUT_PATH ../../bin/loginserver)
add_executable(loginserver ${loginserver_STAT_SRCS})

#SET_TARGET_PROPERTIES(loginserver PROPERTIES CLEAN_DIRECT_OUTPUT 1)
#SET_TARGET_PROPERTIES(loginserver PROPERTIES OUTPUT_NAME loginserver)

target_link_libraries(loginserver event)
target_link_libraries(loginserver zthread)
target_link_libraries(loginserver mysqlclient)
target_link_libraries(loginserver miniini)
#target_link_libraries(Gameserver glog)
#target_link_libraries(loginserver tolua++5.1)
#target_link_libraries(loginserver lua5.1)
target_link_libraries(loginserver event_pthreads)
target_link_libraries(loginserver curl)
target_link_libraries(loginserver tcmalloc) 
TARGET_LINK_LIBRARIES(loginserver mongoc-1.0)
TARGET_LINK_LIBRARIES(loginserver bson-1.0)

target_link_libraries(loginserver proto)
