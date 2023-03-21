#include <qobject.h>
#include <qstring.h>
#include <include/pluginGl.h>
#include <functional>
#include <qsciscintilla.h>
#include "ndd_plugin_implement.h"


bool NDD_PROC_IDENTIFY(NDD_PROC_DATA* pProcData) {
    // NOTEPAD_PLUGIN_METADATA
        // (name, version, author, comment, filepath)
        // 使用插件宏模板来完成简单的插件处理
    NOTEPAD_PLUGIN_METADATA("base-plugin", "0.1", "author", "基本插件", "");
    return true;
}


int NDD_PROC_MAIN(QWidget* pNotepad, const QString& strFileName, std::function<QsciScintilla* ()>getCurEdit) {
    // NOTEPAD_PLUGIN_IMPLEMENT
        // (NddPluginImplement);
        // 使用插件宏模板来完成基本的插件内容处理
    NddPluginImplement *imp = new NddPluginImplement();
    
    // 这个插件是一个窗口需要显示
    // imp->show();

    // 插件已成功实现
    return 0;
}
