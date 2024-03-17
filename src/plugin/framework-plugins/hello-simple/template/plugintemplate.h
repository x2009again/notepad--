#ifndef PLUGINTEMPLATE_H
#define PLUGINTEMPLATE_H

#include <IPluginFramework.h>
#include <QObject>

class PluginTemplate : public QObject, IPluginFramework
{
    Q_OBJECT
    explicit PluginTemplate(QObject *parent = nullptr);
public:
    static PluginTemplate &instance();


    // IPluginFramework interface
public:
    QString PluginName();
    QString PluginVersion();
    QString PluginAuthor();
    QString PluginComment();
    MenuType PluginMenuType();
    void registerNotepad(QWidget *notepad);
    void registerStrFileName(QString str_file_name);
    void PluginTrigger();
    void registerPluginActions(QMenu *rootMenu);
    void registerCurrentEditCallback(std::function<QsciScintilla *(QWidget *)> get_cur_edit_callback);
    void registerPluginCallBack(std::function<bool (QWidget *, int, void *)> plugin_callback);

private:
   QWidget *s_notepad;
   QString s_str_file_name;
   std::function<QsciScintilla*(QWidget*)> s_get_cur_edit_callback;
   std::function<bool(QWidget*, int, void*)> s_plugin_callback;
};

#endif // PLUGINTEMPLATE_H
