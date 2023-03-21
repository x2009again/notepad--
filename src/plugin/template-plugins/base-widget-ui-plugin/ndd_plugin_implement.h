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

private:
    Ui::NddPluginImplement *ui;
    QsciScintilla *currentEdit;
};

#endif // NDD_PLUGIN_IMPLEMENT_H
