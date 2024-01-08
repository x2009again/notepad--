#ifndef HELLO_H
#define HELLO_H

#include <IPluginFramework.h>
#include <QObject>

class Hello : public QObject, IPluginFramework
{
    Q_OBJECT
public:
    explicit Hello(QObject *parent = nullptr);
    static Hello &instance();

signals:


    // IPluginFramework interface
public:
    QString PluginName() override;
    QString PluginVersion() override;
    QString PluginAuthor() override;
    QString PluginComment() override;
    MenuType PluginMenuType() override;
    void registerNotepad(QWidget *notepad) override;
    void registerStrFileName(QString str_file_name) override;
    void PluginTrigger() override;
    void registerPluginActions(QMenu *rootMenu) override;
    void registerCurrentEditCallback(std::function<QsciScintilla *(QWidget *)> get_cur_edit_callback) override;
    void registerPluginCallBack(std::function<bool (QWidget *, int, void *)> plugin_callback) override;


private:
        QWidget *s_notepad;
        QString s_str_file_name;
        std::function<QsciScintilla*(QWidget*)> s_get_cur_edit_callback;
        std::function<bool(QWidget*, int, void*)> s_plugin_callback;
};

#endif // HELLO_H

