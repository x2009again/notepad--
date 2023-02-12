cmake_minimum_required(VERSION 3.22)

project(Notepad-- VERSION 1.22.0)

# -------------- Notepad -- 全局构建配置 ------------- #
option(USE_QT6 "使用 Qt6 进行编译本项目与随带所有内容" OFF)

include(cmake/SparkEnvConfig.cmake)
include(cmake/SparkMacrosConfig.cmake)
if(USE_QT6)
    include(cmake/SparkFindQt6Config.cmake)
else()
    include(cmake/SparkFindQt5Config.cmake)
endif(USE_QT6)
include(cmake/SparkMacrosExtendConfig.cmake)


# ----------------- 构建之前 全局定义 ----------------- #
# 在 Windows 中开启 UNICODE 以支持 TCHAR 功能
if(WIN32)
    add_compile_definitions(_UNICODE= UNICODE=)
endif()

# ----------------- 构建 QSci 库 ----------------- #
if(TRUE)
    include(cmake/modules/QScint.cmake)
endif(TRUE)


# ----------------- 构建 Notepad-- 程序 ----------------- #
if(TRUE)
    include(cmake/modules/Notepad--.cmake)
endif(TRUE)

# ----------------- Notepad-- 其它主线构建相关  ----------------- #
option(USE_WINDOWS_UNIVERSAL "通用 Windows 平台构建"          OFF)
option(USE_WINDOWS_MINGW     "通用 Windows 平台 MinGW 构建"   OFF)
option(USE_WINDOWS_MSVC      "通用 Windows 平台 MSVC 构建"    OFF)
#
option(WINDOWS_DEPLOY_QT     "通用 Windows 平台 Qt 部署"                 OFF)
option(WINDOWS_DEPLOY_QT5    "通用 Windows 平台指定 Qt5 部署 windeployqt" OFF)
option(WINDOWS_DEPLOY_QT6    "通用 Windows 平台指定 Qt6 部署 windeployqt" OFF)
#
option(USE_LINUX_UNIVERSAL   "通用 Linux 平台构建"            OFF)
option(USE_LINUX_DEBIAN      "通用 Debian Linux 平台构建"     OFF)
option(USE_LINUX_APPIMAGE    "通用 Appimage Linux 平台构建"   OFF)
#
option(USE_LINUX_UOS         "独立 Linux 平台的 Uos 构建"      OFF)
#
option(USE_MACOS_UNIVERSAL   "通用 MacOS 平台构建"            OFF)

# 使用 windows.cmake / linux.cmake / macos.cmake 维护构建.

    # 在 Windows 中此处将由通用构建、MinGW 构建、MSVC 构建组成
if(WIN32)
    
    # 通用 Windows 平台构建，一切的起始
    if(USE_WINDOWS_UNIVERSAL)
        include(cmake/platforms/windows-universal.cmake)

        # 为 Windows 中构建的应用进行 windeployqt 的配方
        include(cmake/platforms/windows-deployqt.cmake)
    
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
        # include(cmake/platforms/windows-other.cmake)
    endif(USE_WINDOWS_UNIVERSAL)

    # 在 Linux 中此处将由通用构建、Debian 打包构建、Appimage 打包构建、Uos 打包构建组成
elseif(UNIX AND NOT APPLE AND NOT WIN32 AND NOT ANDROID)

    # 通用 Linux 平台构建，一切的起始
    if(USE_LINUX_UNIVERSAL)
        include(cmake/platforms/linux-universal.cmake)

        # 通用 Linux 平台的 Debian deb 构建
        if(USE_LINUX_DEBIAN)    
            include(cmake/platforms/linux-debian.cmake)
        endif()

        # 通用 Linux 平台的  Appimage  构建
        if(USE_LINUX_APPIMAGE)  
            include(cmake/platforms/linux-appimage.cmake)
        endif()

        # 独立 Linux 平台的 Uos 构建
    elseif(USE_LINUX_UOS)
        include(cmake/platforms/linux-uos.cmake)

        # 其它 Linux 平台的构建
    else()
        # include(cmake/platforms/linux-other.cmake)
    endif(USE_LINUX_UNIVERSAL)

    # 通用 MacOS 平台的构建(一个无具体实现的空构建)
elseif(APPLE)

    # include(cmake/platforms/macos.cmake)
endif()

# ----------------- Notepad-- 其它主线构建相关  ----------------- #

