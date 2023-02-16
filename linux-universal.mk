# Linux Universal - 通用 Linux 平台 构建方案

CPUS=$(shell nproc)

builddir  := build/linux-universal
sourcedir := .
CMAKE_DEBUG     := -DCMAKE_BUILD_TYPE=Debug
CMAKE_RELEASE   := -DCMAKE_BUILD_TYPE=Release
CMAKE_OPTIONS   := -DUSE_LINUX_UNIVERSAL=ON


linux-universal:
	cmake -B$(builddir) $(CMAKE_OPTIONS) $(CMAKE_DEBUG)
	cmake --build $(builddir) -- -j$(CPUS)


linux-universal-release:
	cmake -B$(builddir) $(CMAKE_OPTIONS) $(CMAKE_RELEASE)
	cmake --build $(builddir) -- -j$(CPUS)

# 此配置为构建 linux 通用版本构建