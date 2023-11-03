cmake_minimum_required(VERSION 3.5)

project(hw_tools VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if ((CMAKE_BUILD_TYPE MATCHES "Debug"))
    add_definitions(-D_DEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -O0 -g -Wall")
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -O3 -g -Wall")
endif ()



find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(TS_FILES hw_tools_zh_CN.ts)

include_directories(.)
include_directories(lib)
file(GLOB CORE_SOURCES ui/*.cpp)
file(GLOB CORE_SOURCES_WIDGET ui/widget/*.cpp)
file(GLOB USB_SOURCES lib/HID/*.cpp)
file(GLOB HTTPSERVER_SOURCES lib/http/*.cpp)
file(GLOB UTILS_SOURCES lib/utils/*.cpp)
file(GLOB IMAGE_SOURCES lib/image/*.cpp)
file(GLOB PROTOBUF_SOURCES lib/protobuf/*.cpp)
file(GLOB PROTOBUFCC_SOURCES lib/protobuf/*.cc)
file(GLOB Plugin_SOURCES lib/plugin/*.cpp)
#file(GLOB QT_SOURCES ui/*.cpp)


set(PROJECT_SOURCES
        ${CORE_SOURCES}
        ${CORE_SOURCES_WIDGET}
        ${HTTPSERVER_SOURCES}
        ${USB_SOURCES}
        ${UTILS_SOURCES}
        ${IMAGE_SOURCES}
        ${PROTOBUF_SOURCES}
        ${PROTOBUFCC_SOURCES}
        ${Plugin_SOURCES}
        ${QT_SOURCES}
        main.cpp
        ${TS_FILES}
        res.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    if ((CMAKE_HOST_SYSTEM_NAME MATCHES "Darwin"))
        qt_add_executable(hw_tools
                MANUAL_FINALIZATION
                MACOSX_BUNDLE
                ${PROJECT_SOURCES}
        )
    else ()
        qt_add_executable(hw_tools
                MANUAL_FINALIZATION
                ${PROJECT_SOURCES}
        )
    endif ()
    # Define target properties for Android with Qt 6 as:
    #    set_property(TARGET hw_tools APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
    #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
    # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
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
