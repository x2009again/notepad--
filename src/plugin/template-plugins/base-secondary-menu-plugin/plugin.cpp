#include <qobject.h>
#include <qstring.h>
#include <include/pluginGl.h>
#include <functional>
#include <qsciscintilla.h>
#include "ndd_plugin_implement.h"

#include <QDebug>

bool NDD_PROC_IDENTIFY(NDD_PROC_DATA* pProcData) {
    // NOTEPAD_PLUGIN_METADATA
        // (name, version, author, comment, filepath)
        // 使用插件宏模板来完成简单的插件处理
    NOTEPAD_PLUGIN_METADATA("base-secondary-menu-plugin", "0.1", "author", "基本二级菜单插件", "");

    pProcData->m_menuType = 1;
    return true;
}

static QWidget *s_pNotepad;
static NDD_PROC_DATA s_pProcData;
static std::function<QsciScintilla* ()> s_getCurEdit;

int NDD_PROC_MAIN(QWidget* pNotepad, const QString& strFileName, std::function<QsciScintilla* ()>getCurEdit, NDD_PROC_DATA* pProcData) {
    // NOTEPAD_PLUGIN_IMPLEMENT
        // (NddPluginImplement);
        // 使用插件宏模板来完成基本的插件内容处理
    // NddPluginImplement *imp = new NddPluginImplement();

    if(pNotepad != nullptr) {
        s_pNotepad = pNotepad;
    } else return -1;

    if(pProcData != nullptr) {
        s_pProcData = *pProcData;
    } else return -1;

    if(getCurEdit != nullptr) {
        s_getCurEdit = getCurEdit;
    } else return -1;

    QAction *action = s_pProcData.m_rootMenu->addAction("打印信息");
    action->connect(action, &QAction::triggered, [](){
        qDebug() << "打印信息";
    });

    // 插件已成功实现
    return 0;
}
