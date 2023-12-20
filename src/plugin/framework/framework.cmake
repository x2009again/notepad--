

# add_framework_plugin [<dir>...] [<file>...]
# 构建一个基于 framework 的插件
    # 基于提供的包含源代码文件的的路径，或指定的要构建的源代码
macro(add_framework_plugin _target)
    
    set(${_target}_ARGN ${ARGN})
    # set(${_target}_DIR_OR_SOURCES)
    foreach(arg IN LISTS ${_target}_ARGN)
        list(APPEND ${_target}_DIR_OR_SOURCES ${arg})
    endforeach(arg IN LISTS ${_target}_ARGN)
    

    spark_add_library_path(${_target} SHARED
        ${CMAKE_CURRENT_LIST_DIR}/framework
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
                QSCINTILLA_DLL  # 目前在 Windows 中使用 QSci 库时应该采用 Q_DECL_IMPORT
                                # 控制 QSCINTILLA_EXPORT 符号应为 Q_DECL_IMPORT
        )
    else()
        # 在 Windows 中构建时，需要关注此库的构建形式，QScintilla 应该以何种方式编译
        target_compile_definitions(${_target} 
        PRIVATE 
            # QSCINTILLA_DLL # 目前在 Windows 中使用 QSci 库时应该采用 Q_DECL_IMPORT
                            # 控制 QSCINTILLA_EXPORT 符号应为 Q_DECL_IMPORT
        )
    endif(WIN32 AND NOTEPAD_BUILD_BY_SHARED)
endmacro(add_framework_plugin _target)
