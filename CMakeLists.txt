cmake_minimum_required(VERSION 3.22)

project(notepad-- VERSION 1.22.0)

include(cmake/SparkEnvConfig.cmake)
include(cmake/SparkMacrosConfig.cmake)
include(cmake/SparkFindQt5Config.cmake)
# include(cmake/SparkFindQt6Config.cmake)
include(cmake/SparkMacrosExtendConfig.cmake)


# ----------------- 构建之前 全局定义 ----------------- #
# 在 Windows 中开启 UNICODE 以支持 TCHAR 功能
if(WIN32)
    add_compile_definitions(-D_UNICODE -DUNICODE)
endif()

# ----------------- 构建 QSci 静态库 ----------------- #
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


# ----------------- 构建 Notepad-- 程序 ----------------- #
if(TRUE)

    # 准备构建 CCEditor
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

# ----------------- Notepad-- 其它主线构建相关  ----------------- #

# 使用 macos.cmake / linux.cmake / windows.cmake 维护构建.
if(APPLE)
    include(cmake/platforms/macos.cmake)
elseif(UNIX AND NOT APPLE AND NOT WIN32 AND NOT ANDROID)
    include(cmake/platforms/linux.cmake)
elseif(WIN32)
    include(cmake/platforms/windows.cmake)
endif()
