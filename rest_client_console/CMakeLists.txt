set(SOURCE_FILES 
	source/Main.cpp
)

add_executable(RestClientConsole ${SOURCE_FILES})
set_property(TARGET RestClientConsole ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "$(ProjectDir)$(Configuration)")

target_include_directories(RestClientConsole
	PRIVATE
		source
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	)
	
target_link_libraries(RestClientConsole RestClient)

foreach(item ${RESOURCE_FILES})
	add_custom_command(
		TARGET RestClientConsole
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${item} 
		$<TARGET_FILE_DIR:RestClientConsole>
	)
endforeach(item)

