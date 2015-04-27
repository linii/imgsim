cmake_minimum_required(VERSION 2.8)
project( histogram )

find_package( OpenCV )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( histogram histogram.cpp )
target_link_libraries( histogram ${OpenCV_LIBS} )


set(Boost_USE_STATIC_LIBS ON) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME ON) 
set(BOOST_ROOT )
set(BOOST_LIBRARYDIR ${BOOST_ROOT}/include/boost)

find_package(Boost REQUIRED COMPONENTS filesystem foreach) 

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR}) 
    add_executable(histogram histogram.cpp) 
    target_link_libraries(histogram ${Boost_LIBRARIES})
endif()