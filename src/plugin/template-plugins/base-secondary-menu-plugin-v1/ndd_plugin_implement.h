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

    void setMenuActions(QMenu *menu);
    void setCurrentEditFunc(std::function<QsciScintilla* ()> func) {
        getCurrentEditFunc = func;
    }

private:
    QsciScintilla *currentEdit;
    std::function<QsciScintilla* ()> getCurrentEditFunc;
};

#endif // NDD_PLUGIN_IMPLEMENT_H
