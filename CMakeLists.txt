cmake_minimum_required(VERSION 3.22)

project(notepad-- VERSION 1.22.0)

include(cmake/SparkEnvConfig.cmake)
include(cmake/SparkMacrosConfig.cmake)
include(cmake/SparkFindQt5Config.cmake)
# include(cmake/SparkFindQt6Config.cmake)
include(cmake/SparkMacrosExtendConfig.cmake)

# ----------------- Build QScint ----------------- #
if(TRUE)
    # add_subdirectory(${PROJECT_SOURCE_DIR}/src/qscint)
    # file(GLOB MOC_HEADER src/qscint/src/Qsci/*.h)
    spark_file_glob(MOC_HEADER "src/qscint/src/Qsci/*.h")

    spark_add_source_paths(QSciSources
        src/qscint/src
        src/qscint/scintilla/lexers
        src/qscint/scintilla/lexlib
        src/qscint/scintilla/src
        src/qscint/scintilla/boostregex

        # src/qscint/src/Qsci
        # FAIL: only *.ui will spark_file_glob(MOC_HEADER ...)
    )
    spark_add_library(QSci STATIC ${QSciSources} ${MOC_HEADER})
    target_compile_definitions(QSci PRIVATE SCINTILLA_QT SCI_LEXER INCLUDE_DEPRECATED_FEATURES)
    target_include_directories(QSci PRIVATE
        src/qscint/scintilla/boostregex
        src/qscint/scintilla/lexlib)
    target_include_directories(QSci PUBLIC
        src/qscint/src
        src/qscint/src/Qsci
        src/qscint/scintilla/src
        src/qscint/scintilla/include)
    target_link_qt5(QSci)
    target_link_qt5_PrintSupport(QSci)
    target_link_qt5_Concurrent(QSci)
endif(TRUE)


# ----------------- Build CCEditor ----------------- #
if(TRUE)

    # 准备构建 CCEditor
    set(QRC_SOURCES src/RealCompare.qrc)
    spark_aux_source_paths(CCEditorSources
        src
        src/cceditor
    )
    spark_add_executable(${PROJECT_NAME} ${CCEditorSources} ${QRC_SOURCES})
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
    # target_link_libraries(${PROJECT_NAME} QSci)
    target_link_QSci(${PROJECT_NAME})
    target_link_qt5_XmlPatterns(${PROJECT_NAME})

endif(TRUE)

# ----------------- CCEditor Plugin Support or Other  ----------------- #
if(TRUE)
    
    # 开启插件支持
    target_compile_definitions(${PROJECT_NAME} PUBLIC NO_PLUGIN=0)

    # 其它有关插件的部分处理...

endif(TRUE)

