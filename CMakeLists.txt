cmake_minimum_required(VERSION 3.22)

project(Notepad-- VERSION 1.22.0)

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
option(USE_MACOS_UNIVERSAL   "通用 MacOS 平台构建"            OFF)
#
option(USE_LINUX_UNIVERSAL   "通用 Linux 平台构建"            OFF)
option(USE_LINUX_DEBIAN      "通用 Debian Linux 平台构建"     OFF)
option(USE_LINUX_APPIMGE     "通用 Appimage Linux 平台构建"   OFF)
#
option(USE_LINUX_UOS         "独立 Linux 平台的 Uos 构建"      OFF)
#
option(USE_WINDOWS_UNIVERSAL "通用 Windows 平台构建"          OFF)
option(USE_WINDOWS_MINGW     "通用 Windows 平台 MinGW 构建"   OFF)
option(USE_WINDOWS_MSVC      "通用 Windows 平台 MSVC 构建"    OFF)
#
option(WINDOWS_DEPLOY_QT5    "通用 Windows Qt5 构建 windeployqt" OFF)
option(WINDOWS_DEPLOY_QT6    "通用 Windows Qt6 构建 windeployqt" OFF)


# 使用 macos.cmake / linux.cmake / windows.cmake 维护构建.
if(APPLE)
    # 通用 MacOS 平台的构建(一个无具体实现的空构建)
    # include(cmake/platforms/macos.cmake)

    # 在 Linux 中此处将由通用构建、Debian 打包构建、Appimage 打包构建、Uos 打包构建组成
elseif(UNIX AND NOT APPLE AND NOT WIN32 AND NOT ANDROID)

    # 通用 Linux 平台构建，一切的起始
    if(USE_LINUX_UNIVERSAL)
        # include(cmake/platforms/linux-universal.cmake)

        # 通用平台的 Debian deb 构建
        if(USE_LINUX_DEBIAN)    
            # include(cmake/platforms/linux-debian.cmake)
        endif()

        # 通用平台的  Appimage  构建
        if(USE_LINUX_APPIMGE)  
            # include(cmake/platforms/linux-appimage.cmake)
        endif()

        # 独立 Linux 平台的 Uos 构建
    elseif(USE_LINUX_UOS)
        # include(cmake/platforms/linux-uos.cmake)

        # 其它 Linux 平台的构建
    else()
        # include(cmake/platforms/linux.cmake)
    endif(USE_LINUX_UNIVERSAL)
    
    # 在 Windows 中此处将由通用构建、MinGW 构建、MSVC 构建组成
elseif(WIN32)
    
    # 通用 Windows 平台构建，一切的起始
    if(USE_WINDOWS_UNIVERSAL)
        # include(cmake/platforms/windows.cmake)
    
        # 通用平台的  Windows MinGW 构建
        if(USE_WINDOWS_MINGW)
            # include(cmake/platforms/windows-mingw.cmake)
        endif()
        
        # 通用平台的  Windows MSVC 构建
        if(USE_WINDOWS_MSVC)
            # include(cmake/platforms/windows-msvc.cmake)
        endif()

        # 其它 Windows 平台的构建
    else()
        # include(cmake/platforms/windows.cmake)
    endif(USE_WINDOWS_UNIVERSAL)

endif()

# ----------------- Notepad-- 其它主线构建相关  ----------------- #

