# framework-extension.cmake
    # framework_plugin_include <target> <plugin.cmake>
        # 声明本地资源引用构建
    # framework_plugin_include_with_git <target> <git> [arg...]
        # 声明资源引用构建 with_git

# framework_plugin_include <target> <plugin.cmake>
# 该宏定义了从指定的 plugin.cmake 中获取插件共享构建资源
    # 标记 FRAMEWORK_WANT_INCLUDE 为 ON，让 include(plugin.cmake) 时产生策略性引用构建
    # 清空 FRAMEWORK_INCLUDE_EXPORTS 以便干净的引用资源(头文件目录)得以填充到此处
    # 清空 FRAMEWORK_SOURCES_EXPORTS 以便干净的引用资源(源文件声明)得以填充到此处
    # 框架构建将头文件目录 使用原生方案 追加到目标属性
    # 框架构建将源文件使用 spark_file_glob 解析一切可能的存在，直接提供到目标的构建
macro(framework_plugin_include _target _plug_cmake)
    spark_framework_include(${_target} ${_plug_cmake})
endmacro(framework_plugin_include _target _plug_cmake)

# framework_plugin_include_with_git <target> <git_repo_url> [git_args...]
# 该宏定义了从指定的 git 仓库中获取插件源代码，并检查是否有 plugin.cmake，再转到引用资源构建
function(framework_plugin_include_with_git _target GIT_REPO_URL)
    spark_framework_include_with_git(${_target} ${GIT_REPO_URL} ${ARGN})
endfunction(framework_plugin_include_with_git _target GIT_REPO_URL)

# add_framework_plugin_with_git(https://gitee.com/ndd-community/notepad--plugin.plantuml-preview --branch=cmake-plugins-dev)
