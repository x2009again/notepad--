# Linux Appimage - 通用 Linux 平台 Appimage 构建方案

include linux-universal.mk

CPUS=$(shell nproc)

builddir  := build/linux-appimage
# sourcedir := .
# CMAKE_DEBUG     := -DCMAKE_BUILD_TYPE=Debug
# CMAKE_RELEASE   := -DCMAKE_BUILD_TYPE=Release
CMAKE_OPTIONS   := -DUSE_LINUX_APPIMAGE=ON $(CMAKE_OPTIONS)


# -------------------------------- Appimage Build Tools -------------------------------- #
# Appimage 的构建流 --
# 在 Makefile 进行构建目标构建 Appimage (要求提供工具的绝对路径，然后可依次进行linuxdeployqt, genrate-appimage)
# 来自于 https://github.com/probonopd/linuxdeployqt 	的 linuxdeployqt
# 来自于 https://github.com/AppImage/AppImageKit		的 appimagetool
# 来自于 https://gitlink.org.cn/zinface/bundle-linuxdeployqt.git  托管存储的工具

# 或指定你所想存放克隆项目的位置
BUNDLE_LINUXDEPLOYQT := $(shell pwd)/$(builddir)/bundle-linuxdeployqt

download-bundle-linuxdeploytools:
	-git clone https://gitlink.org.cn/zinface/bundle-linuxdeployqt.git $(BUNDLE_LINUXDEPLOYQT)

LINUXDEPLOYQT := "$(BUNDLE_LINUXDEPLOYQT)/linuxdeployqt-continuous-x86_64.AppImage"
APPIMAGETOOL  := "$(BUNDLE_LINUXDEPLOYQT)/appimagetool-x86_64.AppImage"

# 追加 Appimagetool、linuxdeployqt 构建配置
CMAKE_OPTIONS := -DLINUXDEPLOYQT=$(LINUXDEPLOYQT) -DAPPIMAGETOOL=$(APPIMAGETOOL) $(CMAKE_OPTIONS)

linuxdeploy: download-bundle-linuxdeploytools
	cmake -B$(builddir) $(CMAKE_OPTIONS)
	cmake --build $(builddir) -- linuxdeploy

genrate-appimage:
	cmake -B$(builddir) $(CMAKE_OPTIONS)
	cmake --build $(builddir) -- appimage


package: linux-universal-release linuxdeploy genrate-appimage

linux-build-options:
	@echo $(CMAKE_OPTIONS)

# 此配置为构建 linux Appimage 通用版本构建