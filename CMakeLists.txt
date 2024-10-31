cmake_minimum_required(VERSION 3.16)

project(VacationApp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools Sql)
find_package(PostgreSQL REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core5Compat)


set(TS_FILES VacationApp_ru_RU.ts
    VacationApp_en_US.ts)

set(PROJECT_SOURCES
        main.cpp
        vacationtable.cpp
        vacationtable.h
        vacationtable.ui
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(VacationApp
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        vacationrectitem.h vacationrectitem.cpp
        vacationscene.h vacationscene.cpp
        vacationeditdialog.h vacationeditdialog.cpp vacationeditdialog.ui
        horizontalheaderitem.h horizontalheaderitem.cpp
        employeeadddialog.h employeeadddialog.cpp employeeadddialog.ui
        verticalheaderitem.h verticalheaderitem.cpp
        vacationadddialog.h vacationadddialog.cpp vacationadddialog.ui
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET VacationApp APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(VacationApp SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(VacationApp
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(VacationApp PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(VacationApp PRIVATE Qt${QT_VERSION_MAJOR}::Sql)
# This line adds both include paths and libraries for Postgres
target_link_libraries(VacationApp PRIVATE PostgreSQL::PostgreSQL)
target_link_libraries(VacationApp PRIVATE Qt6::Core5Compat)
# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.VacationApp)
endif()
set_target_properties(VacationApp PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS VacationApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(VacationApp)
endif()
