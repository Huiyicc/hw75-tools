cmake_minimum_required(VERSION 3.5)

project(hw_tools VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

include_directories( . )
include_directories( ./lib )
file(GLOB CORE_SOURCES core/*.cpp)
file(GLOB USB_SOURCES lib/HID/*.cpp)
file(GLOB HTTPSERVER_SOURCES lib/HID/*.cpp)
file(GLOB UTILS_SOURCES lib/utils/*.cpp)
file(GLOB IMAGE_SOURCES lib/image/*.cpp)
file(GLOB PROTOBUF_SOURCES lib/protobuf/*.cpp)
file(GLOB PROTOBUF_SOURCES lib/protobuf/*.cc)
file(GLOB QT_SOURCES ui/*.cpp)


set(PROJECT_SOURCES
        ${CORE_SOURCES}
        ${HTTPSERVER_SOURCES}
        ${USB_SOURCES}
        ${UTILS_SOURCES}
        ${IMAGE_SOURCES}
        ${PROTOBUF_SOURCES}
        ${QT_SOURCES}
        main.cpp
        )

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(hw_tools
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
            )
else()
    add_executable(hw_tools
            ${PROJECT_SOURCES}
            )
endif()

# target_link_libraries(hw_tools PRIVATE Qt${QT_VERSION_MAJOR}::Widgets -lhidapi -lprotobuf -lpthread)

target_link_libraries(hw_tools PUBLIC Qt${QT_VERSION_MAJOR}::Widgets -lhidapi -lprotobuf -lpthread)


if(CMAKE_HOST_UNIX)
    # linux独有库
elseif(CMAKE_HOST_WIN32)
    # windows独有库
    target_link_libraries(hw_tools PRIVATE -lws2_32)
endif()


set_target_properties(hw_tools PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
        )

install(TARGETS hw_tools
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        )

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(hw_tools)
endif()



add_executable(usb_comm lib/protobuf/usb_comm.proto)

