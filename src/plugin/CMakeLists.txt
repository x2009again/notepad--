
option(USE_NOTEPAD_PLUGIN "构建 Notepad-- 内部插件" ON)
option(USE_NOTEPAD_PLUGIN_BASE "构建 Notepad-- 内部的插件基础模板" OFF)


if(USE_NOTEPAD_PLUGIN)
    add_subdirectory(helloworld-next)
    
    if(USE_NOTEPAD_PLUGIN_BASE)
        # 基于源代码构建的系列插件模板
        add_subdirectory(template-plugins/base-plugin)
        add_subdirectory(template-plugins/base-widget-plugin)
        add_subdirectory(template-plugins/base-widget-ui-plugin)
        add_subdirectory(template-plugins/base-secondary-menu-plugin)
        add_subdirectory(template-plugins/base-secondary-menu-plugin-v1)
    endif(USE_NOTEPAD_PLUGIN_BASE)

    # 一个简单的插件示例，但依赖于外部的 opencc 项目(基于 git)
    add_subdirectory(opencc-demo-plugin)
    # 一个简单的版本更新检查的插件
    add_subdirectory(versionUpdate)
    # 一个简单的可二次扩展的插件
    add_subdirectory(external-plugin)

endif(USE_NOTEPAD_PLUGIN)
