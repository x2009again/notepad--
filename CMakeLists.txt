cmake_minimum_required(VERSION 3.24)
project(notepad--)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Concurrent Network PrintSupport XmlPatterns)

add_subdirectory(${PROJECT_SOURCE_DIR}/src/qscint)


file(GLOB UI_SRC ${PROJECT_SOURCE_DIR}/src/*.ui)
set(UI_SRC ${UI_SRC} ${PROJECT_SOURCE_DIR}/src/cceditor/ccnotepad.ui)
aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/cceditor SRC)

add_executable(${PROJECT_NAME} ${SRC} ${UI_SRC} ${PROJECT_SOURCE_DIR}/src/RealCompare.qrc)
target_include_directories(${PROJECT_NAME} PRIVATE 
${PROJECT_SOURCE_DIR}/src
${PROJECT_SOURCE_DIR}/src/cceditor

${PROJECT_SOURCE_DIR}/src/qscint/src
${PROJECT_SOURCE_DIR}/src/qscint/src/Qsci 
${PROJECT_SOURCE_DIR}/src/qscint/scintilla/src
${PROJECT_SOURCE_DIR}/src/qscint/scintilla/include
${PROJECT_SOURCE_DIR}/src/qscint/scintilla/lexlib
${PROJECT_SOURCE_DIR}/src/qscint/scintilla/boostregex
)

target_link_libraries(${PROJECT_NAME} qscintilla2_qt5 Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Concurrent Qt5::Network  Qt5::PrintSupport Qt5::XmlPatterns)

install(
    TARGETS ${PROJECT_NAME}
    DESTINATION "bin"
)

install(DIRECTORY  ${PROJECT_SOURCE_DIR}/src/linux/usr
        DESTINATION "/")

include(${PROJECT_SOURCE_DIR}/cmake/package_config.cmake) 
include(CPack)
