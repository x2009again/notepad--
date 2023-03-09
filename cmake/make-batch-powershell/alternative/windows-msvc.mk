# windows-msvc.mk
# Windows MSVC - 通用 Windows 平台 MSVC 构建方案

# 设定指向用于构建时引用的 MSVC Qt 库位置
QT_MSVC_PREFIX_DIR := C:\Qt\Qt5.14.2\5.14.2\msvc2017_64

CPUS:=$(shell powershell "(Get-CimInstance -ClassName Win32_Processor).NumberOfLogicalProcessors")

builddir  := build/windows-msvc
installdir:= $(builddir)/install
sourcedir := .
CMAKE_DEBUG     := -DCMAKE_BUILD_TYPE=Debug
CMAKE_RELEASE   := -DCMAKE_BUILD_TYPE=Release
CMAKE_CXX_FLAGS := -DCMAKE_CXX_FLAGS="/MP$(CPUS)"
CMAKE_OPTIONS   := $(CMAKE_CXX_FLAGS) -DUSE_WINDOWS_MSVC=ON 
CMAKE_MSVC_DIR  := -DCMAKE_PREFIX_PATH=$(QT_MSVC_PREFIX_DIR)
CMAKE_GENERATER := ""
CMAKE_MSVC_QT   := $(CMAKE_OPTIONS) -DWINDOWS_DEPLOY_QT=ON 
CMAKE_MSVC_QT5  := $(CMAKE_MSVC_QT) -DWINDOWS_DEPLOY_QT5=ON 
CMAKE_MSVC_QT6  := $(CMAKE_MSVC_QT) -DWINDOWS_DEPLOY_QT6=ON 


windows-msvc:
	cmake -B$(builddir) $(CMAKE_DEBUG) $(CMAKE_OPTIONS) $(CMAKE_MSVC_DIR)
	cmake --build $(builddir)

windows-msvc-release:
	cmake -B$(builddir) $(CMAKE_RELEASE) $(CMAKE_OPTIONS) $(CMAKE_MSVC_DIR)
	cmake --build $(builddir) --config Release

windows-msvc-release-qt5:
	cmake -B$(builddir) $(CMAKE_RELEASE) $(CMAKE_MSVC_QT5) $(CMAKE_MSVC_DIR)
	cmake --build $(builddir) --config Release

windows-msvc-release-qt6:
	cmake -B$(builddir) $(CMAKE_RELEASE) $(CMAKE_MSVC_QT6) $(CMAKE_MSVC_DIR)
	cmake --build $(builddir) --config Release

windows-msvc-release-qt5-install: windows-msvc-release-qt5
	cmake --install $(installdir)
	cmake --build $(builddir) --target windows-deployqt

windows-msvc-release-qt6-install: windows-msvc-release-qt6
	cmake --install $(installdir)
	cmake --build $(builddir) --target windows-deployqt