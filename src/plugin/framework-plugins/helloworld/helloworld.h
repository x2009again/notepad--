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

    void PluginTrigger();
    void registerNotepad(QWidget *notepad);
    void registerStrFileName(QString str_file_name) override;
    void registerCurrentEditCallback(std::function<QsciScintilla *(QWidget *)> get_cur_edit_callback);
    void registerPluginCallBack(std::function<bool (QWidget *, int, void *)> plugin_callback) override;
    void registerPluginActions(QMenu *rootMenu);

private:
    QWidget *s_notepad;
    QString s_str_file_name;
    std::function<QsciScintilla *(QWidget *)> s_get_cur_edit_callback;
    std::function<bool(QWidget*, int, void*)> s_plugin_callback;
};

#endif // HELLOWORLD_H
