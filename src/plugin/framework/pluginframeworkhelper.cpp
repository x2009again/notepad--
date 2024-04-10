#include "pluginframeworkhelper.h"

QVariant PluginFrameworkHelper::DoNewEdit(QWidget *notepad, PluginWantDo plugin_callback)
{
    QVariant var;
    plugin_callback(notepad, IPluginFramework::NewEdit, (void*)&var);
    return var;
}

bool PluginFrameworkHelper::DoChangeSyntax(QWidget *notepad, PluginWantDo plugin_callback, IPluginFramework::SyntaxType syntax)
{
    int v = syntax;
    return plugin_callback(notepad, IPluginFramework::ChangeSyntax, (void*)&v);
}
