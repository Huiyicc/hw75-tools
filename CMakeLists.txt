cmake_minimum_required(VERSION 3.5)

project(hw_tools VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#if ((CMAKE_BUILD_TYPE MATCHES "Debug"))
#    add_definitions(-D_DEBUG)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -O0 -g -Wall")
#else ()
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -O3 -g -Wall")
#endif ()

add_definitions(-DCPPHTTPLIB_OPENSSL_SUPPORT )

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
endif()

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)
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
file(GLOB QWT_SOURCES lib/qwt/*.cpp)
include_directories(lib/fmt/inc)
file(GLOB FMT_SOURCES lib/fmt/src/*.cc)
#file(GLOB QT_SOURCES ui/*.cpp)

set(PROJECT_SOURCES_LIB
        ${CORE_SOURCES}
        ${CORE_SOURCES_WIDGET}
        ${HTTPSERVER_SOURCES}
        ${USB_SOURCES}
        ${UTILS_SOURCES}
        ${IMAGE_SOURCES}
        ${Plugin_SOURCES}
        ${PROTOBUF_SOURCES}
        ${PROTOBUFCC_SOURCES}
        ${FMT_SOURCES}
        ${QT_SOURCES}
        ${TS_FILES}
)

set(PROJECT_SOURCES_APP
        ${PROJECT_SOURCES_LIB}
        res.qrc
        ui/widget/KnobDashboard.cpp
        ui/widget/KnobDashboard.hpp
        ui/knobaligndialog.h
        ui/knobaligndialog.cpp
        ui/knobaligndialog.ui
        main.cpp
)

set(PROJECT_SOURCES_IMAGE_CONVERT
        ${PROJECT_SOURCES_LIB}
        "./tools/ImageConvert/convert.cpp"
)

# 设置编译时间戳
string(TIMESTAMP COMPILE_TIME %Y%m%d)
set(build_time    ${COMPILE_TIME})
message(STATUS "build time: ${build_time}")
find_package(Git REQUIRED)
# 设置GIT版本信息
execute_process(
        COMMAND ${GIT_EXECUTABLE} describe --tags --abbrev=0
        OUTPUT_VARIABLE GIT_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
)
# 设置GIT分支信息
execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
        OUTPUT_VARIABLE GIT_BRANCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
)
# 设置GIT HASH
execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
        OUTPUT_VARIABLE GIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
)
configure_file("${CMAKE_SOURCE_DIR}/build_info.h.in" "${CMAKE_SOURCE_DIR}/build_info.h")

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    if ((CMAKE_HOST_SYSTEM_NAME MATCHES "Darwin"))
        add_definitions(-D_D__APPLE__)
        set(OS_IS_APPLE TRUE)
        qt_add_executable(hw_tools
                MANUAL_FINALIZATION
                MACOSX_BUNDLE
                ${PROJECT_SOURCES_APP}
        )
        qt_add_executable(ImageConvert
                MANUAL_FINALIZATION
                MACOSX_BUNDLE
                ${PROJECT_SOURCES_IMAGE_CONVERT}
        )
    else ()
        qt_add_executable(hw_tools
                MANUAL_FINALIZATION
                ${PROJECT_SOURCES_APP}
        )
        qt_add_executable(ImageConvert
                MANUAL_FINALIZATION
                ${PROJECT_SOURCES_IMAGE_CONVERT}
        )
    endif ()
    # Define target properties for Android with Qt 6 as:
    #    set_property(TARGET hw_tools APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
    #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
    # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
#        add_library(hw_tools SHARED
#                ${PROJECT_SOURCES}
#        )
        # Define properties for Android with Qt 5 after find_package() calls as:
        #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(hw_tools
                ${PROJECT_SOURCES_APP}
        )
        add_executable(ImageConvert
                ${PROJECT_SOURCES_IMAGE_CONVERT}
        )
    endif()
endif()
find_package(Protobuf CONFIG REQUIRED)

target_link_libraries(hw_tools PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        -lhidapi
        protobuf::libprotobuf
        -lpthread
)
target_link_libraries(ImageConvert PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        -lhidapi
        protobuf::libprotobuf
        -lpthread
)
# target_link_libraries(uf2conv PRIVATE )

if(CMAKE_HOST_UNIX  AND NOT DEFINED OS_IS_APPLE)
    # linux独有库
    add_definitions(-D_D__linux__)
elseif(CMAKE_HOST_WIN32)
    # windows独有库
    add_definitions(-D_D__WIN32__)
    target_link_libraries(hw_tools PRIVATE
            -lQwt
            -lws2_32
            -lssl
            -lcrypto
            -lCRYPT32
    )
    target_link_libraries(ImageConvert PRIVATE
            -lws2_32
            -lssl
            -lcrypto
            -lCRYPT32
    )
    set_target_properties(hw_tools PROPERTIES WIN32_EXECUTABLE TRUE)
    set_target_properties(ImageConvert PROPERTIES WIN32_EXECUTABLE TRUE)
elseif(OS_IS_APPLE)
    # mac库
    find_package(OpenSSL REQUIRED)
    include_directories(${OPENSSL_INCLUDE_DIR})
    find_library( QWT NAMES qwt qwt6 PATHS /usr/local/Cellar/qwt/6.2.0_1/lib REQUIRED )

    target_link_libraries(hw_tools PRIVATE
            -lssl
            -lcrypto
            ${OPENSSL_LIBRARIES}
            ${QWT}
    )
    target_link_libraries(ImageConvert PRIVATE
            -lssl
            -lcrypto
            ${OPENSSL_LIBRARIES}
    )
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

if(CMAKE_HOST_UNIX  AND NOT DEFINED OS_IS_APPLE)
    # linux

elseif(CMAKE_HOST_WIN32)
    # windows
elseif(OS_IS_APPLE)

endif()
