
option(USE_NOTEPAD_PLUGIN "构建 Notepad-- 内部插件" ON)
option(USE_NOTEPAD_PLUGIN_BASE "构建 Notepad-- 内部的插件基础模板" OFF)


if(USE_NOTEPAD_PLUGIN)

    # [核心]：[引入插件框架核心模块]
    include(framework/framework.cmake)
    include(framework/framework-include.cmake)

    if(USE_NOTEPAD_PLUGIN_BASE) # 早期构建，准备废弃
        # 默认的测试插件
        add_subdirectory(test)
        
        # 被 cmake-plugins-dev 构建的
        # 基于源代码构建的系列插件模板
        set(base-plugin_ENABLE ON)
        add_framework_plugin(base-plugin
            template-plugins/base-plugin)
        set(base-secondary-plugin_ENABLE ON)
        add_framework_plugin(base-secondary-plugin
            template-plugins/base-secondary-plugin
        )
        set(base-secondary-widget-plugin_ENABLE ON)
        add_framework_plugin(base-secondary-widget-plugin
            template-plugins/base-secondary-widget-plugin
        )
        # add_subdirectory(template-plugins/base-widget-plugin)
        # add_subdirectory(template-plugins/base-widget-ui-plugin)
        # add_subdirectory(template-plugins/base-secondary-menu-plugin)
        # add_subdirectory(template-plugins/base-secondary-menu-plugin-v1)
        # add_subdirectory(template-plugins/base-secondary-menu-ui-plugin-v1)
    endif(USE_NOTEPAD_PLUGIN_BASE)

    # [开发者计划]：不改变本 CMakeLists.txt 的情况下开发自己的插件
    # 非冲突化开发方式，在 dev.cmake 中建立自己的本地引用或在线引用来构建自己的插件。
    # 并将其作为开发者计划的关注位置点，以此保证本 CMakeLists.txt 可以直接随项目更新
    include(dev.cmake)
    
    ## [框架：插件目标直接构建]
    # add_framework_plugin <target> [<dir>...] [<file>...]
    # 1. 构建一个基于 framework 的插件
        # 基于提供的包含源代码文件的的路径，或指定的要构建的源代码
    add_framework_plugin(framework-helloworld
        framework-plugins/helloworld
    )

    # 2. 简单的 framework-hello 插件
        # 基于提供的包含源代码文件的的路径，或指定的要构建的源代码
    add_framework_plugin(framework-hello-simple
        ${PROJECT_SOURCE_DIR}/src/utils
        framework-plugins/hello-simple
        framework-plugins/hello-simple/view
        framework-plugins/hello-simple/template
        framework-plugins/hello-simple/buildin.qrc)

    
    ## [框架：插件描述引用构建]
    # include(path/to/plugin.cmake)
    # 1. 使用本地 plugin.cmake 引用构建(示例)


    ## [框架：插件 with_git 的引用构建 - 与在线]
    # add_framework_plugin_with_git <git> [args...]
        # [框架引用：插件扩展处的资源引用构建]
            # framework_plugin_include <target> <git>
            # framework_plugin_include_with_git <target> <git> [args...]

    # 1. 基于 git 仓库在线构建 helloworld 插件
    # add_framework_plugin_with_git(
    #     https://gitee.com/zinface/notepad--plugin.helloworld
    #     --branch=cmake-plugins-dev)

    # 2. 一个 plantuml 预览插件
        # (使用 with_git 引用的在线构建)
    # add_framework_plugin_with_git(
    #     https://gitee.com/ndd-community/notepad--plugin.plantuml-preview
    #     --branch=cmake-plugins-dev)
        
    # 3. 一个简单的 svg 预览插件
        # (使用 with_git 引用的在线构建)
        # (使用了 plantuml 预览插件的共享的资源定义)
    # add_framework_plugin_with_git(
    #     https://gitee.com/ndd-community/notepad--plugin.svg-preview
    #     --branch=cmake-plugins-dev)

    # 4. 一个使用引用构建的 plantumlexamples 插件
        # (使用 with_git 引用的在线构建)
        # (使用了 plantuml 预览插件的共享的资源定义)
        # (使用了 framework-plugin-component-library 提供的共享的资源定义)
    # add_framework_plugin_with_git(
    #     https://gitee.com/zinface/notepad--plugin.plantuml-examples
    #     --branch=cmake-plugins-dev)


    ## 例外：早期的构建模式，即将准备废弃
    # 一个简单的插件示例，但依赖于外部的 opencc 项目(基于 git)
    # add_subdirectory(opencc-demo-plugin)
    # # 一个简单的版本更新检查的插件
    # add_subdirectory(versionUpdate)
    # # 一个简单的可二次扩展的插件
    # add_subdirectory(external-plugin)

    # # Linux 可构建的插件
    # if(UNIX AND NOT APPLE)
    #     # 一个简单的 Linux 终端插件，但依赖于外部的 lxqt-qterminal
    #     add_subdirectory(linux-terminal-plugin)
    # endif(UNIX AND NOT APPLE)
    
    # # TTS 语音合成
    # add_subdirectory(TTS-plugin)

endif(USE_NOTEPAD_PLUGIN)
