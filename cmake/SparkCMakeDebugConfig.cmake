# SparkCMakeDebugConfig.cmake

option(SPARK_CMAKE_DEBUG "CMake内部信息调试能力" ON)

set(SPARK_CMAKE_DEBUG_FILE   "${CMAKE_BINARY_DIR}/spark_debug.txt")
set(SPARK_CMAKE_CONTENT_FILE "${CMAKE_BINARY_DIR}/spark_debug_content.txt")

# 模块内部初始化空信息文件
file(WRITE ${SPARK_CMAKE_CONTENT_FILE} "")

# spark_cmake_debug <message> ...
# 添加、增量需要准备生成的的调试内容。
function(spark_cmake_debug)
    list(JOIN ARGN "\n" _content)
    file(APPEND ${SPARK_CMAKE_CONTENT_FILE} "${_content}\n")
endfunction()

# spark_generator_cmake_debug <target>
# 生成 <target> 目标来驱动打印调试内容
function(spark_generator_cmake_debug _target)
    add_custom_target(${_target}
        COMMAND ${CMAKE_COMMAND} -E cat
            ${SPARK_CMAKE_DEBUG_FILE}
    )
    file(GENERATE
        OUTPUT 
            ${SPARK_CMAKE_DEBUG_FILE}
        INPUT
            ${SPARK_CMAKE_CONTENT_FILE})
endfunction(spark_generator_cmake_debug _target)


#
# 1. 使用 spark_cmake_debug( <content> [...]) 
#   添加、增量需要准备生成的的调试内容。
#   支持普通文本、变量求值、生成器表达式求值。
#
# 2. 使用 spark_generator_cmake_debug( <target> ) 
#   生成 <target> 目标来驱动打印调试内容
#   仅建议在全局结束位置使用此 function 
#
