#include "baseplugin.h"
#include "pluginframeworkhelper.h"

#include <QMessageBox>

BasePlugin::BasePlugin(QObject *parent)
    : QObject{parent}
{}

BasePlugin &BasePlugin::instance()
{
    static BasePlugin _plugin;
    return _plugin;
}

QString BasePlugin::PluginName()
{
    return "base-plugin";
}

QString BasePlugin::PluginVersion()
{
    return "v0.1";
}

QString BasePlugin::PluginAuthor()
{
    return "author";
}

QString BasePlugin::PluginComment()
{
    return u8"基本插件 - 由生成器生成";
}

IPluginFramework::MenuType BasePlugin::PluginMenuType()
{
    return MenuType::None;
}

void BasePlugin::PluginTrigger()
{
    QMessageBox::information(nullptr, "tip", "This is default tip message.");
}

void BasePlugin::registerPluginActions(QMenu *rootMenu)
{
    //Actions: Will never enter here.
}

// Plug-in implementation wrapper
NDD_DECLARE_PLUGIN(BasePlugin::instance())
