find_package(Boost REQUIRED)
IF (Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
    ADD_DEFINITIONS( "-DHAS_BOOST" )
ENDIF()

find_package(Glog)

add_library(async_server async_http_server.cpp)

target_include_directories(async_server PUBLIC include)

target_link_libraries(async_server PUBLIC ${Boost_LIBRARIES} glog::glog)