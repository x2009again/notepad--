# utils.cmake
    # 定义一些其它工具函数
    
# get_current_platform_lib_name <_VAR> <_IS_IS_SHREAD> <_LIB>
    # _VAR          用于存储内容的变量
    # _IS_IS_SHREAD 是否为共享库
    # _LIB          目标名称
function(get_current_platform_lib_name _VAR _IS_SHREAD _LIB)
    set(_LIB_NAME  "")
    if(WIN32)
        set(_LIB_NAME ${_LIB}.lib)
    elseif(UNIX AND NOT APPLE)
        if(${_IS_SHREAD})
            set(_LIB_NAME lib${_LIB}.so)
        else()
            set(_LIB_NAME lib${_LIB}.a)
        endif(${_IS_SHREAD})
    elseif(APPLE)
        if(${_IS_SHREAD})
            set(_LIB_NAME lib${_LIB}.dylib)
        else()
            set(_LIB_NAME lib${_LIB}.a)
        endif(${_IS_SHREAD})
    else()
        message("ERROR: Unknow current platform")
        set(_LIB_NAME ${_LIB})
    endif()
    message("[utils.cmake] get_current_platform_lib_name: ${_LIB}(${_LIB_NAME}) ")
    set(${_VAR} ${_LIB_NAME} PARENT_SCOPE)
endfunction(get_current_platform_lib_name _VAR _IS_SHREAD _LIB)
# 获取当前平台的 lib 名称
# 在 Windows 中
    # 用于编译的静态库与动态库后缀相同： .lib
    # 并且不是 lib 开头
# 在 Linux 中
    # 用于编译的静态库与动态库后缀分别是：.a 与 .so
    # 文件名称由 lib 开头
# 在 MacOS 中
    # 用于编译的静态库与动态库后缀分别是：.a 与 .dylib
    # 文件名称由 lib 开头
