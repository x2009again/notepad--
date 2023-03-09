# windows-mingw.mk
# Windows MinGW - 通用 Windows 平台 MinGW 构建方案

builddir  := build/windows-mingw
sourcedir := .
CMAKE_DEBUG     := -DCMAKE_BUILD_TYPE=Debug
CMAKE_RELEASE   := -DCMAKE_BUILD_TYPE=Release
CMAKE_OPTIONS   := -DUSE_WINDOWS_MINGW=ON
CMAKE_MINGW_DIR := -DCMAKE_PREFIX_PATH=""
CMAKE_GENERATER := -G"CodeBlocks - MinGW Makefiles"
CMAKE_MINGW_QT  := $(CMAKE_OPTIONS)  -DWINDOWS_DEPLOY_QT=ON 
CMAKE_MINGW_QT5 := $(CMAKE_MINGW_QT) -DWINDOWS_DEPLOY_QT5=ON 
CMAKE_MINGW_QT6 := $(CMAKE_MINGW_QT) -DWINDOWS_DEPLOY_QT6=ON 

JOBS=$(shell nproc)

# mingw32-make.exe
QT_MINGW_PREFIX_DIR := "-DCMAKE_PREFIX_PATH=C:\Qt\Qt5.14.2\5.14.2\mingw73_64"

windows-mingw:
	cmake $(CMAKE_GENERATER) -B$(builddir) $(CMAKE_DEBUG) $(CMAKE_OPTIONS) $(CMAKE_MINGW_DIR)
	$(MAKE) -C $(builddir) -j$(JOBS)

windows-mingw-release:
	cmake $(CMAKE_GENERATER) -B$(builddir) $(CMAKE_RELEASE) $(CMAKE_OPTIONS) $(CMAKE_MINGW_DIR)
	$(MAKE) -C $(builddir) -j$(JOBS)

windows-mingw-release-qt5:
	cmake $(CMAKE_GENERATER) -B$(builddir) $(CMAKE_RELEASE) $(CMAKE_MINGW_QT5) $(CMAKE_MINGW_DIR)
	$(MAKE) -C $(builddir) -j$(JOBS)

windows-mingw-release-qt6:
	cmake $(CMAKE_GENERATER) -B$(builddir) $(CMAKE_RELEASE) $(CMAKE_MINGW_QT6) $(CMAKE_MINGW_DIR)
	$(MAKE) -C $(builddir) -j$(JOBS)