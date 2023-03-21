#include "ndd_plugin_implement.h"
#include "ui_ndd_plugin_implement.h"

#include <QSplitter>
#include <QThreadPool>
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

    ui->horizontalLayout->addWidget(splitter);
}

NddPluginImplement::~NddPluginImplement()
{
    
}

void NddPluginImplement::on_textEdit_textChanged()
{
    task = new OpenCCTask(this, ui->textEdit->toPlainText());
    connect(task, &OpenCCTask::complete, this, [=](const QString text) {
        this->ui->textBrowser->setText(text);
    });
    QThreadPool::globalInstance()->start(task);
}
