#ifndef NDD_PLUGIN_IMPLEMENT_H
#define NDD_PLUGIN_IMPLEMENT_H

#include <QWidget>
#include <QMenu>

class QsciScintilla;
class NddPluginImplement : public QWidget
{
    Q_OBJECT

public:
    explicit NddPluginImplement(QWidget *parent = nullptr, QsciScintilla *pEdit = nullptr);
    ~NddPluginImplement();

    void setMenuActions(QMenu *menu);
    void setCurrentEditFunc(std::function<QsciScintilla* ()> func) {
        getCurrentEditFunc = func;
    }

private:
    // 目前看来需要准备一个完整内部状态
    QWidget *currentWidget;
    QsciScintilla *currentEdit;
    std::function<QsciScintilla* ()> getCurrentEditFunc;
};

#endif // NDD_PLUGIN_IMPLEMENT_H
