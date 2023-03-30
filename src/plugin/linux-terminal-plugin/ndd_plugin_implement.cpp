#include "ndd_plugin_implement.h"

#include <qsciscintilla.h>
#include <QDebug>
#include <QMainWindow>
#include <QTabBar>

NddPluginImplement::NddPluginImplement(QWidget *parent, QsciScintilla *pEdit) : QWidget(parent)
    , currentWidget(parent)
    , currentEdit(pEdit)
    , m_dockWidget(nullptr)
    , m_dockTitleBarWidget(nullptr)
    , m_dockEmptyTitleBarWidget(new QWidget())
    , m_tabWidget(nullptr)
{
    
}

NddPluginImplement::~NddPluginImplement()
{
    
}

void NddPluginImplement::setMenuActions(QMenu *menu)
{
    auto actionNewTerminal = menu->addAction("New Terminal");
    connect(actionNewTerminal, &QAction::triggered, this, [&](){

        auto mainWindow = dynamic_cast<QMainWindow*>(currentWidget);

        if(!mainWindow) {
            qDebug() << "None";
            return;
        }
        qDebug() << "Yep";

        if (!m_dockWidget) {
            m_dockWidget = new QDockWidget(mainWindow);
            m_dockTitleBarWidget = m_dockWidget->titleBarWidget();
            m_dockWidget->setTitleBarWidget(m_dockEmptyTitleBarWidget);
            m_dockWidget->setMinimumSize(100, 200);
            connect(m_dockWidget, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)), this, SLOT(onDockLocationChanged(Qt::DockWidgetArea)));
            mainWindow->addDockWidget(Qt::BottomDockWidgetArea, m_dockWidget);
        }

        if (!m_dockWidget->isVisible())
            m_dockWidget->show();

        if (!m_tabWidget) {
            m_tabWidget = new QTabWidget(m_dockWidget);
            m_tabWidget->setTabsClosable(true);
            m_tabWidget->setMovable(true);
            m_tabWidget->tabBar()->setStyleSheet("QTabBar::tab{padding-left:10px}");
            connect(m_tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(onTabCloseRequested(int)));
            m_dockWidget->setWidget(m_tabWidget);
        }

        int tabIndex = m_tabWidget->addTab(new QTermWidget(), "New Terminal");
        m_tabWidget->setCurrentIndex(tabIndex);
    });
}

void NddPluginImplement::onDockLocationChanged(Qt::DockWidgetArea area)
{
    if (area == Qt::NoDockWidgetArea) {
        m_dockWidget->setTitleBarWidget(m_dockTitleBarWidget);
    }
    else {
        m_dockWidget->setTitleBarWidget(m_dockEmptyTitleBarWidget);
    }
}

void NddPluginImplement::onTabCloseRequested(int index)
{
    auto widget = m_tabWidget->widget(index);
    if (widget)
        delete widget;

    if (m_tabWidget->count() == 0)
        m_dockWidget->hide();
}
