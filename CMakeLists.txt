cmake_minimum_required(VERSION 3.24)
project(notepad--)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Concurrent Network PrintSupport XmlPatterns)

# TODO: use system provided libraries to build 
# current status: some header can not be found,for example: Scintilla.h
find_library(QSCINTILLA_LIB_PATH qscintilla2_qt5)
find_path(QSCINTILLA_INC_PATH qsciscintilla.h PATHS /usr/include/)
find_path(SCINTILLA_INC_PATH Scintilla.h PATHS /usr/include/)
if((${QSCINTILLA_LIB_PATH}  EQUAL "QSCINTILLA_LIB_PATH-NOTFOUND") OR 
   (${QSCINTILLA_INC_PATH}  EQUAL "QSCINTILLA_INC_PATH-NOTFOUND") OR 
   (${SCINTILLA_INC_PATH}  STREQUAL "SCINTILLA_INC_PATH-NOTFOUND") )
set(NOTEPAD_USE_SYS_LIB OFF)
message("system libraries or header not found,build from local")
add_subdirectory(${PROJECT_SOURCE_DIR}/src/qscint)
else()
set(NOTEPAD_USE_SYS_LIB ON)
message("use system libraries")
message("QSCINTILLA_LIB_PATH:" ${QSCINTILLA_LIB_PATH})
message("QSCINTILLA_INC_PATH:" ${QSCINTILLA_INC_PATH})
message("SCINTILLA_INC_PATH:" ${SCINTILLA_INC_PATH})
endif()

file(GLOB UI_SRC ${PROJECT_SOURCE_DIR}/src/*.ui)
set(UI_SRC ${UI_SRC} ${PROJECT_SOURCE_DIR}/src/cceditor/ccnotepad.ui)
aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/cceditor SRC)

add_executable(${PROJECT_NAME} ${SRC} ${UI_SRC} ${PROJECT_SOURCE_DIR}/src/RealCompare.qrc)
target_include_directories(${PROJECT_NAME} PRIVATE 
${PROJECT_SOURCE_DIR}/src
${PROJECT_SOURCE_DIR}/src/cceditor
)

if(NOTEPAD_USE_SYS_LIB)
target_include_directories(${PROJECT_NAME} PRIVATE ${QSCINTILLA_INC_PATH} ${SCINTILLA_INC_PATH})
endif()

target_link_libraries(${PROJECT_NAME} qscintilla2_qt5 Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Concurrent Qt5::Network  Qt5::PrintSupport Qt5::XmlPatterns)

install(
    TARGETS ${PROJECT_NAME}
    DESTINATION "bin"
)

install(DIRECTORY  ${PROJECT_SOURCE_DIR}/src/linux/usr
        DESTINATION "/")

include(${PROJECT_SOURCE_DIR}/cmake/package_config.cmake) 
include(CPack)
