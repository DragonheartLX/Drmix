cmake_minimum_required (VERSION 3.19)

project(Drmix VERSION 1.0.0 LANGUAGES CXX)

find_package(Qt6 6.7 COMPONENTS Core Widgets Quick Gui LinguistTools REQUIRED)

qt_standard_project_setup(
    I18N_SOURCE_LANGUAGE en_US
    I18N_TRANSLATED_LANGUAGES zh_CN en_US
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}>)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}>)
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install/${CMAKE_BUILD_TYPE}")


file(GLOB_RECURSE PROJ_Src LIST_DIRECTORIES false 
    src/*.cpp
    src/**/*.cpp
)

file(GLOB_RECURSE QML_Src
    Assets/QML/*.qml
    Assets/QML/**/*.qml
)

foreach(fullpath ${QML_Src})
    set(filepath "")
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" filepath ${fullpath})
    string(REPLACE "Assets/QML/" "" filename ${filepath})
    list(APPEND qml_files ${filepath})
    set_source_files_properties(${filepath} PROPERTIES QT_RESOURCE_ALIAS ${filename})
endforeach()

qt_add_resources(PROJ_Src
    Assets/resource.qrc
)

qt_add_executable(Drmix
    MANUAL_FINALIZATION
    ${PROJ_Src}
)

qt_add_qml_module(Drmix
    URI "Drmix"
    RESOURCE_PREFIX "/qt/qml"
    VERSION 1.0
    QML_FILES ${qml_files}
)

qt_add_translations(Drmix
    TS_FILE_DIR "Assets/ts"
    TS_FILE_BASE Drmix
)

target_include_directories(Drmix PRIVATE Drmix)

target_link_libraries(Drmix PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
    Qt6::Quick
    Qt6::Qml
)

qt_import_qml_plugins(Drmix)
qt_finalize_executable(Drmix)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	find_program(TOOL_WINDEPLOYQT NAMES windeployqt.debug.bat)
else()
	find_program(TOOL_WINDEPLOYQT NAMES windeployqt6)
endif()

add_custom_command(TARGET Drmix POST_BUILD
    COMMAND ${TOOL_WINDEPLOYQT}
            --qmldir "${CMAKE_CURRENT_SOURCE_DIR}/Assets" $<TARGET_FILE:Drmix>
    COMMENT "Running windeployqt..."
)