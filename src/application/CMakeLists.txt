find_package(Glog)

add_executable(application application.cpp)
target_include_directories(application PUBLIC ../boost_http_async/include)
target_link_libraries(application PUBLIC async_server glog::glog)
