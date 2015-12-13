cmake_minimum_required(VERSION 2.6)

set(NAME snake)

project(${NAME} CXX)

set(INCL_DIRS)
set(LINK_LIBS)

add_definitions(-std=c++11 -Wall -g3)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

find_package(SFML 2 REQUIRED system window graphics network audio)
if(SFML_FOUND)
  set(INCL_DIRS ${INCL_DIRS} ${SFML_INCLUDE_DIR})
  set(LINK_LIBS ${LINK_LIBS} ${SFML_LIBRARIES})
endif()

set(SOURCES
"src/main.cpp"
)

set(${INCL_DIRS}
"${CMAKE_SOURCE_DIR}/src"
)

include_directories(${INCL_DIRS})

add_executable(${NAME} ${SOURCES})

target_link_libraries(${NAME} ${LINK_LIBS})
