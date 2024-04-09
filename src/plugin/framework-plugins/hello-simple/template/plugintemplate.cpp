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

void PluginTemplate::PluginTrigger()
{
    //Trigger
}

void PluginTemplate::registerPluginActions(QMenu *rootMenu)
{
    //Actions
}

// Plug-in implementation wrapper
//NDD_DECLARE_PLUGIN(PluginTemplate::instance())
