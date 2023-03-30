#ifndef NDD_PLUGIN_IMPLEMENT_H
#define NDD_PLUGIN_IMPLEMENT_H

#include <QMainWindow>

class QsciScintilla;
class NddPluginImplement : public QWidget
{
    Q_OBJECT

public:
    explicit NddPluginImplement(QWidget *parent = nullptr, QsciScintilla *pEdit = nullptr);
    ~NddPluginImplement();

private:
    QsciScintilla *currentEdit;
};

#endif // NDD_PLUGIN_IMPLEMENT_H
