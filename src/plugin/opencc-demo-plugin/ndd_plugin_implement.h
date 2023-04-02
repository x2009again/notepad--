#ifndef NDD_PLUGIN_IMPLEMENT_H
#define NDD_PLUGIN_IMPLEMENT_H

#include "opencctask.h"
#include <QMainWindow>

class QsciScintilla;
namespace Ui {
    class NddPluginImplement;
}
class NddPluginImplement : public QMainWindow
{
    Q_OBJECT

public:
    explicit NddPluginImplement(QWidget *parent = nullptr, QsciScintilla *pEdit = nullptr);
    ~NddPluginImplement();

    void setMenuActions(QMenu *menu);
    void setCurrentEditFunc(std::function<QsciScintilla* ()> func) {
        getCurrentEditFunc = func;
    }

    void convertCn2TnInEditBrowser(bool cn2tn);
    void convertCn2TnInCurrentEdit(bool cn2tn);

private slots:
    void on_textEdit_textChanged();

    void on_pushButtonCn2Tn_clicked(bool checked);

    void on_pushButtonTn2Cn_clicked(bool checked);

private:
    // 目前看来需要准备一个完整内部状态
    QWidget *currentWidget;
    QsciScintilla *currentEdit;
    std::function<QsciScintilla* ()> getCurrentEditFunc;

private:
    Ui::NddPluginImplement *ui;
    QDockWidget *m_dockWidget;

    OpenCCTask *task;
};

#endif // NDD_PLUGIN_IMPLEMENT_H
