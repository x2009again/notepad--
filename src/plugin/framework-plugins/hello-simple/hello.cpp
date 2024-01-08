#include "hello.h"
#include "pluginframeworkhelper.h"

#include <qmessagebox.h>

Hello::Hello(QObject *parent)
    : QObject{parent}
{

}

Hello &Hello::instance()
{
    static Hello hello;
    return hello;
}

QString Hello::PluginName()
{
    return "Hello";
}

QString Hello::PluginVersion()
{
    return "0.1";
}

QString Hello::PluginAuthor()
{
    return "zinface";
}

QString Hello::PluginComment()
{
    return "hello 插件";
}

IPluginFramework::MenuType Hello::PluginMenuType()
{
    return IPluginFramework::MenuType::SecondaryMenu;
}

void Hello::registerNotepad(QWidget *notepad)
{
    s_notepad = notepad;
}

void Hello::registerStrFileName(QString str_file_name)
{
    s_str_file_name = str_file_name;
}

void Hello::PluginTrigger()
{
    QMessageBox::information(nullptr, "Tip", "This is Hello Plugin");
}

void Hello::registerPluginActions(QMenu *rootMenu)
{
    rootMenu->addAction("First Plugin Action", this, [](){
        QMessageBox::information(nullptr, "Tip", "This is Hello Plugin(First Action)");
    });
    rootMenu->addAction("Second Plugin Action", this, [](){
        QMessageBox::information(nullptr, "Tip", "This is Hello Plugin(Second Action)");
    });
    rootMenu->addAction("Three Plugin Action", this, [this](){


//        QsciScintilla *edit = s_get_cur_edit_callback(s_notepad);

//        QMessageBox::information(nullptr, "Tip", edit->text());

        QVariant editName = PluginFrameworkHelper::DoNewEdit(s_notepad, s_plugin_callback);
        QMessageBox::information(nullptr, "New Edit", editName.toString());
    });
}

void Hello::registerCurrentEditCallback(std::function<QsciScintilla *(QWidget *)> get_cur_edit_callback)
{
    s_get_cur_edit_callback = get_cur_edit_callback;
}

void Hello::registerPluginCallBack(std::function<bool (QWidget *, int, void *)> plugin_callback)
{
    s_plugin_callback = plugin_callback;
}


NDD_DECLARE_PLUGIN(Hello::instance())