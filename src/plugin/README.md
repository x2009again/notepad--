# Notepad--Plugin - 基于源代码树的插件开发说明

> 当前插件开发提供一系列模板

- 有关基于源代码的插件开发描述与 template-plugins 说明

    ```shell
    ...
    template-plugins/
    ├── base-plugin                 # 一个比较原始的插件结构，仅用于参考
    ├── base-widget-plugin          # 一个基于 QWidget 的插件模板
    ├── base-widget-ui-plugin       # 一个基于 QMainWindow 的插件模板
    │   ├── CMakeLists.txt
    │   ├── ndd_plugin_implement.cpp    # 提供一组结构实现
    │   ├── ndd_plugin_implement.h      # 
    │   ├── ndd_plugin_implement.ui     # 提供 ui 文件的插件
    │   └── plugin.cpp                  # 提供 插件信息描述文件
    ├── ... # 添加更多的模板类型
    └── plugin.cpp # 插件宏变化文件，以记录插件宏与接口的历史性变化
    ...
    ```

- 如何开发一个基于源代码树的插件

    ```cmake
    # 复制一份你所想继承的模板，例如: base-widget-ui-plugin

    # 基于 base-widget-ui-plugin 开发一个新的插件 version-update
        # 1. 将文件夹重命名为插件名称
        # 2. 替换内部 CMakeLists.txt 文件的 base-widget-ui-plugin 字符串
        # 3. 在 plugin/CMakeLists.txt 中添加
        add_subdirectory(version-update)

        # 4. 在 version-update/plugin.cpp 中
        NOTEPAD_PLUGIN_METADATA_IDENTIFY("检查更新", "v0.1", "zinface", 
            "Notepad-- 版本检查", "");

        NOTEPAD_PLUGIN_METADATA_IMPLEMENT(NddPluginImplement, false);
        # NddPluginImplement 是来源于 ndd_plugin_implement.h 的类
        # false 表示这个窗口不需要显示
    ```

- 有关 plugin.cpp 宏变化文件

    ```shell
    # 1. 记录了在 cmake-dev 以前的插件宏实现方式
    # 2. 记录了在 cmake-dev 以来的插件宏实现方式
    # 3. 记录了在 cmake-plugins-dev 以来的插件宏实现方式
    # ...
    ```


- 写在后面
    
    ```shell
    # 此系列插件继承原有的模式，暂不支持在源代码之外构建
    # 
    # 注意：
        # 由于插件系列统一以 ndd_plugin_implement 为文件名
        # 在编写 ui 文件时，请提前打开此 ui 文件相关的 cpp 文件
        # 或关闭其它不相关的插件源文件，以避免生成槽函数时出现在其它位置
    ```