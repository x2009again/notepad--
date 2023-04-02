#include "ndd_plugin_implement.h"
#include "ui_ndd_plugin_implement.h"

#include <qsciscintilla.h>
#include <QDebug>
#include <QDockWidget>
#include <QHostInfo>

NddPluginImplement::NddPluginImplement(QWidget *parent, QsciScintilla *pEdit) : QMainWindow(parent)
    , currentWidget(parent)
    , currentEdit(pEdit)
    , ui(new Ui::NddPluginImplement)
    , m_dockWidget(new QDockWidget)
{
    ui->setupUi(this);
    ui->computer->setText(QHostInfo::localHostName());
    ui->computer->setReadOnly(true);
}

NddPluginImplement::~NddPluginImplement()
{
    
}

void NddPluginImplement::setMenuActions(QMenu *menu)
{
  QAction *action1 = menu->addAction(u8"第一项打印");
  connect(action1, &QAction::triggered, this, [](){
    // 普通的菜单项
    qDebug() << "action1";
  });

  connect(menu->addAction("Show Ui"), &QAction::triggered, this, [=](){
      on_pushButton_clicked();
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
  });
}

void NddPluginImplement::on_pushButton_clicked()
{
  this->setParent(nullptr);
  this->resize(QSize(480,340));
  this->show();
}
