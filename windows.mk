Msg   := 'Build with the following configuration:'
One   := '1. make -f windows-universal.mk'

all:
	@echo -e $(Msg)
	@echo -e $(One)    "\n\tThe default Universal Windows platform build"
