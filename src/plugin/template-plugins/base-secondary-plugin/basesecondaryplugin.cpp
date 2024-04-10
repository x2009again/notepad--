#include "basesecondaryplugin.h"
#include "pluginframeworkhelper.h"

#include <QMessageBox>

BaseSecondaryPlugin::BaseSecondaryPlugin(QObject *parent)
    : QObject{parent}
{}

BaseSecondaryPlugin &BaseSecondaryPlugin::instance()
{
    static BaseSecondaryPlugin _plugin;
    return _plugin;
}

QString BaseSecondaryPlugin::PluginName()
{
    return "base-secondary-plugin";
}

QString BaseSecondaryPlugin::PluginVersion()
{
    return "v0.1";
}

QString BaseSecondaryPlugin::PluginAuthor()
{
    return "author";
}

QString BaseSecondaryPlugin::PluginComment()
{
    return "基本二级菜单插件";
}

IPluginFramework::MenuType BaseSecondaryPlugin::PluginMenuType()
{
    return MenuType::SecondaryMenu;
}

void BaseSecondaryPlugin::PluginTrigger()
{
    //Trigger: Will never enter here.
}

void BaseSecondaryPlugin::registerPluginActions(QMenu *rootMenu)
{
    rootMenu->addAction("Default", this, [this](){
        QMessageBox::information(nullptr, "tip", "This is default tip message.");
    });

    connect(rootMenu->addAction(u8"第一项打印"), &QAction::triggered, this, [](){
        // 普通的菜单项
        qDebug() << "action1";
    });

    connect(rootMenu->addAction(u8"第二项打印"), &QAction::triggered, this, [](){
        // 普通的菜单项
        qDebug() << "action2";
    });

    connect(rootMenu->addAction(u8"打印默认编辑器内容(源代码项注意)"), &QAction::triggered, this, [=](){
        // FAQ: 当第一次使用 m_editor 时，使用时可能会引发错误。
        //      此时 m_editor 可能是空的。
        if (m_editor == nullptr) {
            qDebug() << "Warring: m_editor == nullptr!";
            return;
        }
        qDebug() << m_editor->text();
    });

    connect(rootMenu->addAction(u8"设置当前编辑器为默认"), &QAction::triggered, this, [=](){
        // 获取当前编辑器，并作为当前默认编辑器
        m_editor = s_get_cur_edit_callback(s_notepad);
        qDebug() << u8"设置完成";
    });

    connect(rootMenu->addAction(u8"打印当前编辑器内容"), &QAction::triggered, this, [=](){
        // 获取当前编辑器，并直接打印编辑器内容
        auto editor = s_get_cur_edit_callback(s_notepad);
        qDebug() << editor->text();
    });

    connect(rootMenu->addAction(u8"创建新的编辑器"), &QAction::triggered, this, [=](){
        // 创建一个新的编辑器
        QVariant editor = PluginFrameworkHelper::DoNewEdit(s_notepad, s_plugin_callback);
        qDebug() << editor.toString();
    });

    connect(rootMenu->addAction(u8"改变当前编辑器语法 - Html"), &QAction::triggered, this, [=](){
        // 改变语法 Html
        bool re = PluginFrameworkHelper::DoChangeSyntax(s_notepad, s_plugin_callback, SyntaxType::Html);
        qDebug() << "Html" << re;
    });

    connect(rootMenu->addAction(u8"改变当前编辑器语法 - Js"), &QAction::triggered, this, [=](){
        // 改变语法 Js
        bool re = PluginFrameworkHelper::DoChangeSyntax(s_notepad, s_plugin_callback, SyntaxType::Js);
        qDebug() << "Js" << re;
    });

    connect(rootMenu->addAction(u8"改变当前编辑器语法 - Json"), &QAction::triggered, this, [=](){
        // 改变语法 Json
        bool re = PluginFrameworkHelper::DoChangeSyntax(s_notepad, s_plugin_callback, SyntaxType::Json);
        qDebug() << "Json" << re;
    });
}

// Plug-in implementation wrapper
NDD_DECLARE_PLUGIN(BaseSecondaryPlugin::instance())
