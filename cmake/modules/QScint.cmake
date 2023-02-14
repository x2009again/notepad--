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
