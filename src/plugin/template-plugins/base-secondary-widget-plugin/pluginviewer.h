#ifndef PLUGINVIEWER_H
#define PLUGINVIEWER_H

#include <QMainWindow>

namespace Ui {
class PluginViewer;
}

class PluginViewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit PluginViewer(QWidget *parent = nullptr);
    ~PluginViewer();

private:
    Ui::PluginViewer *ui;
};

#endif // PLUGINVIEWER_H
