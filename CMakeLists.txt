cmake_minimum_required(VERSION 2.8)

set (PROJECT_NAME "EventLoop")

# set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -W -Map=output2.map")

project(${PROJECT_NAME})

#set folder path variable
set(WORKER_DIR "${CMAKE_CURRENT_LIST_DIR}/worker")
set(TIMER_DIR "${CMAKE_CURRENT_LIST_DIR}/timer")
set(UTIL_DIR "${CMAKE_CURRENT_LIST_DIR}/util")
set(JSON_DIR "${CMAKE_CURRENT_LIST_DIR}/json")
set(LUA_DIR "${CMAKE_CURRENT_LIST_DIR}/lua")
set(LUA_WRAPPER_DIR "${CMAKE_CURRENT_LIST_DIR}/luawrapper")
set(TEST_DIR "${CMAKE_CURRENT_LIST_DIR}/test")

#set include dir
list(APPEND INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}")
list(APPEND INCLUDE_DIRS ${WORKER_DIR})
list(APPEND INCLUDE_DIRS ${TIMER_DIR})
list(APPEND INCLUDE_DIRS ${UTIL_DIR})
list(APPEND INCLUDE_DIRS ${JSON_DIR})
list(APPEND INCLUDE_DIRS ${LUA_DIR})
list(APPEND INCLUDE_DIRS ${LUA_WRAPPER_DIR})
list(APPEND INCLUDE_DIRS ${TEST_DIR})

include_directories(${INCLUDE_DIRS})

#set src dir
aux_source_directory(${CMAKE_CURRENT_LIST_DIR} SRC_ROOT)
aux_source_directory(${WORKER_DIR} SRC_WORKER)
aux_source_directory(${TIMER_DIR} SRC_TIMER)
aux_source_directory(${UTIL_DIR} SRC_UTIL)
list(APPEND SRC_JSON "${JSON_DIR}/cJSON.c")
list(APPEND SRC_JSON "${JSON_DIR}/CJsonObject.cpp")
aux_source_directory(${LUA_DIR} SRC_LUA)
list(FILTER SRC_LUA EXCLUDE REGEX "lua.c")
aux_source_directory(${LUA_WRAPPER_DIR} SRC_LUA_WRAPPER)
aux_source_directory(${TEST_DIR} SRC_TEST)

#collect all src files
list(APPEND SRC_FILES ${SRC_ROOT})
list(APPEND SRC_FILES ${SRC_WORKER})
list(APPEND SRC_FILES ${SRC_TIMER})
list(APPEND SRC_FILES ${SRC_UTIL})
list(APPEND SRC_FILES ${SRC_JSON})
list(APPEND SRC_FILES ${SRC_LUA})
list(APPEND SRC_FILES ${SRC_LUA_WRAPPER})
list(APPEND SRC_FILES ${SRC_TEST})

message("SRC_FILES:")
foreach(src_file ${SRC_FILES})
    message(${src_file})
endforeach()

add_executable(${PROJECT_NAME} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} pthread rt)