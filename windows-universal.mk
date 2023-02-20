# windows-universal.mk
# Windows Universal - 通用 Windows 平台构建方案

builddir  := build
sourcedir := .
CMAKE_DEBUG     := -DCMAKE_BUILD_TYPE=Debug
CMAKE_RELEASE   := -DCMAKE_BUILD_TYPE=Release
CMAKE_OPTIONS   := -DUSE_WINDOWS_UNIVERSAL=ON
CMAKE_GENERATER := ""
CMAKE_UNIVERSAL_QT  := $(CMAKE_OPTIONS)      -DWINDOWS_DEPLOY_QT=ON 
CMAKE_UNIVERSAL_QT5 := $(CMAKE_UNIVERSAL_QT) -DWINDOWS_DEPLOY_QT5=ON 
CMAKE_UNIVERSAL_QT6 := $(CMAKE_UNIVERSAL_QT) -DWINDOWS_DEPLOY_QT6=ON 

JOBS=$(shell nproc)


windows-universal:
	cmake -B$(builddir) $(CMAKE_DEBUG) $(CMAKE_GENERATER) $(CMAKE_OPTIONS) 
	cmake --build $(builddir) -- -j$(JOBS)

windows-universal-release:
	cmake -B$(builddir) $(CMAKE_RELEASE) $(CMAKE_GENERATER) $(CMAKE_OPTIONS) 
	cmake --build $(builddir) -- -j$(JOBS)

windows-universal-release-qt5:
	cmake $(CMAKE_GENERATER) -B$(builddir) $(CMAKE_RELEASE) $(CMAKE_UNIVERSAL_QT5)
	cmake --build $(builddir) -- -j$(JOBS)

windows-universal-release-qt6:
	cmake $(CMAKE_GENERATER) -B$(builddir) $(CMAKE_RELEASE) $(CMAKE_UNIVERSAL_QT6)
	cmake --build $(builddir) -- -j$(JOBS)