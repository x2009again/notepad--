#include "ndd_plugin_implement.h"
#include "ui_ndd_plugin_implement.h"

#include <qsciscintilla.h>
#include <QDir>
#include <QDebug>
#include <QLibrary>
#include <QFunctionPointer>

#include "external-plugin/interface/external.h"
#include <QDesktopWidget>
#include <QDesktopServices>
#include <QUrl>

NddPluginImplement::NddPluginImplement(QWidget *parent, QsciScintilla *pEdit) : QMainWindow (parent)
  , ui(new Ui::NddPluginImplement)
  , currentEdit(pEdit)
{
    ui->setupUi(this);


    qDebug() << qApp->applicationPid();
    qDebug() << qApp->applicationName();
    qDebug() << qApp->applicationVersion();
    qDebug() << qApp->applicationDirPath();
    qDebug() << qApp->applicationFilePath();
    QDir dir("plugin/external-plugins");
    
    QStringList filters;

  #if defined (Q_OS_WIN)
    filters << "*.dll";
  #elif defined(Q_OS_UNIX)
    filters << "*.so";
  #endif // Q_OS_WIN
    
    dir.setNameFilters(filters);
    QFileInfoList list = dir.entryInfoList(QDir::Files);
    foreach(QFileInfo info, list) {
      qDebug() << info.fileName();  // false
      qDebug() << info.filePath();  // true
      qDebug() << info.absoluteFilePath(); // true
      
      QLibrary lib(info.filePath());
      lib.load();
      
      qDebug() << lib.isLoaded();

      externalplugin *external_plugin;
      externalfunction func;
      // externalmetadata metadata;

      // func = (externalfunction)lib.resolve("process");
      // metadata = (externalfunction)lib.resolve("metadata");
      external_plugin = (externalplugin*)lib.resolve("plugin");
      if (external_plugin != nullptr) {
        qDebug() << "Process!";
        plugins.append(external_plugin);
      }

      externalplugin *external_plugins;
      external_plugins = (externalplugin*)lib.resolve("plugins");
      if (external_plugins != nullptr) {
        external_plugin = external_plugins;

        while (external_plugin->type != ActionUnknow) {
          qDebug() << "Action:" << QString::fromLocal8Bit(external_plugin->meta);
          plugins.append(external_plugin);
          external_plugin++;
        }
      }
    }
}

NddPluginImplement::~NddPluginImplement()
{
    delete ui;
}

void NddPluginImplement::setMenuActions(QMenu *menu)
{
  // 此处开始配置 Menu 内容
  for (int i = 0; i < plugins.size(); i++)
  {
    int length = strlen(plugins[i]->meta);
    
    QString name = QString::fromLocal8Bit(plugins[i]->meta, length);
    QAction *action = menu->addAction(name);

    action->connect(action, &QAction::triggered, [=](){
      qDebug() << "func!";

      // 作为二级菜单引擎，以当前编辑器为主进行扩展可用的设计
      currentEdit = getCurrentEditFunc();
      if (currentEdit == nullptr) {
        qDebug() << "Error: no current edit";
        return;
      }

      // 准备当前编辑器的内容，转为可用的标准 C++ 类型
      QString s = currentEdit->text();
      std::string data = s.toStdString();
      
      // 准备对不同的目标动作进行响应
      switch(plugins[i]->type) {

        case ActionAppend:
          plugins[i]->func(data);
          currentEdit->append(QString::fromStdString(data));
          break;

        case ActionReplace:
          plugins[i]->func(data);
          currentEdit->setText(QString::fromStdString(data));
          break;

        case ActionOpenHomeDir:
          QDesktopServices::openUrl(QUrl(QStandardPaths::locate(QStandardPaths::HomeLocation, "", QStandardPaths::LocateDirectory)));
          break;

        case ActionOpenLink:
          plugins[i]->func(data);
          QDesktopServices::openUrl(QUrl(QString::fromStdString(data)));
          break;

        default:
          break;
      }
    });
  }
}
