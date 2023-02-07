UNAME:=WIN32
UNAME:=$(shell uname)

# Win下使用Git Bash运行make

ifeq ($(UNAME), Linux)
all:linux
linux:
	make -f linux.mk package
else
all:
	make -f win.mk all
msvc:
	make -f win.mk msvc
mingw:
	make -f win.mk mingw
endif
