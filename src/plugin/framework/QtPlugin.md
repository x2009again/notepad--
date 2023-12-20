# Qt 插件开发

> https://gitee.com/zinface/qt.plugin-dev

- 定义一个插件的接口

    ```cpp
    // 定义一个插件所使用的 IID 名称，被实现的插件也可以使用此宏
    #define IPlugin_IID "com.test.plugin" 

    class IPlugin
    {
    public:
        // 定义一个插件时所用的名称
        virtual QString PluginName() = 0;  // 插件声明函数必须 = 0
    };
    
    Q_DECLARE_INTERFACE(IPlugin, IPlugin_IID)
    ```

- 实现一个插件

    ```cpp
    #include "iplugin.h"

    class PluginDemo : public QObject, IPlugin
    {
        Q_OBJECT
        Q_PLUGIN_METADATA(IID IPlugin_IID)
        Q_INTERFACES(IPlugin)
    public:
        QString PluginName() override;
    };
    ```

    ```cpp
    #include "plugindemo.h"

    QString PluginDemo::PluginName()
    {
        return QString("插件demo");
    }
    ```

    ```
    # 不过设定好像也没啥问题
    add_definitions("${QT_DEFINITIONS} -DQT_PLUGIN")
    # 该库编译将产生一个 `libplugindemo.so`，在 cmake 中应当被指定为 SHARED 共享库目标
    ```

- 使用插件
    
    ```cpp
    IPlugin *plugin;

    // 使用 QPluginLoader，参数 argv[1] 表示为插件路径
    QPluginLoader pluginLoader(argv[1]);
    // 使用插件加载器加载扩展并尝试创建实例
    QObject *instance = pluginLoader.instance();
    if (instance) {
        // 如果实例创建成功，尝试转换对象
        plugin = qobject_cast<IPlugin *>(instance);
        if (plugin) {
            QTextStream(stdout) << QString("> 加载插件并调用函数:\n")
                << "> plugin->PluginName(): " 
                << plugin->PluginName() << "\n";
        }
        pluginLoader.unload();
    }
    ```

    ```shell
    # 将会在 build 目录中，执行以下命令加载插件(因Qt插件机制必须为'绝对路径')
    $ ./plugin-loader `realpath libplugindemo.so`
    plugin->PluginName: 插件demo
    ```