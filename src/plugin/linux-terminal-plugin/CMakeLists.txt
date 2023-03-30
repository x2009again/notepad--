# linux-terminal-plugin/CMakeLists.txt
    # 用于在源代码中构建一个插件的模板
    
# 如果不是在源代码中构建插件，请不要使用这个部分
    # 如果想在源代码中添加更多内置模板
    # 请替换 linux-terminal-plugin 为你的插件名称
    # linux-terminal-plugin -> your plugin name

set(LOCAL_PLUGIN_NAME "linux-terminal-plugin")

# linux-terminal-plugin 核心构建
# 在模块化构建中，这个部分代表着构建 linux-terminal-plugin 插件
# 1. 默认构建时产出的目标为 linux-terminal-plugin
# 2. 在此处可对 linux-terminal-plugin 目标进行详细的构建计划

if(TRUE)
    # 准备构建 linux-terminal-plugin 主程序扩展
    spark_file_glob(LocalSources
        ./*.h ./*.cpp # ./*.ui
    )
    spark_add_library(${LOCAL_PLUGIN_NAME} SHARED ${LocalSources})
    target_include_directories(${LOCAL_PLUGIN_NAME} PRIVATE
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/src/cceditor

        ${PROJECT_SOURCE_DIR}/src/qscint/src
        ${PROJECT_SOURCE_DIR}/src/qscint/src/Qsci
        ${PROJECT_SOURCE_DIR}/src/qscint/scintilla/src
        ${PROJECT_SOURCE_DIR}/src/qscint/scintilla/include
        ${PROJECT_SOURCE_DIR}/src/qscint/scintilla/lexlib
        ${PROJECT_SOURCE_DIR}/src/qscint/scintilla/boostregex
    )
    # target_link_libraries(${LOCAL_PLUGIN_NAME} QSci)
    target_link_QSci(${LOCAL_PLUGIN_NAME})
    if(USE_QT6)
        # target_link_qt6_Core5Compat(${LOCAL_PLUGIN_NAME})   # 兼容性: Qt6 可使用 Core5Compat 少量更改 Qt5 部分
        # target_link_qt6_PrintSupport(${LOCAL_PLUGIN_NAME})
        # target_link_qt6_XmlPatterns(${LOCAL_PLUGIN_NAME}) # Bug 初期配置时无此依赖要求
    else()
        # target_link_qt5_PrintSupport(${LOCAL_PLUGIN_NAME})
        # target_link_qt5_XmlPatterns(${LOCAL_PLUGIN_NAME})
    endif(USE_QT6)

    # 确保生成到 Notepad-- 的相对 plugin 目录下
    set_target_properties(${LOCAL_PLUGIN_NAME}
        PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY $<TARGET_FILE_DIR:${PROJECT_NAME}>/plugin
            LIBRARY_OUTPUT_DIRECTORY $<TARGET_FILE_DIR:${PROJECT_NAME}>/plugin
            ARCHIVE_OUTPUT_DIRECTORY $<TARGET_FILE_DIR:${PROJECT_NAME}>/plugin)

    # 控制是否安装到 bin/plugin 而各种配方之下，每个位置或许都不一样(特别是 Linux)
    # install(TARGETS ${LOCAL_PLUGIN_NAME} DESTINATION bin/plugin)
endif(TRUE)


# ----------------- linux-terminal-plugin 构建宏支持相关  ----------------- #

if(WIN32 AND NOTEPAD_BUILD_BY_SHARED)
    # 在 Windows 中构建时，需要关注此库的构建形式，QScintilla 应该以何种方式编译
    target_compile_definitions(${LOCAL_PLUGIN_NAME} 
        PRIVATE 
            NOTEPAD_PLUGIN_MANAGER
            QSCINTILLA_DLL  # 目前在 Windows 中使用 QSci 库时应该采用 Q_DECL_IMPORT
                            # 控制 QSCINTILLA_EXPORT 符号应为 Q_DECL_IMPORT
    )
else()
    # 在 Windows 中构建时，需要关注此库的构建形式，QScintilla 应该以何种方式编译
    target_compile_definitions(${LOCAL_PLUGIN_NAME} 
    PRIVATE 
        NOTEPAD_PLUGIN_MANAGER
        # QSCINTILLA_DLL # 目前在 Windows 中使用 QSci 库时应该采用 Q_DECL_IMPORT
                         # 控制 QSCINTILLA_EXPORT 符号应为 Q_DECL_IMPORT
    )
endif(WIN32 AND NOTEPAD_BUILD_BY_SHARED)

if(UNIX)
    # 默认在 Unix/Linux 中仅需要定义一个内部插件宏管理器
    target_compile_definitions(${LOCAL_PLUGIN_NAME} 
        PRIVATE 
            NOTEPAD_PLUGIN_MANAGER
    )
endif(UNIX)

# ----------------- linux-terminal-plugin 构建的外部项目依赖 ----------------- #
include(${CMAKE_SOURCE_DIR}/cmake/SparkExternalProject.cmake)

# 1. 提供 lxqt 系列项目使用的配置依赖
spark_add_external_git_cmake_project(lxqt-build-tool
    https://www.gitlink.org.cn/zinface/lxqt-build-tools.git
        ""
        ""
        ""
        GIT_SHALLOW TRUE)

# 2. QTerminal 项目，依赖于 lxqt-build-tool 配置来构建
spark_add_external_git_cmake_project(qtermwidget_git
    https://www.gitlink.org.cn/zinface/qtermwidget.git
        # 配置编译时使用 lxqt-build-tool 提供的内容，安装时路径为"该项目内的安装目录"
        "-DCMAKE_PREFIX_PATH=${lxqt-build-tool-install};-DCMAKE_INSTALL_PREFIX=\${prj_install}"
        "--;-j12"
        ""
        GIT_SHALLOW TRUE)

# -------------------------------------------------------------------------------
# 3. 构建完成时，引入平台函数
    # 一个静态的 qtermwidget5 库应该为
        # libqtermwidget5.a 或 qtermwidget5.lib
include(${CMAKE_SOURCE_DIR}/cmake/platforms/utils.cmake)
get_current_platform_lib_name(libqtermwidget5_lib FALSE qtermwidget5)

add_library(qtermwidget SHARED IMPORTED)
set_target_properties(qtermwidget
    PROPERTIES
        IMPORTED_LOCATION ${qtermwidget_git-install}/lib/${libqtermwidget5_lib}
        INTERFACE_INCLUDE_DIRECTORIES ${qtermwidget_git-install}/include
        INTERFACE_LINK_DIRECTORIES ${qtermwidget_git-install}/lib)

# ----------------- linux-terminal-plugin 构建的外部项目依赖 ----------------- #
# 1.明确声明 qtermwidget_git 依赖于 lxqt-build-tool 
add_dependencies(qtermwidget_git lxqt-build-tool)

# 2.明确声明 qtermwidget 依赖于 qtermwidget_git
add_dependencies(qtermwidget qtermwidget_git)

# 3.明确声明 qtermwidget 依赖于 qtermwidget_git
add_dependencies(${LOCAL_PLUGIN_NAME} qtermwidget qtermwidget_git lxqt-build-tool)

# 4. 链接 qtermwidget 目标
target_link_libraries(${LOCAL_PLUGIN_NAME} qtermwidget)