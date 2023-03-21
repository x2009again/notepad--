
option(USE_NOTEPAD_PLUGIN "Notepad-- 插件构建配置" ON)

if(TRUE)
    add_subdirectory(helloworld-next)
    
    # 基于源代码构建的系列插件模板
    add_subdirectory(template-plugins/base-plugin)
    add_subdirectory(template-plugins/base-widget-plugin)
    add_subdirectory(template-plugins/base-widget-ui-plugin)
endif(TRUE)
