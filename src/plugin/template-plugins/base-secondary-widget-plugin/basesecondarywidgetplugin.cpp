#include "basesecondarywidgetplugin.h"
#include "pluginframeworkhelper.h"
#include "pluginviewer.h"

#include <QDockWidget>
#include <QMessageBox>

BaseSecondaryWidgetPlugin::BaseSecondaryWidgetPlugin(QObject *parent)
    : QObject{parent}
{}

BaseSecondaryWidgetPlugin &BaseSecondaryWidgetPlugin::instance()
{
    static BaseSecondaryWidgetPlugin _plugin;
    return _plugin;
}

QString BaseSecondaryWidgetPlugin::PluginName()
{
    return "base-secondary-widget-plugin";
}

QString BaseSecondaryWidgetPlugin::PluginVersion()
{
    return "v0.1";
}

QString BaseSecondaryWidgetPlugin::PluginAuthor()
{
    return "author";
}

QString BaseSecondaryWidgetPlugin::PluginComment()
{
    return "二级菜单高级 UI 插件";
}

IPluginFramework::MenuType BaseSecondaryWidgetPlugin::PluginMenuType()
{
    return MenuType::SecondaryMenu;
}

void BaseSecondaryWidgetPlugin::PluginTrigger()
{
    //Trigger: Will never enter here.
}

void BaseSecondaryWidgetPlugin::registerPluginActions(QMenu *rootMenu)
{
    rootMenu->addAction("Default", this, [this](){
        QMessageBox::information(nullptr, "tip", "This is default tip message.");
    });

    rootMenu->addAction("Show Ui", this, [=](){
        auto viewer = new PluginViewer();
        viewer->show();
    });

    rootMenu->addAction("Show Sider", this, [=](){
        auto notepad = dynamic_cast<QMainWindow*>(s_notepad);

        auto viewer = new PluginViewer(s_notepad);
        viewer->show();
        viewer->setMinimumWidth(300);

        auto dock =new QDockWidget("Plugin Sider Viewer", s_notepad);
        connect(viewer, &QObject::destroyed, dock, &QObject::deleteLater);

        dock->setAllowedAreas(Qt::DockWidgetArea::LeftDockWidgetArea | Qt::DockWidgetArea::RightDockWidgetArea);
        dock->setWidget(viewer);

        (dynamic_cast<QMainWindow*>(s_notepad))->addDockWidget(Qt::RightDockWidgetArea, dock);
    });

    rootMenu->addAction("Show Bottom", this, [=](){
        auto notepad = dynamic_cast<QMainWindow*>(s_notepad);

        auto viewer = new PluginViewer(s_notepad);
        viewer->show();
        viewer->setMinimumHeight(100);

        auto dock =new QDockWidget("Plugin Bottom Viewer", s_notepad);
        connect(viewer, &QObject::destroyed, dock, &QObject::deleteLater);

        dock->setAllowedAreas(Qt::DockWidgetArea::BottomDockWidgetArea);
        dock->setWidget(viewer);

        (dynamic_cast<QMainWindow*>(s_notepad))->addDockWidget(Qt::BottomDockWidgetArea, dock);
    });
}

// Plug-in implementation wrapper
NDD_DECLARE_PLUGIN(BaseSecondaryWidgetPlugin::instance())
