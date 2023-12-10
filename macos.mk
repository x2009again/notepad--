Msg   := 'Build with the following configuration:'
One   := '1. make macosx-universal'
Two   := '2. make macosx-universal-release'
Three := '3. make package'

all:
	@echo $(Msg)
	@echo $(One)    "\n\t通用 MacOSX 平台构建(Debug)."
	@echo $(Two)    "\n\t通用 MacOSX 平台构建(Release)."
	@echo $(Three)  "\n\t通用 MacOSX 平台构建 CPack 打包."

builddir  := build/macosx-universal
sourcedir := .
CMAKE_DEBUG     := -DCMAKE_BUILD_TYPE=Debug
CMAKE_RELEASE   := -DCMAKE_BUILD_TYPE=Release
CMAKE_OPTIONS   := -DUSE_MACOSX_UNIVERSAL=ON

CPUS=8

macosx-universal:
	cmake -B$(builddir) $(CMAKE_OPTIONS) $(CMAKE_DEBUG)
	cmake --build $(builddir) -- -j$(CPUS)

macosx-universal-release:
	cmake -B$(builddir) $(CMAKE_OPTIONS) $(CMAKE_RELEASE)
	cmake --build $(builddir) -- -j$(CPUS)


package: macosx-universal-release
	make -C $(builddir) package