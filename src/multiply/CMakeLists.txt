find_package(Glog REQUIRED)

add_library(multiply multiply.cpp)

target_include_directories(multiply PUBLIC include)
target_link_libraries(multiply PUBLIC glog::glog )
