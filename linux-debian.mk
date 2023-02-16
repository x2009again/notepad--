# Linux Debian - 通用 Linux 平台 Debian 构建方案

include linux-universal.mk

CPUS=$(shell nproc)

builddir  := build/linux-debian
# sourcedir := .
# CMAKE_DEBUG     := -DCMAKE_BUILD_TYPE=Debug
# CMAKE_RELEASE   := -DCMAKE_BUILD_TYPE=Release
CMAKE_OPTIONS   := -DUSE_LINUX_DEBIAN=ON $(CMAKE_OPTIONS)

package: linux-universal-release
	make -C $(builddir) package

linux-build-options:
	@echo $(CMAKE_OPTIONS)

# 此配置为构建 linux debian 通用版本构建