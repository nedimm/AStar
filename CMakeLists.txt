cmake_minimum_required(VERSION 2.8)
project(AStar)

set("OpenCV_DIR" "C:/opencv/build")
find_package(OpenCV REQUIRED)

set(SOURCE_FILES 
	include/Application.hpp
	source/Application.cpp
	include/Map.hpp
	source/Map.cpp
	include/Grid.hpp
	source/Grid.cpp
	include/Graph.hpp
	source/Graph.cpp
	include/IParameterServer.hpp
	include/ParameterServer.hpp
	include/ParameterStorage.hpp
	source/ParameterServer.cpp
	include/TypedParameter.hpp
	include/BaseParameter.hpp
	include/CustomDataTypes.hpp
	include/AStar.hpp
	source/AStar.cpp
	source/implementation.cpp
	source/Main.cpp
)

set(RESOURCE_FILES 
	parameters.csv
	images/testmap_883_556.png
)

add_executable( AStar ${SOURCE_FILES} ${RESOURCE_FILES})
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT AStar)

target_link_libraries( AStar ${OpenCV_LIBS} )

foreach(item ${RESOURCE_FILES})
	add_custom_command(
		TARGET ${PROJECT_NAME}
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${item} 
		$<TARGET_FILE_DIR:${PROJECT_NAME}>
	)
endforeach(item)

