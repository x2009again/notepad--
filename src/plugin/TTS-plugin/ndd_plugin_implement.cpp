#include "ndd_plugin_implement.h"
#include "ui_ndd_plugin_implement.h"

#include <qsciscintilla.h>
#include <QDebug>
#include <QProcess>

#include <QDir>
#include <QDockWidget>
#include <QFile>
#include <QFileInfo>
#include <QThread>
NddPluginImplement::NddPluginImplement(QWidget *parent, QsciScintilla *pEdit) : QMainWindow(parent)
    , currentWidget(parent)
    , currentEdit(pEdit)
    , ttsSettingWidget(new Ui::NddPluginImplement)
{
    ttsSettingWidget->setupUi(this);
}

NddPluginImplement::~NddPluginImplement()
{
    
}

void NddPluginImplement::setMenuActions(QMenu *menu)
{

    connect(menu->addAction(u8"TTS Settings"), &QAction::triggered, this, [=](){
        this->setParent(nullptr);
        this->show();
    });

    connect(menu->addAction(u8"TTS Settings(Dock)"), &QAction::triggered, this, [=](){
        auto mainWindow = dynamic_cast<QMainWindow*>(currentWidget);
        if(!mainWindow) {
            qDebug() << "None";
            return;
        }

        QDockWidget *m_dockWidget = new QDockWidget(mainWindow);
        m_dockWidget->setMinimumSize(100, 200);
        mainWindow->addDockWidget(Qt::RightDockWidgetArea, m_dockWidget);

        if (!m_dockWidget->isVisible())
            m_dockWidget->show();

        m_dockWidget->setWidget(this);
    });


    // 
    connect(menu->addAction(u8"TTS Start"), &QAction::triggered, this, [&](){
        // 获取当前编辑器，并直接打印编辑器内容
        QsciScintilla *edit = getCurrentEditFunc();

        QStringList arguments = QStringList() << "-v" << "zh" << edit->selectedText();;
        speakeSpeck(arguments);
    });
        
    connect(menu->addAction(u8"TTS Stop"), &QAction::triggered, this, [&](){
        if (process->state() != QProcess::NotRunning) {
            process->kill();
        }
    });

    connect(menu->addAction(u8"阅读选中内容 eSpeak (未指定)"), &QAction::triggered, this, [&](){
        QStringList arguments = QStringList() << getCurrentEditFunc()->selectedText();

        speakeSpeck(arguments);
    });

    connect(menu->addAction(u8"阅读选中内容 eSpeak (普通话)"), &QAction::triggered, this, [&](){
        QStringList arguments = QStringList() << "-v" << "zh" << getCurrentEditFunc()->selectedText();
        
        speakeSpeck(arguments);
    });

    connect(menu->addAction(u8"阅读选中内容 Ekho (未指定)"), &QAction::triggered, this, [&](){
        QStringList arguments = QStringList() << getCurrentEditFunc()->selectedText();

        speakeSpeck(arguments);
    });

    connect(menu->addAction(u8"阅读选中内容 Ekho (普通话)"), &QAction::triggered, this, [&](){
        QStringList arguments = QStringList() << "-v" << "Mandarin" << getCurrentEditFunc()->selectedText();
        
        speakEkho(arguments);
    });

    connect(menu->addAction(u8"阅读选中内容 Ekho (广东话-粤语)"), &QAction::triggered, this, [&](){
        QStringList arguments = QStringList() << "-v" << "Cantonese" << getCurrentEditFunc()->selectedText();
        
        speakEkho(arguments);
    });

    connect(menu->addAction(u8"阅读选中内容 Ekho (台山话)"), &QAction::triggered, this, [&](){
        QStringList arguments = QStringList() << "-v" << "Toisanese" << getCurrentEditFunc()->selectedText();
        
        speakEkho(arguments);
    });

    connect(menu->addAction(u8"阅读选中内容 Ekho (客家话)"), &QAction::triggered, this, [&](){
        QStringList arguments = QStringList() << "-v" << "Hakka" << getCurrentEditFunc()->selectedText();
        
        speakEkho(arguments);
    });

    connect(menu->addAction(u8"阅读选中内容 Ekho (藏语)"), &QAction::triggered, this, [&](){
        QStringList arguments = QStringList() << "-v" << "Tibetan" << getCurrentEditFunc()->selectedText();
        
        speakEkho(arguments);
    });

    connect(menu->addAction(u8"阅读选中内容 Ekho (南京话)"), &QAction::triggered, this, [&](){
        QStringList arguments = QStringList() << "-v" << "Ngangien" << getCurrentEditFunc()->selectedText();
        
        speakEkho(arguments);
    });

    connect(menu->addAction(u8"阅读选中内容 Ekho (韩语)"), &QAction::triggered, this, [&](){
        QStringList arguments = QStringList() << "-v" << "Hangul" << getCurrentEditFunc()->selectedText();
        
        speakEkho(arguments);
    });

    connect(menu->addAction(u8"监听变化"), &QAction::triggered, this, [&](){
        // QThread *thread = new QThread;

        currentEdit = getCurrentEditFunc();

        // 光标移动声
        connect(currentEdit, &QsciScintilla::selectionChanged, this, [=](){
            speakeSpeckEditrMonitor("k", 450);
        });

        // 输入声 - change event
        connect(currentEdit, &QsciScintilla::textChanged, this, [=](){
            QStringList arguments = QStringList() << "-v" << "zh";
            speakeSpeckEditrMonitor(u8"滴", arguments, 450);
        });
    });

    // Cursor
    connect(menu->addAction(u8"光标行号变化监听"), &QAction::triggered, this, [&](){
        currentEdit = getCurrentEditFunc();
        connect(currentEdit, &QsciScintilla::cursorPosChange, this, [=](int x, int y){
            QStringList arguments = QStringList() << "-v" << "zh";
            speakEkhoEditrMonitor(QString(u8"现在是第 %1").arg(x+1), arguments, 175);
        });
    });
}

