# Notepad--.cmake

# Notepad-- 核心构建
# 在模块化构建中，这个部分代表着构建 Notepad-- 
# 1. 默认构建时产出的目标为 Notepad--
# 2. 在此处可对 Notepad-- 目标进行详细的构建计划

# Notepad-- 版本配置
configure_file(cmake/modules/config.h.in
    ${CMAKE_BINARY_DIR}/config.h @ONLY)

if(TRUE)
    # 准备构建 Notepad-- 主程序
    set(QRC_SOURCES src/RealCompare.qrc)
    spark_add_executable_path(${PROJECT_NAME}
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/src/cceditor
        ${QRC_SOURCES})

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
        target_link_qt6_Core5Compat(${PROJECT_NAME})   # 兼容性: Qt6 可使用 Core5Compat 少量更改 Qt5 部分
        target_link_qt6_PrintSupport(${PROJECT_NAME})
        # target_link_qt6_XmlPatterns(${PROJECT_NAME}) # Bug 初期配置时无此依赖要求
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

# 在 Windows 中构建时，需要关注此库的构建形式，QScintilla 应该以何种方式编译
target_compile_definitions(${PROJECT_NAME} 
    PRIVATE 
        NO_PLUGIN       # 开启插件支持
)

# 添加 Notepad-- 目标属性与生成器调试信息
spark_cmake_debug(
    ">>>>>>>>>>>>>>>>>>>>>>>>>>> Notepad-- CMake Debug <<<<<<<<<<<<<<<<<<<<<<<<<<<"
    "Notepad-- LINK_LIBRARIES:        $<TARGET_PROPERTY:Notepad--,LINK_LIBRARIES>"
    "Notepad-- COMPILE_DEFINITIONS:   $<TARGET_PROPERTY:Notepad--,COMPILE_DEFINITIONS>"
    "Notepad-- INTERFACE:             $<TARGET_PROPERTY:Notepad--,INTERFACE>"
    "Notepad-- TARGET_FILE_BASE_NAME: $<TARGET_FILE_BASE_NAME:Notepad-->"
    "Notepad-- TARGET_FILE_NAME:      $<TARGET_FILE_NAME:Notepad-->"
    ">>>>>>>>>>>>>>>>>>>>>>>>>>> Notepad-- CMake Debug <<<<<<<<<<<<<<<<<<<<<<<<<<<"
)