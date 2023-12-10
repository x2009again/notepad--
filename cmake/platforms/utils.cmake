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

# qt5_qt6_compatible_check <file> <var>
    # Qt5/6 兼容性构建检查方案，如果文件中存在 true 将认为可兼容构建
    # _CHECK_FILE Qt5/Qt6 兼容性检查文件
    # _VAR 检查结果
function(qt5_qt6_compatible_check _CHECK_FILE _VAR)
    set(${_VAR} true PARENT_SCOPE)
    file(READ ${_CHECK_FILE} file_content)
    string(FIND ${file_content} "true" _index)
    if(_index LESS 0) 
        # -1: It is not checked and passed
        set(${_VAR} false PARENT_SCOPE)
    endif(_index LESS 0)
endfunction(qt5_qt6_compatible_check _CHECK_FILE _VAR)


# windeployqt_install
    # Windows 在 install 目标时进行 windeployqt 的自动化
    # 根据
    # https://blog.nathanosman.com/2017/11/24/using-windeployqt-with-cpack.html
function(windeployqt_install TARGET)
    # string(TOLOWER "${ALIAS}_file"   _file)
    # string(TOLOWER "${ALIAS}_output" _output)

    set(WINDEPLOYQT_EXECUTABLE "${WINDOWS_QT_DIR}/../../../bin/windeployqt")
    #
    file(GENERATE OUTPUT "${CMAKE_BINARY_DIR}/${TARGET}_PATH"
        CONTENT "$<TARGET_FILE:${TARGET}>"
    )
    install(CODE
        "
        file(READ \"${CMAKE_BINARY_DIR}/${TARGET}_PATH\" _file)
        execute_process(
            COMMAND \"${WINDEPLOYQT_EXECUTABLE}\"
                    # 虚假的运行 windeployqt 而不复制任何内容
                    --dry-run
                    # 扫描QML-从目录开始导入。
                    --qmlimport ${WINDOWS_QT_DIR}/../../../qml
                    # 部署编译器运行时(仅限桌面)。
                    --compiler-runtime
                    # 以源 目标的输出形成映射关系，以便用于解析内容
                    --list mapping
                    \${_file}
            OUTPUT_VARIABLE _output
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )

        # 将内容转为一个列表，使用 WINDOWS_COMMAND 类型的处理方式
        separate_arguments(_files WINDOWS_COMMAND \${_output})

        while(_files)            
            list(GET _files 0 _src)
            list(GET _files 1 _dest)
            execute_process(
                COMMAND \"\${CMAKE_COMMAND}\" -E
                    copy_if_different \${_src} \"\${CMAKE_INSTALL_PREFIX}/bin/\${_dest}\"
            )
            message(\"COPY \${_src} \${CMAKE_INSTALL_PREFIX}/bin/\${_dest}\")
            list(REMOVE_AT _files 0 1)
        endwhile(_files)
        "
    )
endfunction(windeployqt_install TARGET)

# 
# [UnTested]macdeployqt_install
    # MacOSX 在 install 目标时进行 macdeployqt 的自动化
    # 由于
function(macdeployqt_install _target)
    set(MACDEPLOYQT_EXECUTABLE "${MACOS_QT_DIR}/../../../bin/macdeployqt")
    #
    file(GENERATE OUTPUT "${CMAKE_BINARY_DIR}/${_target}_PATH"
        # CONTENT "$<TARGET_FILE:${_target}>"
        CONTENT "${CMAKE_BINARY_DIR}/_CPack_Packages/Darwin/Bundle/${_target}-${PROJECT_VERSION}-Darwin/${_target}.app"
    )
    install(CODE
        "
        file(READ \"${CMAKE_BINARY_DIR}/${_target}_PATH\" _file)
        execute_process(
            COMMAND \"${MACDEPLOYQT_EXECUTABLE}\"
                    # 虚假的运行 macdeployqt 而不复制任何内容
                    --dry-run
                    # 扫描QML-从目录开始导入。
                    #--qmlimport=${MACOS_QT_DIR}/../../../qml
                    # 部署编译器运行时(仅限桌面)。
                    #--compiler-runtime
                    # 以源 目标的输出形成映射关系，以便用于解析内容
                    #--list mapping
                    #-no-plugins
                    #-timestamp
                    \${_file}
            OUTPUT_VARIABLE _output
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        "
    )
endfunction(macdeployqt_install _target)
