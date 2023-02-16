# 独立 Linux 平台的 Uos 构建

include linux-universal.mk

# 覆盖 linux-universal.mk 中定义的部分
linux-universal:
	@echo "此目标不应该由 UOS 配方构建"
linux-universal-release:
	@echo "此目标不应该由 UOS 配方构建"


CPUS=$(shell nproc)

builddir  := build/linux-uos
# sourcedir := .
# CMAKE_DEBUG     := -DCMAKE_BUILD_TYPE=Debug
# CMAKE_RELEASE   := -DCMAKE_BUILD_TYPE=Release
CMAKE_OPTIONS   := -DUSE_LINUX_UOS=ON


linux-uos:
	cmake -B$(builddir) $(CMAKE_OPTIONS) $(CMAKE_DEBUG)
	cmake --build build -- -j$(CPUS)

linux-uos-release:
	cmake -B$(builddir) $(CMAKE_OPTIONS) $(CMAKE_RELEASE)
	cmake --build build -- -j$(CPUS)

package:
	cmake -B$(builddir) $(CMAKE_OPTIONS) $(CMAKE_RELEASE)
	cmake --build $(builddir) -- -j$(CPUS) package

package-contents:
	-cd $(builddir)/_CPack_Packages/Linux/DEB/ && find

package-contents-tree:
	-tree $(builddir)/_CPack_Packages/Linux/DEB/

# 此配置为构建 linux 通用版本构建