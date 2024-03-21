
option(USE_NOTEPAD_PLUGIN "构建 Notepad-- 内部插件" ON)
option(USE_NOTEPAD_PLUGIN_BASE "构建 Notepad-- 内部的插件基础模板" OFF)


if(USE_NOTEPAD_PLUGIN)

    if(USE_NOTEPAD_PLUGIN_BASE)
        # 默认的测试插件
        add_subdirectory(test)
        # 被 cmake-plugins-dev 构建的 helloworld 插件
        add_subdirectory(helloworld-next)
        # 基于源代码构建的系列插件模板
        add_subdirectory(template-plugins/base-plugin)
        add_subdirectory(template-plugins/base-widget-plugin)
        add_subdirectory(template-plugins/base-widget-ui-plugin)
        add_subdirectory(template-plugins/base-secondary-menu-plugin)
        add_subdirectory(template-plugins/base-secondary-menu-plugin-v1)
        add_subdirectory(template-plugins/base-secondary-menu-ui-plugin-v1)
    endif(USE_NOTEPAD_PLUGIN_BASE)

    # 构建一个简单的基于 framwork 的 framework-helloworld 插件

    include(framework/framework.cmake)
    # add_framework_plugin [<dir>...] [<file>...]
    # 构建一个基于 framework 的插件
        # 基于提供的包含源代码文件的的路径，或指定的要构建的源代码
    add_framework_plugin(framework-helloworld
        framework-plugins/helloworld
    )

    # 一个最为简单的 framework-hello 插件
    add_framework_plugin(framework-hello-simple
        ${PROJECT_SOURCE_DIR}/src/utils
        framework-plugins/hello-simple
        framework-plugins/hello-simple/view
        framework-plugins/hello-simple/template
        framework-plugins/hello-simple/buildin.qrc)

    # 一个使用引用构建的插件
    # include(path/to/plugin.cmake)

    # # 一个简单的 plantuml 预览插件
    # add_framework_plugin_with_git(https://gitee.com/ndd-community/notepad--plugin.plantuml-preview --branch=cmake-plugins-dev)


    # # 一个简单的 svg 预览插件
    # add_framework_plugin(framework-svg-preview
    #     framework-plugins/svg-preview
    # )
    # if(framework-svg-preview_ENABLE)
    #     find_package(Qt5Svg)
    #     target_link_libraries(framework-svg-preview Qt5::Svg)
    # endif(framework-svg-preview_ENABLE)

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
