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
	QMenu* 	m_rootMenu;			 // [依赖]当 m_menuType = 1，则给出二级根菜单的地址(默认为 nullptr)
	QAction* m_pAction;          // []如果m_menuType = 0，给出一级菜单动作的地址。其他值nullptr 插件内部不用填写，主程序传递下来

	ndd_proc_data(): m_rootMenu(nullptr), m_pAction(nullptr),m_menuType(0)
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

// v1.x 版本的插件接口
	NDD_EXPORT bool NDD_PROC_IDENTIFY(NDD_PROC_DATA* pProcData);
	NDD_EXPORT int NDD_PROC_MAIN(QWidget* pNotepad, const QString& strFileName, std::function<QsciScintilla* ()>getCurEdit, NDD_PROC_DATA* pProcData);

// v2.0 版本后的插件接口
    // NDD_EXPORT bool NDD_PROC_IDENTIFY(NDD_PROC_DATA* pProcData);
    // NDD_EXPORT int NDD_PROC_MAIN(QWidget* pNotepad, const QString& strFileName, std::function<QsciScintilla* (QWidget*)>getCurEdit, std::function<bool(QWidget*, int, void*)> pluginCallBack, NDD_PROC_DATA* procData);

#ifdef __cplusplus
	}
#endif

#define NOTEPAD_PLUGIN_METADATA(name, version, author, comment, filepath)\
        pProcData->m_strPlugName = QString(name);       \
        pProcData->m_version     = QString(version);    \
        pProcData->m_strComment  = QString(comment);    \
        pProcData->m_auther      = QString(author);     \
        pProcData->m_strFilePath = QString(filepath);   \

#define NOTEPAD_PLUGIN_IMPLEMENT(imp_class)                 \
    imp_class *imp = new imp_class(pNotepad, getCurEdit()); \
    //imp->setWindowFlag(Qt::Window);                       \
    imp->show();

/*** Interface Implementation Assistant ***/

// 原始接口
#define NOTEPAD_PLUGIN_METADATA_IDENTIFY(name, version, author, comment, filepath) \
    bool NDD_PROC_IDENTIFY(NDD_PROC_DATA* pProcData) {                  \
    NOTEPAD_PLUGIN_METADATA(name, version, author, comment, filepath)   \
        return true;\
    }\

#define NOTEPAD_PLUGIN_METADATA_IMPLEMENT(imp_class, imp_show_window) \
    int NDD_PROC_MAIN(QWidget* pNotepad, const QString& strFileName, std::function<QsciScintilla* ()>getCurEdit, NDD_PROC_DATA* pProcData) {\
        NOTEPAD_PLUGIN_IMPLEMENT(imp_class);   \
        if (imp_show_window) {\
            imp->show();\
        }\
        return 0;\
    }\

// v1 支持 Menu 的接口
#define NOTEPAD_PLUGIN_METADATA_IDENTIFY_V1(name, version, author, comment, filepath) \
    bool NDD_PROC_IDENTIFY(NDD_PROC_DATA* pProcData) {                  \
    NOTEPAD_PLUGIN_METADATA(name, version, author, comment, filepath)   \
	    pProcData->m_menuType = 1;\
        return true;\
    }\

#define NOTEPAD_PLUGIN_METADATA_IMPLEMENT_V1(imp_class, imp_show_window) \
    static QWidget *s_pNotepad;\
    static NDD_PROC_DATA s_pProcData;\
    static std::function<QsciScintilla* ()> s_getCurEdit;\
    int NDD_PROC_MAIN(QWidget* pNotepad, const QString& strFileName, std::function<QsciScintilla* ()>getCurEdit, NDD_PROC_DATA* pProcData) {\
        if(pNotepad != nullptr) {                   \
            s_pNotepad = pNotepad;                  \
        } else return -1;                           \
                                                    \
        if(pProcData != nullptr) {                  \
            s_pProcData = *pProcData;               \
        } else return -1;                           \
                                                    \
        if(getCurEdit != nullptr) {                 \
            s_getCurEdit = getCurEdit;              \
        } else return -1;                           \
                                                    \
        NOTEPAD_PLUGIN_IMPLEMENT(imp_class);        \
        imp->setCurrentEditFunc(s_getCurEdit);      \
        imp->setMenuActions(s_pProcData.m_rootMenu);\
        if (imp_show_window) {\
            imp->show();\
        }\
        return 0;\
    }\

#endif  //NOTEPAD_PLUGIN_MANAGER
/***********在编译插件时提供的内容**************/
