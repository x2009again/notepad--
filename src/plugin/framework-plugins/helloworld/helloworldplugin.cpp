

// #include "../include/pluginGl.h"

#include "qdebug.h"
#include <pluginGl.h>
#include <functional>
#include <qsciscintilla.h>

//#ifdef  NOTEPAD_PLUGIN_MANAGER


#define NDD_EXPORTDLL

#if defined(Q_OS_WIN)
	#if defined(NDD_EXPORTDLL)
		#define NDD_EXPORT __declspec(dllexport)
	#else
		#define NDD_EXPORT __declspec(dllimport)
	#endif
#else
	#define NDD_EXPORT __attribute__((visibility("default")))
#endif

#include "helloworld.h"

#ifdef __cplusplus
	extern "C" {
#endif

#define plugin HelloWorld::instance()

NDD_EXPORT bool NDD_PROC_IDENTIFY(NDD_PROC_DATA* pProcData) {

    qDebug() << (pProcData == NULL);
    if(pProcData == NULL)
	{
		return false;
	}
    pProcData->m_strPlugName = plugin.PluginName();
    pProcData->m_strComment = plugin.PluginComment();
    pProcData->m_version = plugin.PluginVersion();
    pProcData->m_auther = plugin.PluginAuthor();
    pProcData->m_menuType = plugin.PluginMenuType();

    return true;
}


NDD_EXPORT int NDD_PROC_MAIN(QWidget* pNotepad, const QString& strFileName, std::function<QsciScintilla* (QWidget*)>getCurEdit, std::function<bool(QWidget*, int, void*)> pluginCallBack, NDD_PROC_DATA* procData){

    plugin.registerNotepad(pNotepad);
    plugin.registerStrFileName(strFileName);
    plugin.registerCurrentEditCallback(getCurEdit);
    plugin.registerPluginCallBack(pluginCallBack);

    if (plugin.PluginMenuType() == IPluginFramework::None) {
        procData->m_pAction->connect(procData->m_pAction, &QAction::triggered, [](){
            plugin.PluginTrigger();
        });
    } else {
        plugin.registerPluginActions(procData->m_rootMenu);
    }

    qDebug() << strFileName;
}

#ifdef __cplusplus
	}
#endif

//#endif  //NOTEPAD_PLUGIN_MANAGER
