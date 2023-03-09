@echo off

set build_dir=build
set source_dir=.
set generator=-G Ninja
set build_type_debug=Debug
set build_type_release=Relase
set build_type=%build_type_debug%
set universal=ON
set qt=ON
set qt5=ON
set qt6=OFF



echo finding vcvars64.bat...
set vcvars=
for %%i in (C: D: E: F: G: H:) do (
	if exist %%i (
		pushd %%i\
		for /r %%j in (*vcvars64.bat) do (
			set vcvars=%%j
			popd
			goto :find_vcvars
		)
		popd
	)
)

:find_vcvars
if not "%vcvars%"=="" (
	call "%vcvars%"
	echo config MSVC environment...
)else (
	echo error: could not find vcvars64.bat MSVC ENV config file.
	goto :out
)

echo finding qt msvc path...
set qt_msvc=
for %%i in (D: D: E: F: G: H:) do (
	if exist %%i (
		pushd %%i\
		for /d /r %%j in (*msvc20*) do (
			echo %%j
			if exist %%j\bin (
				if exist %%j\bin\windeployqt.exe (
					if exist %%j\bin\Qt5Core.dll (
						set qt_msvc=%%j
						popd
						goto :find_qt_msvc
					)
				)
			)
		)
		popd
	)
)

:find_qt_msvc
if not "%qt_msvc%"=="" (
	echo config qt msvc path...
) else (
	echo error: could not find qt msvc path.
	goto :out
)

cmake -B%build_dir% CMAKE_BUILD_TYPE=%build_type% %generator% -DUSE_WINDOWS_UNIVERSAL=%universal% -DWINDOWS_DEPLOY_QT=%qt% -DWINDOWS_DEPLOY_QT5=%qt5% -DWINDOWS_DEPLOY_QT6=%qt6% -DCMAKE_PREFIX_PATH=%qt_msvc% %source_dir%
cmake --build %build_dir% -- 
xcopy %build_dir%\windows-deployqt\Notepad--.app\bin\ %build_dir%\bin\ /e

:out
pause