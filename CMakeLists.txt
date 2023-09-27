cmake_minimum_required(VERSION 3.5)

project(hw_tools VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(TS_FILES hw_tools_zh_CN.ts)

include_directories( . )
include_directories( ./lib )
file(GLOB CORE_SOURCES ui/*.cpp)
file(GLOB USB_SOURCES lib/HID/*.cpp)
file(GLOB HTTPSERVER_SOURCES lib/HID/*.cpp)
file(GLOB UTILS_SOURCES lib/utils/*.cpp)
file(GLOB IMAGE_SOURCES lib/image/*.cpp)
file(GLOB PROTOBUF_SOURCES lib/protobuf/*.cpp)
file(GLOB PROTOBUF_SOURCES lib/protobuf/*.cc)
#file(GLOB QT_SOURCES ui/*.cpp)


set(PROJECT_SOURCES
        ${CORE_SOURCES}
        ${HTTPSERVER_SOURCES}
        ${USB_SOURCES}
        ${UTILS_SOURCES}
        ${IMAGE_SOURCES}
        ${PROTOBUF_SOURCES}
        ${QT_SOURCES}
        main.cpp
        ${TS_FILES}
        res.qrc
)

#set(PROJECT_SOURCES
#        main.cpp
#        mainwindow.cpp
#        mainwindow.h
#        mainwindow.ui
#        ${TS_FILES}
#)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(hw_tools
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        res.qrc
        ui/switchdialog.h ui/switchdialog.cpp ui/switchdialog.ui
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET hw_tools APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(hw_tools SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(hw_tools
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

find_package(Protobuf CONFIG REQUIRED)

target_link_libraries(hw_tools PRIVATE Qt${QT_VERSION_MAJOR}::Widgets -lhidapi protobuf::libprotobuf -lpthread)

if(CMAKE_HOST_UNIX)
    # linux独有库
elseif(CMAKE_HOST_WIN32)
    # windows独有库
    target_link_libraries(hw_tools PRIVATE -lws2_32)
    set_target_properties(hw_tools PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.hw_tools)
endif()
set_target_properties(hw_tools PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS hw_tools
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(hw_tools)
endif()
