# opencc-demo-plugin/CMakeLists.txt
    # 用于在源代码中构建一个插件的模板
    
# 如果不是在源代码中构建插件，请不要使用这个部分
    # 如果想在源代码中添加更多内置模板
    # 请替换 opencc-demo-plugin 为你的插件名称
    # opencc-demo-plugin -> your plugin name

set(LOCAL_PLUGIN_NAME "opencc-demo-plugin")

# opencc-demo-plugin 核心构建
# 在模块化构建中，这个部分代表着构建 opencc-demo-plugin 插件
# 1. 默认构建时产出的目标为 opencc-demo-plugin
# 2. 在此处可对 opencc-demo-plugin 目标进行详细的构建计划

if(TRUE)
    # 准备构建 opencc-demo-plugin 主程序扩展
    spark_file_glob(LocalSources
        ./*.h ./*.cpp ./*.ui
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


# ----------------- opencc-demo-plugin 构建宏支持相关  ----------------- #

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


# ----------------- opencc-demo 构建的外部项目依赖 ----------------- #

set(libopencc           "opencc_git")
set(libopencc_name      "OpenCC")
set(libopencc_version   "master")
set(libopencc_url       "https://gitee.com/mirrors/opencc")
set(libopencc_prefix    ${CMAKE_CURRENT_BINARY_DIR}/${libopencc_name}-${libopencc_version})
set(libopencc_src       ${libopencc_prefix}/src/${libopencc})
set(libopencc_build     ${libopencc_prefix}/src/${libopencc}-build)
set(libopencc_install   ${libopencc_prefix}/src/${libopencc}-install)
set(libopencc_flags     "-DBUILD_SHARED_LIBS=OFF;-DCMAKE_INSTALL_PREFIX=../${libopencc}-install")

include(${CMAKE_SOURCE_DIR}/cmake/platforms/utils.cmake)
get_current_platform_lib_name(libopencc_lib FALSE opencc)
get_current_platform_lib_name(libmarisa_lib FALSE marisa)

include(ExternalProject)
ExternalProject_Add(${libopencc}
    PREFIX              ${libopencc_prefix}
    GIT_REPOSITORY      ${libopencc_url}
    GIT_TAG             ${libopencc_version}
    # GIT_SHALLOW         TRUE
    CONFIGURE_COMMAND   ${CMAKE_COMMAND} -S ${libopencc_src} -B ${libopencc_build} ${libopencc_flags}
    # CMAKE_GENERATOR
    BUILD_COMMAND       ${CMAKE_COMMAND} --build ${libopencc_build} --config $<CONFIG>
    # INSTALL_COMMAND     ${CMAKE_COMMAND} --install ${libopencc_build} --prefix ${libopencc_install}
    INSTALL_COMMAND     ${CMAKE_COMMAND} --install ${libopencc_build} --config $<CONFIG>
    # TEST_COMMAND
    GIT_REMOTE_UPDATE_STRATEGY CHECKOUT
    EXCLUDE_FROM_ALL
)

add_library(opencc STATIC IMPORTED)
set_target_properties(opencc
    PROPERTIES
        IMPORTED_LOCATION        ${libopencc_install}/lib/${libopencc_lib}
        INTERFACE_LINK_LIBRARIES ${libopencc_install}/lib/${libmarisa_lib}
        INTERFACE_INCLUDE_DIRECTORIES ${libopencc_install}/include)

# ----------------------------------------------------------------

# 确保 opencc_git 在此插件之前构建完成
add_dependencies(${LOCAL_PLUGIN_NAME}      opencc_git)
target_link_libraries(${LOCAL_PLUGIN_NAME} opencc)

if(WIN32)
    target_include_directories(${LOCAL_PLUGIN_NAME} PUBLIC ${libopencc_install}/include)
    target_compile_definitions(${LOCAL_PLUGIN_NAME} 
        PRIVATE 
            OPENCC_EXPORT=)
endif(WIN32)