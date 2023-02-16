Msg   := 'Build with the following configuration:'
One   := '1. make -f linux-universal.mk'
Two   := '2. make -f linux-debian.mk package'
Three := '3. make -f linux-appimage.mk package'
Four  := '4. make -f linux-uos.mk package'


all:
	@echo $(Msg)
	@echo $(One)    "\n\t默认的通用 Linux 平台构建."
	@echo $(Two)    "\n\t通用 Linux 平台的 Debian deb 构建."
	@echo $(Three)  "\n\t通用 Linux 平台的 Appimage 构建."
	@echo $(Four)   "\n\t独立 Linux 平台的 Uos 构建."

