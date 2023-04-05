# QScint.cmake 

# 在模块化构建中，这个部分代表着构建 Qscintilla
# 1. 静态化构建库时产出的目标为 libQSci.a
# 2. 在此处可对 QSci 目标进行详细的构建计划

option(NOTEPAD_BUILD_BY_SHARED "指定 Notepad 将构建为动态库" OFF)

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
    if(NOTEPAD_BUILD_BY_SHARED)
        spark_add_library(QSci SHARED ${QSciSources} ${MOC_HEADER})
    else()
        spark_add_library(QSci STATIC ${QSciSources} ${MOC_HEADER})
    endif(NOTEPAD_BUILD_BY_SHARED)
    target_include_directories(QSci PRIVATE
        src/qscint/scintilla/boostregex
        src/qscint/scintilla/lexlib)
    target_include_directories(QSci PUBLIC
        src/qscint/src
        src/qscint/src/Qsci
        src/qscint/scintilla/src
        src/qscint/scintilla/include)
    if(USE_QT6)
        target_link_qt6(QSci)
        target_link_qt6_Core5Compat(QSci)
        target_link_qt6_PrintSupport(QSci)
        target_link_qt6_Concurrent(QSci)
    else()
        target_link_qt5(QSci)
        target_link_qt5_PrintSupport(QSci)
        target_link_qt5_Concurrent(QSci)
    endif(USE_QT6)
endif(TRUE)


# QSci 库构建时依赖了一些其它内容，像 PrintSupport，Concurrent 等
# QSci 库构建...配置

if(WIN32)
    # 在 Windows 中构建时，需要关注此库的构建形式，QScintilla 应该以何种方式编译
    target_compile_definitions(QSci 
        PRIVATE 
            SCINTILLA_QT                # 
            SCI_LEXER                   # 
            INCLUDE_DEPRECATED_FEATURES # 

        # 控制 QSCINTILLA_EXPORT 符号应为：
                                        # 构建时(导出)，由外部使用时(导入)
            QSCINTILLA_MAKE_DLL         # 在 Windows 中构建此库时应该采用 Q_DECL_EXPORT
        INTERFACE
            QSCINTILLA_DLL              # 在 Windows 中使用此库时应该采用 Q_DECL_IMPORT
    )
endif(WIN32)

if(UNIX)
    # 在 Linux 中构建时，需要关注此库的构建形式，QScintilla 应该以何种方式编译
    target_compile_definitions(QSci 
        PRIVATE 
            SCINTILLA_QT                # 
            SCI_LEXER                   # 
            INCLUDE_DEPRECATED_FEATURES # 
            # QSCINTILLA_MAKE_DLL       # 在 Linux 未使用 Q_DECL_EXPORT 、 Q_DECL_IMPORT
                                        # 控制 QSCINTILLA_EXPORT 符号应为留空
    )
endif(UNIX)
