#ifndef NDD_PLUGIN_IMPLEMENT_H
#define NDD_PLUGIN_IMPLEMENT_H

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

private slots:
    void on_pushButton_clicked();

private:
    // 目前看来需要准备一个完整内部状态
    QWidget *currentWidget;
    QsciScintilla *currentEdit;
    std::function<QsciScintilla* ()> getCurrentEditFunc;

private:
    Ui::NddPluginImplement *ui;
    QDockWidget *m_dockWidget;
};

#endif // NDD_PLUGIN_IMPLEMENT_H
