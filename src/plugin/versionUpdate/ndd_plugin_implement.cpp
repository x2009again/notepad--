#include "ndd_plugin_implement.h"
#include "ui_ndd_plugin_implement.h"

#include <qsciscintilla.h>

#include "StatusWidget/statuswidget.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QApplication>
#include <QDesktopServices>

NddPluginImplement::NddPluginImplement(QWidget *parent, QsciScintilla *pEdit) : QMainWindow (parent)
  , ui(new Ui::NddPluginImplement)
  , currentEdit(pEdit)
{
    ui->setupUi(this);

    StatusWidget *statusWidget = new StatusWidget(currentEdit);
    statusWidget->setShowMessage(u8"提示", u8"正在检查更新");
    statusWidget->setDirection(StatusWidget::TopIn, StatusWidget::RightOut);
    statusWidget->start(false);
    statusWidget->show();
#ifdef Q_OS_WIN
    statusWidget->setIcon(QPixmap("://assets/5.png"));
#else
    statusWidget->setIcon(QIcon::fromTheme("notepad--").pixmap(32,32));
#endif
    statusWidget->setStyleSheet("border-radius: 8px");
    QNetworkAccessManager manager;
    QEventLoop loop;
    QJsonDocument document;

    connect(&manager, &QNetworkAccessManager::finished, this, [&](QNetworkReply *reply){
        if (reply->error() == QNetworkReply::NetworkError::NoError) {
            int statusCode = reply->attribute(QNetworkRequest::Attribute::HttpStatusCodeAttribute).toInt();
            qDebug() << u8"[Notepad--]: 请求状态: " << statusCode;
            if (statusCode == 301) {
                QUrl redirectionUrl = reply->attribute(QNetworkRequest::Attribute::RedirectionTargetAttribute).toUrl();
                qDebug() << u8"[Notepad--]: 请求被重定向: (" << redirectionUrl.toString() << ")";
                manager.get(QNetworkRequest(redirectionUrl));
                // tempFilePath = saveHttpImageToTemplateFile(redirectionUrl.toString());
            } else {
                qDebug() << u8"[Notepad--]: 请求被确认";
                QByteArray byteArray;
                byteArray = reply->readAll();
                if(!byteArray.isNull()) {
                    document = QJsonDocument::fromJson(byteArray);
                }
                loop.quit();
            }
        } else {
            loop.quit();
        }
    });
    QNetworkReply *reply = manager.get(QNetworkRequest(QUrl("https://gitee.com/api/v5/repos/cxasm/notepad--/releases/latest")));
    loop.exec();

    qDebug() << reply->attribute(QNetworkRequest::Attribute::HttpStatusCodeAttribute).toInt();
    qDebug() << reply->errorString();

    if (!document.isEmpty()) {
        QJsonObject obj = document.object();
        if (obj.contains("tag_name")) {
            QString kv = obj.value("tag_name").toString();
            QString currentVersion = qApp->applicationVersion();
            if (currentVersion.compare(kv) != 0) {
                statusWidget->setShowMessage(u8"版本更新", QString("%1 > %2").arg(currentVersion).arg(kv));
                QDesktopServices::openUrl(QUrl("https://gitee.com/cxasm/notepad--/releases/latest"));
            } else {
                statusWidget->setShowMessage(u8"提示", u8"检查完成，没有任何更新");
            }
            qDebug() << u8"[Notepad--]: 版本确认 -" << kv;
        } else {

        }
    } else {
        qDebug() << u8"[Notepad--]: 版本未未确认";
        statusWidget->setShowMessage(u8"提示", u8"检查更新失败");
    }
}

NddPluginImplement::~NddPluginImplement()
{
    
}
