#set(CMAKE_CXX_FLAGS "-fpermissive -p -O2")
#set(CMAKE_CXX_FLAGS "-fpermissive -p -g -pthread -DCC_TARGET_PLATFORM=linux")
set(CMAKE_CXX_FLAGS "-fpermissive -g -Wall -pthread -DCC_TARGET_PLATFORM=linux")
#SET(CMAKE_BUILD_TYPE Release)
SET(CMAKE_BUILD_TYPE Debug)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/../pitayaserver/framework
)

add_executable(client client.cpp)
add_executable(logtest logtest.cpp ../pitayaserver/framework/Log.cpp)

add_subdirectory(backserver)
add_subdirectory(frontserver)
