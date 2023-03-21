#ifndef NDD_PLUGIN_IMPLEMENT_H
#define NDD_PLUGIN_IMPLEMENT_H

#include "opencctask.h"

#include <QMainWindow>
#include <QWidget>

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

private slots:
    void on_textEdit_textChanged();

private:
    Ui::NddPluginImplement *ui;
    QsciScintilla *currentEdit;

    OpenCCTask *task;
};

#endif // NDD_PLUGIN_IMPLEMENT_H
