
option(USE_NOTEPAD_PLUGIN "Notepad-- 插件构建配置" ON)

if(TRUE)
    add_subdirectory(helloworld-next)
    
    # 基于源代码构建的系列插件模板
    add_subdirectory(template-plugins/base-plugin)
    add_subdirectory(template-plugins/base-widget-plugin)
    add_subdirectory(template-plugins/base-widget-ui-plugin)

    # 一个简单的插件示例，但依赖于外部的 opencc 项目(基于 git)
    add_subdirectory(opencc-demo-plugin)
    # 一个简单的版本更新检查的插件
    add_subdirectory(versionUpdate)
endif(TRUE)
