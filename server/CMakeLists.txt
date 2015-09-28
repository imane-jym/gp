
project(card)

cmake_minimum_required(VERSION 2.6)

# add dependencies
add_subdirectory(dep)

# add gameserver sources
add_subdirectory(singledemo)

# add gameserver sources lua version
add_subdirectory(luademo)

# add test sources
add_subdirectory(test)
