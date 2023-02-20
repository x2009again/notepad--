# QScint.cmake 

# 在模块化构建中，这个部分代表着构建 Qscintilla
# 1. 静态化构建库时产出的目标为 libQSci.a
# 2. 在此处可对 QSci 目标进行详细的构建计划

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
    target_compile_definitions(QSci PRIVATE SCINTILLA_QT SCI_LEXER INCLUDE_DEPRECATED_FEATURES QSCINTILLA_MAKE_DLL)
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


# QSci 库构建时依赖了一些其它内容，像 PrintSupport，Concurrent 等
# QSci 库构建...
