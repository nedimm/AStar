set(SOURCE_FILES 
	include/Application.hpp
	source/Application.cpp
	include/IParameterServer.hpp
	include/ParameterServer.hpp
	include/ParameterStorage.hpp
	source/ParameterServer.cpp
	include/TypedParameter.hpp
	include/BaseParameter.hpp
	include/CustomDataTypes.hpp
	source/Main.cpp
)

set(RESOURCE_FILES 
	parameters.csv
	images/testmap_883_556.png
	images/Parking_01.png
	images/Parking_02.png
)

add_executable(AStarWin ${SOURCE_FILES} ${RESOURCE_FILES})
set_property(TARGET AStarWin ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "$(ProjectDir)$(Configuration)")

target_include_directories(AStarWin
	PRIVATE
		source
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	)
	
target_link_libraries(AStarWin AStar)

foreach(item ${RESOURCE_FILES})
	add_custom_command(
		TARGET AStarWin
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${item} 
		$<TARGET_FILE_DIR:AStarWin>
	)
endforeach(item)

