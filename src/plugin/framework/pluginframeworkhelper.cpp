#include "pluginframeworkhelper.h"

QVariant PluginFrameworkHelper::DoNewEdit(QWidget *notepad, std::function<bool (QWidget *, int, void *)> plugin_callback)
{
    QVariant var;
    plugin_callback(notepad, IPluginFramework::NewEdit, (void*)&var);
    return var;
}

bool PluginFrameworkHelper::DoChangeSyntax(QWidget *notepad, std::function<bool (QWidget *, int, void *)> plugin_callback, IPluginFramework::SyntaxType syntax)
{
    int v = syntax;
    return plugin_callback(notepad, IPluginFramework::ChangeSyntax, (void*)&v);
}