# 可能需要变更为 CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux" ??
# 并使用 Linux.cmake 维护?
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
        # 有关此 %F 参数可查看: https://gitee.com/zinface/z-tools/blob/desktop-dev/src/DesktopGenerater/desktopexecparamdialog.cpp
        "notepad-- %F"
        # 图标路径: Icon=
        "/usr/share/notepad--/icons/spark.png"
        # 应用分类: Category=
        "Development"
    )
    # spark_install_file(/usr/share/applications/    ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.desktop)
    # spark_install_file(/usr/share/notepad--/icons/ assets/spark.png)
    # spark_install_target(/usr/bin/ ${PROJECT_NAME})

    # 完成：如果主线已经将 linux/destkop 的PR合入，即可移除此部分以上部分，而使用以下部分
        # 但 spark_desktop_macros 不能移除，因为它被 Appimage 构建部分依赖
        # Appimage 构建部分，要求(使用来自 Spark 构建的 Desktop 构建中配置的信息(必须要求 spark-desktop))
    spark_install_directory(/usr   src/linux/usr/*)    # 完整的 Linux 资源文件

    # ------------------ INSTALL PLUGIN CONFIG ------------------ #
    # ------------------ INSTALL PLUGIN CONFIG ------------------ #
    # ------------------ INSTALL PLUGIN CONFIG ------------------ #
    include(CMakePackageConfigHelpers)
    include(GNUInstallDirs)
    
    # 定义插件配置安装位置
    set(CMAKE_INSTALL_PREFIX "/usr")
    set(NOTEPAD_PLUGIN NotepadPlugin)
    
    # 定义开发插件时的配置目录与 CMake 模块文件名称
    set(NOTEPAD_PLUGIN_CONFIG       ${NOTEPAD_PLUGIN}Config.cmake)
    set(NOTEPAD_PLUGIN_INCLUDEDIR   ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}/${NOTEPAD_PLUGIN})
    set(NOTEPAD_PLUGIN_LIBDIR       ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/${NOTEPAD_PLUGIN})
    
    # 定义一些扩展内容，主要是提供给 CMake 模块文件填充
    set(NOTEPAD_PLUGIN_CORELIB QSci)
    set(NOTEPAD_PLUGIN_EXTERNAL_INCLUDES 
        ${NOTEPAD_PLUGIN_INCLUDEDIR}/Qsci)

    # 定义在插件开发的 CMake 模块中，Notepad-- 是否是基于 QT5 实现
        # 并自动为插件开发层自动开启相关 Qt 依赖组件
        # 此部分逻辑将自动提供给 add_notepad_plugin 自行处理
    set(NOTEPAD_BUILD_BY_QT5 TRUE)
    set(NOTEPAD_BUILD_BY_QT6 FALSE)

    # 定义在插件开发的 CMake 模块中，Notepad-- 是否将提供 "插件安装目录(位置)"
    set(NOTEPAD_PLUGIN_EXTERNAL_PLUGIN_INSTALL_DIRECTORY "")

    # 将 NOTEPAD_PLUGIN_INCLUDEDIR NOTEPAD_PLUGIN_LIBDIR 填充到 cmake 文件
    configure_package_config_file(cmake/NotepadPluginConfig.cmake.in
        ${CMAKE_BINARY_DIR}/${NOTEPAD_PLUGIN_CONFIG}
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${NOTEPAD_PLUGIN})
    # 安装 cmake 文件
    spark_install_file(${CMAKE_INSTALL_LIBDIR}/cmake/${NOTEPAD_PLUGIN} 
        ${CMAKE_BINARY_DIR}/${NOTEPAD_PLUGIN_CONFIG})
    # 安装头插件接口文件
    spark_install_file(${NOTEPAD_PLUGIN_INCLUDEDIR} 
        src/include/pluginGl.h)
    # 安装插件所需要的Qsci文件
    spark_install_directory(${NOTEPAD_PLUGIN_INCLUDEDIR} 
        src/qscint/src/Qsci)
    
    # 导出 QSci 的头文件(从插件实现层面来看，目前是使用*.h 与 pluginGl.h
        # 但从 Qsci 层面来看，*.h 使用的是 #include <Qsci/*.h> ，所以此部分不被使用)
    # spark_file_glob(QSci_HEADERS src/qscint/src/Qsci/*.h src/include/pluginGl.h)
    # set_target_properties(QSci PROPERTIES PUBLIC_HEADER "${QSci_HEADERS}")

    # 导出 Notepad-- 的接口文件(从逻辑层面来看，目前已经在前面安装了接口文件，所以此部分不被使用)
    # spark_file_glob(Notepad_HEADERS src/include/pluginGl.h)
    # set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${QSci_HEADERS}")

    # 安装项目文件与 QSci 文件
    install(TARGETS ${PROJECT_NAME} QSci
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION ${NOTEPAD_PLUGIN_LIBDIR}
        ARCHIVE DESTINATION ${NOTEPAD_PLUGIN_LIBDIR}
        PUBLIC_HEADER DESTINATION ${NOTEPAD_PLUGIN_INCLUDEDIR}
    )
    # ------------------ INSTALL PLUGIN CONFIG ------------------ #
    # ------------------ INSTALL PLUGIN CONFIG ------------------ #
    # ------------------ INSTALL PLUGIN CONFIG ------------------ #


    # 1. 在顶层构建中导入 Appimage 的构建
    include(cmake/SparkAppimageConfig.cmake)  # 导入来自 Spark 构建的 Appimage 构建
    add_appimage_icon(assets/spark.png)       # 添加到 Appimage 中的默认的图标
    add_appimage_desktop()                    # 添加到 Appimage 中的默认desktop(使用来自 Spark 构建的 Desktop 构建中配置的信息(必须要求 spark-desktop))
    add_appimage()                            # 应用对 Appimage 的构建


    # 注释行(使用方式)
    find_package(DebPackage PATHS ${CMAKE_SOURCE_DIR})
    add_package_descript(cmake/package-deb.descript)

endif(CMAKE_HOST_UNIX)
