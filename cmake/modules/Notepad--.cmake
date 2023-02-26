# Notepad--.cmake

# Notepad-- 核心构建
# 在模块化构建中，这个部分代表着构建 Notepad-- 
# 1. 默认构建时产出的目标为 Notepad--
# 2. 在此处可对 Notepad-- 目标进行详细的构建计划

if(TRUE)
    # 准备构建 Notepad-- 主程序
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
    if(USE_QT6)
        target_link_qt6_PrintSupport(${PROJECT_NAME})
        target_link_qt6_XmlPatterns(${PROJECT_NAME})
    else()
        target_link_qt5_PrintSupport(${PROJECT_NAME})
        target_link_qt5_XmlPatterns(${PROJECT_NAME})
    endif(USE_QT6)
endif(TRUE)

# ----------------- Notepad-- 插件支持相关  ----------------- #
if(TRUE)
    # 开启插件支持 - 此处废弃，并由构建宏支持部分处理
    # target_compile_definitions(${PROJECT_NAME} PUBLIC NO_PLUGIN)
    # 其它有关插件的部分处理...
endif(TRUE)


# Notepad-- 目标在构建时依赖了一些其它内容，像先前构建的 QSci 目标、Qt5::XmlPatterns 
# Notepad-- 程序构建...配置

# ----------------- Notepad-- 构建宏支持相关  ----------------- #

if(WIN32)
    # 在 Windows 中构建时，需要关注此库的构建形式，QScintilla 应该以何种方式编译
    target_compile_definitions(${PROJECT_NAME} 
        PRIVATE 
            NO_PLUGIN       # 开启插件支持
            QSCINTILLA_DLL  # 目前在 Windows 中使用 QSci 库时应该采用 Q_DECL_IMPORT
                            # 控制 QSCINTILLA_EXPORT 符号应为 Q_DECL_IMPORT
    )
endif(WIN32)

if(UNIX)
    # 在 Windows 中构建时，需要关注此库的构建形式，QScintilla 应该以何种方式编译
    target_compile_definitions(${PROJECT_NAME} 
        PRIVATE 
            NO_PLUGIN       # 开启插件支持
    )
endif(UNIX)
