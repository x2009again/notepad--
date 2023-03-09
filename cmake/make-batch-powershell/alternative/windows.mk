Msg   := 'Build with the following configuration:'
One   := '1. mingw-make32.exe -f windows-universal.mk'
Two   := '2. mingw-make32.exe -f windows-mingw.mk'
Three := '3. mingw-make32.exe -f windows-msvc.mk'

all:
	@echo -e $(Msg)
	@echo -e $(One)    "\n\tThe default Universal Windows platform build"
	@echo -e $(Two)    "\n\tThe default Universal Windows Platform MinGW build."
	@echo -e $(Three)  "\n\tThe default Universal Windows Platform MSVC build."

