
# 基于 powershell 环境编写
# windows 下mingw32-make.exe cmake.exe msbuild.exe cl.exe工具链编译
# 使用方式 mingw32-make -f win-mingw-make-msbuild.mk

# 填入cmake程序完整路径
CMAKE_TOOL:=D:\Soft\mingw64\bin\cmake.exe
# 填入cpack程序完整路径
CPACK_TOOL:=D:\Soft\mingw64\bin\cpack.exe
# 填入MSVC工具链路径
# 填入msbuild程序完整路径
MSBUILD_TOOL:=C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\msbuild.exe
# 填入cl程序完整路径
CL_COMPILER:=C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.29.30133\bin\Hostx64\x64\cl.exe

# 填入Qt的msvc相关工具安装路径
# 填入MSVC Qt库查找路径
Qt_MSVC_PREFIX_PATH:=d:\Soft\Qt\5.15.2\msvc2019_64

NUM_LOGICAL_PROCESSOR=$(shell powershell (Get-CimInstance -ClassName Win32_Processor).NumberOfLogicalProcessors)

ifneq ($(shell powershell Test-Path build\bin\plugin), True)
	MKDIR:=mkdir -p build\bin\plugin
endif

all:msvc

msvc:
	${CMAKE_TOOL} -Bbuild -DCMAKE_BUILD_TYPE=Release -DPLUGIN_EN=on -DCMAKE_PREFIX_PATH=${Qt_MSVC_PREFIX_PATH} .
	${CMAKE_TOOL} --build ./build --config=Release -j${NUM_LOGICAL_PROCESSOR}
	$(shell powershell New-Item -ErrorAction SilentlyContinue -Type Directory build\bin\plugin)
	$(shell powershell cp -Force build\Release\NotePad--.exe build\bin\)
	cd build/bin && ${Qt_MSVC_PREFIX_PATH}\bin\windeployqt.exe  --qmldir=${Qt_MSVC_PREFIX_PATH}\bin\qml NotePad--.exe
	$(shell powershell cp -r -Force build\src\*\*\Release\*.dll build\bin\plugin)
	$(shell powershell cp -r -Force build\src\*\Release\*.lib build\bin\plugin)
	cd build && ${CPACK_TOOL} --config CPackConfig.cmake

