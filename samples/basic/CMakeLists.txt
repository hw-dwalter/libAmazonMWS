include_directories(${CMAKE_SOURCE_DIR}/lib)

find_package(Boost REQUIRED COMPONENTS system date_time)
find_package(CURL)
find_package(OpenSSL)

set(basicSrcs main.cpp)
add_executable(basic ${basicSrcs})
 
target_link_libraries(basic amazonmws ${OpenSSL_LIBRARIES} ${CURL_LIBRARIES} ${Boost_LIBRARIES} )
