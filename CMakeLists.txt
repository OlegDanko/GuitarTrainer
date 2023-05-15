cmake_minimum_required(VERSION 3.20)

project(trainer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Boost COMPONENTS atomic)
find_library(PORTAUDIO_LIB NAMES portaudio)
find_library(QWT_LIB NAMES qwt)
find_library(FFTW_LIB NAMES fftw3)

set(LIBS
    Qt${QT_VERSION_MAJOR}::Widgets
    ${Boost_LIBRARIES}
    ${PORTAUDIO_LIB}
    ${QWT_LIB}
    ${FFTW_LIB}
)

set(PROJECT_SOURCES
        main.cpp
        Note.cpp
        FretWidget.cpp
        DrawableWidgetBase.cpp
        FretBoardWidget.cpp
        ToneMapper.cpp
        AudioSynthesizer.cpp
        DelFn.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(trainer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        res/resources.qrc
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET trainer APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(trainer SHARED
            ${PROJECT_SOURCES}
            res/resources.qrc
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(trainer
            ${PROJECT_SOURCES}
            res/resources.qrc
        )
    endif()
endif()

target_link_libraries(trainer ${LIBS})

set_target_properties(trainer PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS trainer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(trainer)
endif()
