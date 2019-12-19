set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set("Qt5Core_DIR" "C:/Qt/Qt5.13.1/5.13.1/msvc2017_64/lib/cmake/Qt5Core")
set("Qt5Gui_DIR" "C:/Qt/Qt5.13.1/5.13.1/msvc2017_64/lib/cmake/Qt5Gui")
set("Qt5Widgets_DIR" "C:/Qt/Qt5.13.1/5.13.1/msvc2017_64/lib/cmake/Qt5Widgets")
set("Qt5_DIR" "C:/Qt/Qt5.13.1/5.13.1/msvc2017_64/lib/cmake/Qt5")
set("qt_install_prefix" "C:/Qt/Qt5.13.1/5.13.1/msvc2017_64")

find_package(Qt5 COMPONENTS Widgets REQUIRED)

if(Qt5_FOUND AND WIN32 AND TARGET Qt5::qmake AND NOT TARGET Qt5::windeployqt)
    get_target_property(_qt5_qmake_location Qt5::qmake IMPORTED_LOCATION)

    execute_process(
        COMMAND "${_qt5_qmake_location}" -query QT_INSTALL_PREFIX
        RESULT_VARIABLE return_code
        OUTPUT_VARIABLE qt5_install_prefix
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    set(imported_location "${qt5_install_prefix}/bin/windeployqt.exe")

    if(EXISTS ${imported_location})
        add_executable(Qt5::windeployqt IMPORTED)

        set_target_properties(Qt5::windeployqt PROPERTIES
            IMPORTED_LOCATION ${imported_location}
        )
    endif()
endif()

add_executable(ParoValMVP WIN32
    ParoValMVP.ui
    ParoValMVP.cpp
    main.cpp
    ParoValMVP.qrc
)

set_property(TARGET ParoValMVP ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "$(ProjectDir)$(Configuration)")

target_link_libraries(ParoValMVP PRIVATE Qt5::Widgets)
add_custom_command(TARGET ParoValMVP
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_CURRENT_BINARY_DIR}/windeployqt"
        COMMAND set PATH=%PATH%$<SEMICOLON>${qt5_install_prefix}/bin
        COMMAND Qt5::windeployqt --dir "$(ProjectDir)$(Configuration)" "$<TARGET_FILE_DIR:ParoValMVP>/$<TARGET_FILE_NAME:ParoValMVP>"
)