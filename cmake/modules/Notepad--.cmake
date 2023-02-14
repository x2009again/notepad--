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


