#include "helloworld.h"
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
    return None;
//    return SecondaryMenu;
}

void HelloWorld::registerNotepad(QWidget *notepad)
{
    s_notepad = notepad;
}

void HelloWorld::registerCurrentEditCallback(std::function<QsciScintilla *(QWidget *)> get_cur_edit_callback)
{
    s_get_cur_edit_callback = get_cur_edit_callback;
}

void HelloWorld::PluginTrigger()
{
    qDebug() << "Triggered";
    QMessageBox::information(nullptr, "提示信息", "您已 trigger 触发此插件");

    int length = s_get_cur_edit_callback(s_notepad)->text().count();

    QMessageBox::information(nullptr, "提示信息", QString("您当前编辑器有 %1 个字符").arg(length));
}

void HelloWorld::registerPluginActions(QMenu *rootMenu)
{
    QMessageBox::information(nullptr, "提示信息", "您的 MenuType 类型为 1 需要注册 QAction 集合");
    rootMenu->addAction("第一个 QAction", [](){
        QMessageBox::information(nullptr, "提示信息", "您已 trigger 触发插件的第一个 QAction");
    });
}
