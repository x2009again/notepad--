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
    add_compile_definitions(_UNICODE= UNICODE=)
endif()

# ----------------- 构建 QSci 静态库 ----------------- #
if(TRUE)
    include(cmake/modules/QScint.cmake)
endif(TRUE)


# ----------------- 构建 Notepad-- 程序 ----------------- #
if(TRUE)
    include(cmake/modules/Notepad--.cmake)
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
