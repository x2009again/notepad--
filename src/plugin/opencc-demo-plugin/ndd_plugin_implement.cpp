#include "ndd_plugin_implement.h"
#include "ui_ndd_plugin_implement.h"

#include <QSplitter>
#include <QThreadPool>
#include <qpushbutton.h>
#include <qsciscintilla.h>

NddPluginImplement::NddPluginImplement(QWidget *parent, QsciScintilla *pEdit) : QMainWindow (parent)
  , ui(new Ui::NddPluginImplement)
  , currentEdit(pEdit)
{
    ui->setupUi(this);

    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(ui->textEdit);
    splitter->addWidget(ui->textBrowser);
    splitter->setChildrenCollapsible(false);
    splitter->setHandleWidth(5);

    ui->horizontalLayout_2->addWidget(splitter);

#if WIN32
    connect(ui->pushButton, &QPushButton::clicked, this, [&](){
        task = new OpenCCTask(this, ui->textEdit->toPlainText());
        connect(task, &OpenCCTask::complete, this, [=](const QString text) {
            this->ui->textBrowser->setText(text);
        });
        task->run();
        task->deleteLater();
    });
#else
    ui->pushButton->hide();
#endif
}

NddPluginImplement::~NddPluginImplement()
{
    
}

void NddPluginImplement::on_textEdit_textChanged()
{
#if WIN32
#else
    task = new OpenCCTask(this, ui->textEdit->toPlainText());
    connect(task, &OpenCCTask::complete, this, [=](const QString text) {
        this->ui->textBrowser->setText(text);
    });
    QThreadPool::globalInstance()->start(task);
#endif
}
