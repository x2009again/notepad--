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
    target_link_qt5_XmlPatterns(${PROJECT_NAME})

endif(TRUE)

# ----------------- Notepad-- 插件支持相关  ----------------- #
if(TRUE)
    # 开启插件支持
    target_compile_definitions(${PROJECT_NAME} PUBLIC NO_PLUGIN=0)
    # 其它有关插件的部分处理...
endif(TRUE)


# Notepad-- 目标在构建时依赖了一些其它内容，像先前构建的 QSci 目标、Qt5::XmlPatterns 