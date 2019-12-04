set("OpenCV_DIR" "C:/opencv/build")
find_package(OpenCV REQUIRED)

set(SOURCE_FILES 
	include/Map.hpp
	source/Map.cpp
	include/Grid.hpp
	source/Grid.cpp
	include/Graph.hpp
	source/Graph.cpp
	include/AStar.hpp
	source/AStar.cpp
	include/Path.hpp
	source/Path.cpp
)

add_library(AStar
	STATIC 
	${SOURCE_FILES}
)

target_include_directories(AStar
	PRIVATE
		source
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	)
	
target_link_libraries( AStar ${OpenCV_LIBS} )


