#include "pathutil.h"

#include <QApplication>
#include <QDir>

QString PathUtil::execDir()
{
    QString path = qApp->applicationDirPath();
    return path + QDir::separator();
}

QString PathUtil::pluginDir() {
    QString path = execDir() + "plugin";
    return path + QDir::separator();
}
