cmake_minimum_required(VERSION 3.24)
project(notepad-- VERSION 1.22.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Concurrent Network PrintSupport XmlPatterns)

# TODO: use system provided libraries to build 
# current status: some header can not be found,for example: Scintilla.h
find_library(QSCINTILLA_LIB_PATH qscintilla2_qt5)
find_path(QSCINTILLA_INC_PATH qsciscintilla.h PATHS /usr/include/)
find_path(SCINTILLA_INC_PATH Scintilla.h PATHS /usr/include/)
if((${QSCINTILLA_LIB_PATH} STREQUAL "QSCINTILLA_LIB_PATH-NOTFOUND") OR 
   (${QSCINTILLA_INC_PATH} STREQUAL "QSCINTILLA_INC_PATH-NOTFOUND") OR 
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

target_link_libraries(${PROJECT_NAME} qscint Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Concurrent Qt5::Network  Qt5::PrintSupport Qt5::XmlPatterns)

if(CMAKE_HOST_UNIX)
    include(cmake/SparkInstallMacrosConfig.cmake)
    include(cmake/SparkDesktopMacros.cmake)
    # 内容默认应用名称: Name= 应与项目名称相同
    spark_desktop_macros(
        # 应用名称: Name=
        ${PROJECT_NAME}
        # 应用名称: Name[zh_CN]=
        "Notepad--"
        # 应用说明: Comment=
        "Notepad-- 是一个国产跨平台、简单的文本编辑器。"
        # 应用类型: Type=
        "Application"
        # 执行程序: Exec=
        "notepad--"
        # 图标路径: Icon=
        "/usr/share/notepad--/icons/spark.png"
        # 应用分类: Category=
        "Development"
    )
    spark_install_file(/usr/share/applications/    ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.desktop)
    spark_install_file(/usr/share/notepad--/icons/ assets/spark.png)
    spark_install_target(/usr/bin/ ${PROJECT_NAME})


    # 1. 在顶层构建中导入 Appimage 的构建
    include(cmake/SparkAppimageConfig.cmake)  # 导入来自 Spark 构建的 Appimage 构建
    add_appimage_icon(assets/spark.png)       # 添加到 Appimage 中的默认的图标
    add_appimage_desktop()                    # 添加到 Appimage 中的默认desktop(使用来自 Spark 构建的 Desktop 构建中配置的信息(必须要求 spark-desktop))
    add_appimage()                            # 应用对 Appimage 的构建


    # 注释行(使用方式)
    find_package(DebPackage PATHS ${CMAKE_SOURCE_DIR})
    add_package_descript(cmake/package-deb.descript)

endif(CMAKE_HOST_UNIX)
