cmake_minimum_required(VERSION 2.8)
project( hist )

set(Boost_USE_STATIC_LIBS ON) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME ON) 

set(BOOST_ROOT /usr/local/Cellar/boost/1.57.0)
set(BOOST_LIBRARYDIR ${BOOST_ROOT}/lib)
set(Boost_LIBRARIES ${BOOST_LIBRARYDIR})
set(Boost_INCLUDE_DIR /usr/local/Cellar/boost/1.57.0/include)
set(CMAKE_EXE_LINKER_FLAGS "-lboost_system")

message ( status "BOOST_LIBRARYDIR: " ${Boost_INCLUDE_DIR})
find_package( OpenCV )
find_package(Boost REQUIRED COMPONENTS filesystem) 

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR}) 
    include_directories( ${OpenCV_INCLUDE_DIRS} )
    add_executable(hist histogram.cpp) 
    target_link_libraries(hist ${Boost_LIBRARIES})
    target_link_libraries( hist ${OpenCV_LIBS} )
endif()