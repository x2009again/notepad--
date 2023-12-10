Msg   := 'Build with the following configuration:'
One   := '1. make macosx-universal'
Two   := '1. make macosx-universal-release'

all:
	@echo $(Msg)
	@echo $(One)    "\n\t默认的通用 MacOSX 平台构建(Debug)."
	@echo $(Two)    "\n\t默认的通用 MacOSX 平台构建(Release)."

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
