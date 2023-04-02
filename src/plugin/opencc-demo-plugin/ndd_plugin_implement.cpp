#include "ndd_plugin_implement.h"
#include "ui_ndd_plugin_implement.h"

#include <qsciscintilla.h>
#include <QDebug>
#include <QDockWidget>
#include <QSplitter>
#include <QThreadPool>
#include <qpushbutton.h>
#include <qsciscintilla.h>

NddPluginImplement::NddPluginImplement(QWidget *parent, QsciScintilla *pEdit) : QMainWindow(parent)
    , currentWidget(parent)
    , currentEdit(pEdit)
    , ui(new Ui::NddPluginImplement)
    , m_dockWidget(new QDockWidget)
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
        task->setCn2Tn(ui->radioCnTn->isChecked());
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

void NddPluginImplement::setMenuActions(QMenu *menu)
{
  connect(menu->addAction("Show Ui"), &QAction::triggered, this, [=](){
//      on_pushButton_clicked();
      this->setParent(nullptr);
      this->show();
      this->resize(QSize(570,400));
      this->ui->tabWidget->setCurrentWidget(ui->tab);
  });

  connect(menu->addAction("Show Sider"), &QAction::triggered, this, [=](){
      auto mainWindow = dynamic_cast<QMainWindow*>(currentWidget);
      if(!mainWindow) {
          qDebug() << "None";
          return;
      }

      m_dockWidget->setMinimumSize(100, 200);
      mainWindow->addDockWidget(Qt::RightDockWidgetArea, m_dockWidget);

      if (!m_dockWidget->isVisible())
          m_dockWidget->show();

      m_dockWidget->setWidget(this);

      ui->tabWidget->setCurrentWidget(ui->tab_2);
  });
}

void NddPluginImplement::convertCn2TnInEditBrowser(bool cn2tn)
{
    task = new OpenCCTask(this, ui->textEdit->toPlainText());
    task->setCn2Tn(cn2tn);
    connect(task, &OpenCCTask::complete, this, [=](const QString text) {
        this->ui->textBrowser->setText(text);
    });
#if WIN32
    task->run();
    task->deleteLater();
#else
    QThreadPool::globalInstance()->start(task);
#endif
}

void NddPluginImplement::convertCn2TnInCurrentEdit(bool cn2tn)
{
    task = new OpenCCTask(this, getCurrentEditFunc()->text());
    task->setCn2Tn(cn2tn);
    connect(task, &OpenCCTask::complete, this, [=](const QString text) {
        getCurrentEditFunc()->setText(text);
    });
#if WIN32
    task->run();
    task->deleteLater();
#else
    QThreadPool::globalInstance()->start(task);
#endif
}

void NddPluginImplement::on_textEdit_textChanged()
{
#if WIN32
    // 不采用实时任务
#else
    convertCn2TnInEditBrowser(this->ui->radioCnTn->isChecked());
#endif
}

void NddPluginImplement::on_pushButtonCn2Tn_clicked(bool checked)
{
    convertCn2TnInCurrentEdit(true);
}

void NddPluginImplement::on_pushButtonTn2Cn_clicked(bool checked)
{
    convertCn2TnInCurrentEdit(false);
}
