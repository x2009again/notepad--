#ifndef PLUGINTEMPLATE_H
#define PLUGINTEMPLATE_H

#include <IPluginFramework.h>
#include <QObject>

class PluginTemplate : public QObject, public IPluginFramework
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
    void PluginTrigger();
    void registerPluginActions(QMenu *rootMenu);

    /** 当前版本 IPluginManager 中已经为此实现 - 如需加载时执行自定义代码请重写 registerNotepad 实现.
        并在其实现内首行调用 IPluginFramework::registerNotepad(notepad);
    */
    // void registerNotepad(QWidget *notepad);
    // void registerStrFileName(QString str_file_name);
    // void registerCurrentEditCallback(std::function<QsciScintilla *(QWidget *)> get_cur_edit_callback);
    // void registerPluginCallBack(std::function<bool (QWidget *, int, void *)> plugin_callback);
};

#endif // PLUGINTEMPLATE_H
