#include "pluginviewer.h"
#include "ui_pluginviewer.h"

PluginViewer::PluginViewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PluginViewer)
{
    ui->setupUi(this);
}

PluginViewer::~PluginViewer()
{
    delete ui;
}
