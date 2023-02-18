#pragma once
#include <QString>
#include <QMenu>

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

struct ndd_proc_data
{
	QString m_strPlugName;       // [必选]插件名称
	QString m_strFilePath;       // [留空]插件的全局路径, 由主程序传递下来的路径
	QString m_strComment;        // [可选]插件说明
	QString m_version;           // [可选]版本号码
	QString m_auther;            // [可选]作者名称
	int 	m_menuType;			 // [可选]菜单类型(0：不使用二级菜单 1：创建二级菜单)
	QMenu* 	m_rootMenu;			 // [依赖]当 m_menuType = 1，给出二级根菜单的地址(默认为 nullptr)

	ndd_proc_data(): m_rootMenu(nullptr), m_menuType(0)
	{

	}
};


typedef struct ndd_proc_data NDD_PROC_DATA;

typedef bool (*NDD_PROC_IDENTIFY_CALLBACK)(NDD_PROC_DATA* pProcData);
typedef void (*NDD_PROC_FOUND_CALLBACK)(NDD_PROC_DATA* pProcData, void* pUserData);


/***********在编译插件时提供的内容**************/

#ifdef  NOTEPAD_PLUGIN_MANAGER

#if defined(Q_OS_WIN)
#   if defined(NDD_EXPORTDLL)
#       define NDD_EXPORT __declspec(dllexport)
#   else
#       define NDD_EXPORT __declspec(dllimport)
#   endif
#endif

#include <Qsci/qsciscintilla.h>

#ifdef __cplusplus
	extern "C" {
#endif

	NDD_EXPORT bool NDD_PROC_IDENTIFY(NDD_PROC_DATA* pProcData);
	NDD_EXPORT int NDD_PROC_MAIN(QWidget* pNotepad, const QString& strFileName, std::function<QsciScintilla* ()>getCurEdit);

#ifdef __cplusplus
	}
#endif

#define NOTEPAD_PLUGIN_METADATA(name, version, author, comment, filepath)\
        pProcData->m_strPlugName = QString(name);       \
        pProcData->m_version     = QString(version);    \
        pProcData->m_strComment  = QString(comment);    \
        pProcData->m_auther      = QString(author);     \
        pProcData->m_strFilePath = QString(filepath);   \

#define NOTEPAD_PLUGIN_IMPLEMENT(imp_class)                        \
    imp_class *imp = new imp_class(pNotepad, getCurEdit()); \
    //imp->setWindowFlag(Qt::Window);                         \
    imp->show();

#endif  //NOTEPAD_PLUGIN_MANAGER
/***********在编译插件时提供的内容**************/
