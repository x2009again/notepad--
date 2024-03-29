#include "helloworld.h"
#include "pluginframeworkhelper.h"
#include "qdebug.h"

#include <QAction>
#include <QMessageBox>

HelloWorld::HelloWorld(QObject *parent)
    : QObject{parent}
{

}

HelloWorld::~HelloWorld()
{

}

HelloWorld &HelloWorld::instance()
{
    static HelloWorld helloworld;
    return helloworld;
}

QString HelloWorld::PluginName()
{
    return "Framework HelloWorld";
}

QString HelloWorld::PluginVersion()
{
    return "v0.1";
}

QString HelloWorld::PluginAuthor()
{
    return "zinface";
}

QString HelloWorld::PluginComment()
{
    return "这一个基于IPluginFramework 实现的插件";
}

IPluginFramework::MenuType HelloWorld::PluginMenuType()
{
//    return None;
    return SecondaryMenu;
}

void HelloWorld::PluginTrigger()
{
    qDebug() << "Triggered";
    QMessageBox::information(nullptr, "提示信息", "您已 trigger 触发此插件");

    int length = s_get_cur_edit_callback(s_notepad)->text().count();

    QMessageBox::information(nullptr, "提示信息", QString("您当前编辑器有 %1 个字符").arg(length));
}


void HelloWorld::registerNotepad(QWidget *notepad)
{
    s_notepad = notepad;
}

void HelloWorld::registerStrFileName(QString str_file_name)
{
    s_str_file_name = str_file_name;
}

void HelloWorld::registerCurrentEditCallback(std::function<QsciScintilla *(QWidget *)> get_cur_edit_callback)
{
    s_get_cur_edit_callback = get_cur_edit_callback;
}

void HelloWorld::registerPluginCallBack(std::function<bool (QWidget *, int, void *)> plugin_callback)
{
    s_plugin_callback = plugin_callback;
}

void HelloWorld::registerPluginActions(QMenu *rootMenu)
{
    // QMessageBox::information(nullptr, "提示信息", "您的 MenuType 类型为 1 需要注册 QAction 集合");
    rootMenu->addAction("第一个 QAction", [](){
        QMessageBox::information(nullptr, "提示信息", "您已 trigger 触发插件的第一个 QAction");
    });

    rootMenu->addAction("创建一个编辑器", [this](){
        QVariant editName = PluginFrameworkHelper::DoNewEdit(s_notepad, s_plugin_callback);
        QMessageBox::information(nullptr, "提示信息", QString("您已 创建了一个编辑器: %1").arg(editName.toString()));
    });

    rootMenu->addAction("改变当前编辑器语法为 Json", [this](){
        bool ret = PluginFrameworkHelper::DoChangeSyntax(s_notepad, s_plugin_callback, IPluginFramework::Json);
        if (ret) {
            QMessageBox::information(nullptr, "提示信息", QString("您已 改变了当前编辑器语法为 Json"));
        } else {
            QMessageBox::information(nullptr, "提示信息", QString("您未能成功改变当前编辑器语法为 Json"));
        }
    });

    rootMenu->addAction("显示当前插件文件路径", [this](){
        QMessageBox::information(nullptr, "提示信息", QString("您当前插件的路径为:\n%1").arg(s_str_file_name));
    });
}
