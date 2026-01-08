cmake_minimum_required(VERSION 3.16)

project(MultiScope VERSION 0.1 LANGUAGES CXX)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets SerialPort PrintSupport OpenGL)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets SerialPort PrintSupport OpenGL)

add_definitions(-DQCUSTOMPLOT_USE_OPENGL)

set(PROJECT_SOURCES
        main.cpp
        Widget.cpp
        Widget.h
        Widget.ui
        qcustomplot.cpp  # 加入这里
        qcustomplot.h    # 加入这里
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(MultiScope
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET MultiScope APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(MultiScope SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(MultiScope
            ${PROJECT_SOURCES}
        )
    endif()
endif()


target_link_libraries(MultiScope PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::SerialPort
    Qt${QT_VERSION_MAJOR}::PrintSupport
    Qt${QT_VERSION_MAJOR}::OpenGL
    opengl32  # <--- 必须手动添加这一行，解决 __imp_glClear 等引用丢失问题
)

# 针对 MinGW 的专项强力配置
if(MINGW)
    # 强制开启大目标文件支持
    add_compile_options(-Wa,-mbig-obj)

    # 彻底静默 Qt 6 的过时警告，让编译器专心处理代码
    add_definitions(-DQT_NO_DEPRECATED_WARNINGS)

    # 对 qcustomplot.cpp 单独关照：开启 O2 优化（减小中间体积）并再次确认 mbig-obj
    set_source_files_properties(qcustomplot.cpp PROPERTIES COMPILE_FLAGS "-O2 -Wa,-mbig-obj")
endif()


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.MultiScope)
endif()
set_target_properties(MultiScope PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS MultiScope
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(MultiScope)
endif()
