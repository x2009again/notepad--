#ifndef PLUGINFRAMEWORKHELPER_H
#define PLUGINFRAMEWORKHELPER_H

#include "IPluginFramework.h"

#include <QObject>
#include <QWidget>
#include <qsciscintilla.h>

class PluginFrameworkHelper
{
public:
    static QVariant DoNewEdit(QWidget *notepad, std::function<bool(QWidget*, int, void*)> plugin_callback);
    static bool DoChangeSyntax(QWidget *notepad, std::function<bool(QWidget*, int, void*)> plugin_callback, IPluginFramework::SyntaxType syntax);
};

#endif // PLUGINFRAMEWORKHELPER_H
