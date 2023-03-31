#ifndef NDD_PLUGIN_IMPLEMENT_H
#define NDD_PLUGIN_IMPLEMENT_H

#include <QMainWindow>
#include <QMenu>
#include <QProcess>

class QsciScintilla;
namespace Ui {
class NddPluginImplement;
};

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
    void speakeSpeck(const QString &text);
    void speakeSpeck(const QStringList &arguments);
    void speakEkho(const QString &text);
    void speakEkho(const QStringList &arguments);

    void speakeSpeckEditrMonitor(const QString &text);
    void speakeSpeckEditrMonitor(const QString &text, int speed);
    void speakeSpeckEditrMonitor(const QString &text, QStringList &arguments, int speed);

    void speakEkhoEditrMonitor(const QString &text);
    void speakEkhoEditrMonitor(const QString &text, int speed);
    void speakEkhoEditrMonitor(const QString &text, QStringList &arguments, int speed);

    void on_pushButton_clicked();

private:
    // 目前看来需要准备一个完整内部状态
    QWidget *currentWidget;
    QsciScintilla *currentEdit;
    std::function<QsciScintilla* ()> getCurrentEditFunc;

    QProcess *process = new QProcess;

private:
    Ui::NddPluginImplement *ttsSettingWidget;

};

#endif // NDD_PLUGIN_IMPLEMENT_H
