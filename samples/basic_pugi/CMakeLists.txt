include_directories(${CMAKE_SOURCE_DIR}/lib)

find_package(Boost REQUIRED COMPONENTS system date_time)
find_package(CURL)
find_package(OpenSSL)

set(srcs main.cpp ../pugixml.cpp)
add_executable(basic_pugi ${srcs})
 
target_link_libraries(basic_pugi amazonmws ${OpenSSL_LIBRARIES} ${CURL_LIBRARIES} ${Boost_LIBRARIES} )
