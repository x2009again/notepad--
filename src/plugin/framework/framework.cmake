

# add_framework_plugin [<dir>...] [<file>...]
# 构建一个基于 framework 的插件
    # 基于提供的包含源代码文件的的路径，或指定的要构建的源代码
macro(add_framework_plugin _target)

    option(${_target}_ENABLE "是否确认构建 ${_target} 插件" OFF)

if(${${_target}_ENABLE})

    set(${_target}_ARGN ${ARGN})
    # set(${_target}_DIR_OR_SOURCES)
    foreach(arg IN LISTS ${_target}_ARGN)
        list(APPEND ${_target}_DIR_OR_SOURCES ${arg})
    endforeach(arg IN LISTS ${_target}_ARGN)

    # 相对于使用宏 add_framework_plugin 的位置约定
    set(FRAMEWORK_DIR ${CMAKE_CURRENT_LIST_DIR}/framework)
    if(NOT EXISTS FRAMEWORK_DIR)
        # 相对于真实 CMakeLists.txt 节点的位置
        set(FRAMEWORK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/framework)
    endif(NOT EXISTS FRAMEWORK_DIR)

    spark_add_library_path(${_target} SHARED
        ${FRAMEWORK_DIR}
        ${${_target}_DIR_OR_SOURCES}
    )
    target_include_directories(${_target} PRIVATE
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/src/cceditor

        ${PROJECT_SOURCE_DIR}/src/qscint/src
        ${PROJECT_SOURCE_DIR}/src/qscint/src/Qsci
        ${PROJECT_SOURCE_DIR}/src/qscint/scintilla/src
        ${PROJECT_SOURCE_DIR}/src/qscint/scintilla/include
        ${PROJECT_SOURCE_DIR}/src/qscint/scintilla/lexlib
        ${PROJECT_SOURCE_DIR}/src/qscint/scintilla/boostregex
    )
    # target_link_libraries(${_target} QSci)
    target_link_QSci(${_target})
    if(USE_QT6)
        # target_link_qt6_Core5Compat(${_target})   # 兼容性: Qt6 可使用 Core5Compat 少量更改 Qt5 部分
        # target_link_qt6_PrintSupport(${_target})
        # target_link_qt6_XmlPatterns(${_target}) # Bug 初期配置时无此依赖要求
    else()
        # target_link_qt5_PrintSupport(${_target})
        # target_link_qt5_XmlPatterns(${_target})
    endif(USE_QT6)

    set_target_properties(${_target}
        PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY $<TARGET_FILE_DIR:${PROJECT_NAME}>/plugin
            LIBRARY_OUTPUT_DIRECTORY $<TARGET_FILE_DIR:${PROJECT_NAME}>/plugin
            ARCHIVE_OUTPUT_DIRECTORY $<TARGET_FILE_DIR:${PROJECT_NAME}>/plugin)

        
    if(WIN32 AND NOTEPAD_BUILD_BY_SHARED)
        # 在 Windows 中构建时，需要关注此库的构建形式，QScintilla 应该以何种方式编译
        target_compile_definitions(${_target} 
            PRIVATE 
                NOTEPAD_PLUGIN_MANAGER
                QSCINTILLA_DLL  # 目前在 Windows 中使用 QSci 库时应该采用 Q_DECL_IMPORT
                                # 控制 QSCINTILLA_EXPORT 符号应为 Q_DECL_IMPORT
        )
    else()
        # 在 Windows 中构建时，需要关注此库的构建形式，QScintilla 应该以何种方式编译
        target_compile_definitions(${_target} 
        PRIVATE 
            NOTEPAD_PLUGIN_MANAGER
            # QSCINTILLA_DLL # 目前在 Windows 中使用 QSci 库时应该采用 Q_DECL_IMPORT
                            # 控制 QSCINTILLA_EXPORT 符号应为 Q_DECL_IMPORT
        )
    endif(WIN32 AND NOTEPAD_BUILD_BY_SHARED)


    spark_cmake_debug(
        ">>>>>>>>>>>>>>>>>>>>>>>>>>> ${_target} CMake Debug <<<<<<<<<<<<<<<<<<<<<<<<<<<"
        "${_target} LINK_LIBRARIES:        $<TARGET_PROPERTY:${_target},LINK_LIBRARIES>"
        "${_target} COMPILE_DEFINITIONS:   $<TARGET_PROPERTY:${_target},COMPILE_DEFINITIONS>"
        "${_target} INTERFACE:             $<TARGET_PROPERTY:${_target},INTERFACE>"
        "${_target} TARGET_FILE_BASE_NAME: $<TARGET_FILE_BASE_NAME:${_target}>"
        "${_target} TARGET_FILE_NAME:      $<TARGET_FILE_NAME:${_target}>"
        ">>>>>>>>>>>>>>>>>>>>>>>>>>> ${_target} CMake Debug <<<<<<<<<<<<<<<<<<<<<<<<<<<"
    )

endif(${${_target}_ENABLE})

endmacro(add_framework_plugin _target)
