#ifndef HELLOWORLD_H
#define HELLOWORLD_H

#include <QObject>
#include "framework/IPluginFramework.h"

class HelloWorld : public QObject, IPluginFramework
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID IPluginFramework_IID)
    Q_INTERFACES(IPluginFramework)
public:
    explicit HelloWorld(QObject *parent = nullptr);
    ~HelloWorld();

    static HelloWorld &instance();

    // IPluginFramework interface
public:
    QString PluginName();
    QString PluginVersion();
    QString PluginAuthor();
    QString PluginComment();
    MenuType PluginMenuType();

    // IPluginFramework interface
public:
    void registerNotepad(QWidget *notepad);

    // IPluginFramework interface
public:
    void registerCurrentEditCallback(std::function<QsciScintilla *(QWidget *)> get_cur_edit_callback);

    // IPluginFramework interface
public:
    void PluginTrigger();

    // IPluginFramework interface
public:
//    void registerTrigger(QAction *action);
    void registerPluginActions(QMenu *rootMenu);

private:
    QWidget *s_notepad;
    std::function<QsciScintilla *(QWidget *)> s_get_cur_edit_callback;

};

#endif // HELLOWORLD_H
