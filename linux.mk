Msg   := 'Build with the following configuration:'
One   := '1. make -f linux-universal.mk'
Two   := '2. make -f linux-debian.mk package'


all:
	@echo $(Msg)
	@echo $(One)    "\n\t默认的通用 Linux 平台构建."
	@echo $(Two)    "\n\t通用 Linux 平台的 Debian deb 构建."

