set Msg='Build with the following configuration:'
set One='1. mingw-make32.exe -f windows-universal.mk'
set Two='2. mingw-make32.exe -f windows-mingw.mk'
set Three='3. mingw-make32.exe -f windows-msvc.mk'


@echo %Msg%
@echo %One%    "\n\tThe default Universal Windows platform build"
@echo %Two%    "\n\tThe default Universal Windows Platform MinGW build."
@echo %Three%  "\n\tThe default Universal Windows Platform MSVC build."

