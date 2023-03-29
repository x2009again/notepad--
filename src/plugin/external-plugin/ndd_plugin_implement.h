#ifndef NDD_PLUGIN_IMPLEMENT_H
#define NDD_PLUGIN_IMPLEMENT_H

#include <QMainWindow>

#include "external-plugin/interface/external.h"

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

private:
    Ui::NddPluginImplement *ui;
    QsciScintilla *currentEdit;
    std::function<QsciScintilla* ()> getCurrentEditFunc;

    QList<externalplugin*> plugins;
};

#endif // NDD_PLUGIN_IMPLEMENT_H
