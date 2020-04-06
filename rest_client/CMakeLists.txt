set(SOURCE_FILES 
	include/GdClient.hpp
	source/GdClient.cpp
)

find_package(cpprestsdk REQUIRED)

add_library(RestClient
	STATIC 
	${SOURCE_FILES}
)

target_include_directories(RestClient
	PRIVATE
		source
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
)

target_link_libraries(RestClient cpprestsdk::cpprest)