// ...

void NddPluginImplement::speakeSpeckEditrMonitor(const QString &text) {

    QStringList arguments = QStringList() << "-v" << "en" << "-s" << "450" << text;

    speakeSpeck(arguments);
}

void NddPluginImplement::speakeSpeckEditrMonitor(const QString &text, int speed) {
    QStringList arguments = QStringList() << "-v" << "en" << "-s" << QString::number(speed) << text;

    speakeSpeck(arguments);
}

void NddPluginImplement::speakeSpeckEditrMonitor(const QString &text, QStringList &arguments, int speed)
{
    arguments << "-s" << QString::number(speed) << text;

    speakeSpeck(arguments);
}

void NddPluginImplement::speakEkhoEditrMonitor(const QString &text)
{
    QStringList arguments = QStringList() << text;

    speakEkho(arguments);
}

void NddPluginImplement::speakEkhoEditrMonitor(const QString &text, int speed)
{
    QStringList arguments = QStringList() << "-s" << QString::number(speed) << text;

    speakeSpeck(arguments);
}

void NddPluginImplement::speakEkhoEditrMonitor(const QString &text, QStringList &arguments, int speed)
{
    arguments << "-s" << QString::number(speed) << text;

    speakeSpeck(arguments);
}

void NddPluginImplement::speakeSpeck(const QString &text) {
    QStringList arguments = QStringList() << "-v" << "zh" << "-s" << "600";

    arguments << "滴";

    speakeSpeck(arguments);
}

void NddPluginImplement::speakeSpeck(const QStringList &arguments)
{
    if (process->state() == QProcess::Running) {
        return;
    }

    QString eSpeakEngine = ttsSettingWidget->espeakEngine->text();
    if (!QFile::exists(eSpeakEngine)) {
        return;
    }
    QDir eSpeakEngineDir = QFileInfo(eSpeakEngine).absoluteDir();

    if (process->state() == QProcess::NotRunning) {
        process->start(eSpeakEngine, arguments);
        if (!process->waitForStarted(1000)) {
            qDebug() << "Failed to start espeak process";
            return;
        }
    }
}

void NddPluginImplement::speakEkho(const QString &text) {
    QStringList arguments = QStringList() << getCurrentEditFunc()->text();

    speakEkho(arguments);
}

void NddPluginImplement::speakEkho(const QStringList &arguments)
{
    if (process->state() == QProcess::Running) {
        process->kill();
    }

    QString ekhoEngine = ttsSettingWidget->ekhoEngine->text();
    if (!QFile::exists(ekhoEngine)) {
        return;
    }
    QDir ekhoEngineDir = QFileInfo(ekhoEngine).absoluteDir();

    process->setWorkingDirectory(ekhoEngineDir.absolutePath());
    if (process->state() == QProcess::NotRunning) {
        process->start(ekhoEngine, arguments);
        if (!process->waitForStarted(1000)) {
            qDebug() << "Failed to start espeak process";
            return;
        }
    }
}

void NddPluginImplement::on_pushButton_clicked()
{

}

