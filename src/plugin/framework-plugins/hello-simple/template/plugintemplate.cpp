#include "plugintemplate.h"
#include "pluginframeworkhelper.h"

#include <QMessageBox>

PluginTemplate::PluginTemplate(QObject *parent)
    : QObject{parent}
{}

PluginTemplate &PluginTemplate::instance()
{
    static PluginTemplate _plugin;
    return _plugin;
}

QString PluginTemplate::PluginName()
{
    return "//Name";
}

QString PluginTemplate::PluginVersion()
{
    return "//Version";
}

QString PluginTemplate::PluginAuthor()
{
    return "//Author";
}

QString PluginTemplate::PluginComment()
{
    return "//Comment";
}

IPluginFramework::MenuType PluginTemplate::PluginMenuType()
{
    return MenuType::None;
}

void PluginTemplate::registerNotepad(QWidget *notepad)
{
    s_notepad = notepad;
}

void PluginTemplate::registerStrFileName(QString str_file_name)
{
    s_str_file_name = str_file_name;
}

void PluginTemplate::PluginTrigger()
{
    //Trigger
}

void PluginTemplate::registerPluginActions(QMenu *rootMenu)
{
    //Actions
}

void PluginTemplate::registerCurrentEditCallback(std::function<QsciScintilla *(QWidget *)> get_cur_edit_callback)
{
    s_get_cur_edit_callback = get_cur_edit_callback;
}

void PluginTemplate::registerPluginCallBack(std::function<bool (QWidget *, int, void *)> plugin_callback)
{
    s_plugin_callback = plugin_callback;
}

// Plug-in implementation wrapper
//NDD_DECLARE_PLUGIN(PluginTemplate::instance())
