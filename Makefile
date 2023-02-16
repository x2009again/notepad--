# 主体构建入口

# 1. 在 Windows 中 $(OS) 为 Windows_NT
# 2. 在 Linux 中 $(OS) 为空的，此时应该使用 uname 
    # 注意：在 Linux 中 uanme -s 显示为 Linux
    # 注意：在 Linux 中某些内核 uanme -p 显示为 unknow 
# 3. 待验证：在 OSX 中 $(OS) 为空的，此时应该使用 uname 
    # 待验证：在 OSX 中 uname -s 显示为 Darwin
    # 待验证：在 OSX 中 uname -p 显示为 ?

# Windows 平台相关
ifeq ($(OS),Windows_NT)
    include windows.mk
# Unix 平台相关
else 
    UNAME_S := $(shell uname -s)
    # Linux 构建
    ifeq ($(UNAME_S),Linux)
		include linux.mk
    endif

    # MacOS 构建
    ifeq ($(UNAME_S),Darwin)
        include macos.mk
    endif
endif

