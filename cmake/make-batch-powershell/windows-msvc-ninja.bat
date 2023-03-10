:: windows-msvc-ninja.bat

:: Windows MSVC - 通用 Windows 平台 MSVC 构建方案
:: 此文件需以 GBK 方案保存，并在 PowerShell 中运行

@echo off

:: 预固化配置
set builddir=build/windows-universal
set installdir=%builddir%/install

::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
::                                                          ::
::      !!! 定制: 指向用于构建时引用的 MSVC Qt 库位置!!!         ::
set QT_MSVC_PREFIX_DIR=C:\Qt\Qt5.14.2\5.14.2\msvc2017_64
::                                                          ::
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


:: 构建选项预定义
set CMAKE_OPTIONS=
set CMAKE_OPTIONS=%CMAKE_OPTIONS% -G Ninja
set CMAKE_OPTIONS=%CMAKE_OPTIONS% -DCMAKE_PREFIX_PATH=%QT_MSVC_PREFIX_DIR%
@REM set CMAKE_OPTIONS=%CMAKE_OPTIONS% -DCMAKE_BUILD_TYPE=Debug
@REM set CMAKE_OPTIONS=%CMAKE_OPTIONS% -DUSE_WINDOWS_UNIVERSAL=ON
@REM set CMAKE_OPTIONS=%CMAKE_OPTIONS% -DWINDOWS_DEPLOY_QT=ON
@REM set CMAKE_OPTIONS=%CMAKE_OPTIONS% -DWINDOWS_DEPLOY_QT5=ON
@REM set CMAKE_OPTIONS=%CMAKE_OPTIONS% -DWINDOWS_DEPLOY_QT6=ON
@REM set CMAKE_OPTIONS=%CMAKE_OPTIONS% -DUSE_QT6=ON

:: 在配置时 installdir 表示将要安装的路径
set CMAKE_OPTIONS=%CMAKE_OPTIONS% -DCMAKE_INSTALL_PREFIX=%installdir%


:: --------------------------------------------------- ::
:: 检查可用配置接收参数
if "%1" == "run" (
    :: 此处 \ 与 / 相反
    build\windows-universal\windows-deployqt\Notepad--.app\bin\Notepad--.exe
    goto :finish
)
:: --------------------------------------------------- ::

@echo ---- 即将构建 Notepad-- 项目 ----

:: 1. 询问构建的类型
@echo 1. 构建 Debug(默认), 2. 构建 Release
set /p chose="请输入序号: "

if "%chose%" == "2" (
    set CMAKE_BUILD_TYPE=Relase
    set CMAKE_OPTIONS=%CMAKE_OPTIONS% -DCMAKE_BUILD_TYPE=Relase
) else (
    set CMAKE_BUILD_TYPE=Debug
    set CMAKE_OPTIONS=%CMAKE_OPTIONS% -DCMAKE_BUILD_TYPE=Debug
)

:: 2. 询问 QSci 构建的类型
@echo ---- 是否指定 QScint 构建为动态库? 默认(OFF) ----
set /p useshare=输入(on):

if "%useshare%" == "on" (
    set CMAKE_OPTIONS=%CMAKE_OPTIONS% -DNOTEPAD_BUILD_BY_SHARED=ON
)

:: 3. 询问配置 USE_WINDOWS_UNIVERSAL
@echo ---- 是否配置 USE_WINDOWS_UNIVERSAL? 默认(ON) ----
set /p universal=输入(n):

if not "%universal%" == "n" (
    set CMAKE_OPTIONS=%CMAKE_OPTIONS% -DUSE_WINDOWS_UNIVERSAL=ON
)

:: 4. 询问配置 WINDOWS_DEPLOY_QT 
@echo ---- 是否配置 WINDOWS_DEPLOY_QT? 默认(ON) ----
set /p deployqt=输入(n):

if not "%deployqt%" == "n" (
    set CMAKE_OPTIONS=%CMAKE_OPTIONS% -DWINDOWS_DEPLOY_QT=ON
)

:: 5. 询问配置 WINDOWS_DEPLOY_QT5 或 WINDOWS_DEPLOY_QT6 USE_QT6
@echo ---- 是否配置 WINDOWS_DEPLOY_QT5 版本? 默认(5,可选6) ----
set /p deployqt5=输入(6):

if not "%deployqt5%" == "6" (
    set CMAKE_OPTIONS=%CMAKE_OPTIONS% -DWINDOWS_DEPLOY_QT5=ON
) else (
    set CMAKE_OPTIONS=%CMAKE_OPTIONS% -DWINDOWS_DEPLOY_QT6=ON -DUSE_QT6=ON
)

:: ------------------------------------------------------------------- ::
@echo ----------------------- 构建前配置预览 ---------------------------
@echo "cmake -B%builddir% %CMAKE_OPTIONS% ."
@echo "cmake --build %builddir% --config %CMAKE_BUILD_TYPE%"
@echo "cmake --install %builddir% --prefix %installdir%"
@echo "cmake --build %builddir% --target windows-deployqt"
:: ------------------------------------------------------------------- ::


:: 构建前配置 - 寻找硬盘中的 msvc 环境配置
@echo ---- 寻找硬盘中的 msvc 环境配置 ----

set vcvars=
for %%d in (C: D: E: F: G: H:) do (
    if exist %%d (
        @echo 正在查找硬盘 %%d 中的 VC 配置环境...
        pushd %%d\
        for /r %%p in (*vcvars64.bat) do (
            @echo 发现配置文件: %%p
            set vcvars=%%p
            popd
            goto :set_vcvars
        )
        popd
    )
)

:set_vcvars
if "%vcvars%" == "" (
    @echo "could not found vcvars64.bat file"
    goto :finish
) else (
    @echo "config MSVC environment..."
    call "%vcvars%" amd64
)


:: 正式对项目进行配置、构建、安装、部署
@echo ------------ 准备构建 ------------
cmake -B%builddir% %CMAKE_OPTIONS% .
cmake --build %builddir% --config %CMAKE_BUILD_TYPE%
cmake --install %builddir% --prefix %installdir%
cmake --build %builddir% --target windows-deployqt
goto :finish


:: 正常或异常退出的定位
:finish
pause

:: 本配置模板可在 cmd 命令行 或 Powershell 中运行
:: 本配置模板复制到项目根目录，并定制，可在 Windows 中双击运行
:: 本配置可