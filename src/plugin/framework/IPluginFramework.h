#ifndef __IPLUGINFRAMEWORK__H__
#define __IPLUGINFRAMEWORK__H__

#include <QAction>
#include <cstddef>
#include <qmenu.h>
#include <qobject.h>
#include <qobjectdefs.h>
#include <qsciscintilla.h>

#define IPluginFramework_IID "com.gitee.ndd.pluginframework"

class IPluginFramework
{
public:
    // 
    enum MenuType {
        None = 0,
        SecondaryMenu = 1,
    };

    /**
     * @brief 让编辑器做指定的事
     * @note 目前编辑器支持由插件发出两种指令
     */
    enum Do {
        NewEdit,
        ChangeSyntax,
    };

    /********************************************* 插件基本信息/
    /**
     * @brief 插件名称
     * @return
     */
    virtual QString PluginName() = 0;
    /**
     * @brief 插件版本
     * @return
     */
    virtual QString PluginVersion() = 0;
    /**
     * @brief 插件作者
     * @return
     */
    virtual QString PluginAuthor() = 0;
    /**
     * @brief 插件说明
     * @return
     */
    virtual QString PluginComment() = 0;
    /**
     * @brief 插件菜单类型
     * @note 如仅提供插件，而不实现任何其它菜单功能则建议为 None，否则
     * @return
     */
    virtual MenuType PluginMenuType() = 0;


    /********************************************* 插件的主程序 Notepad 注册*/

    virtual void registerNotepad(QWidget *notepad) = 0;

    /********************************************* 插件的一级(MenuType=None)或二级(MenuType=SecondaryMenu)菜单触发与注册接口/

    /**
     * @brief 由插件实现的一级菜单触发动作(默认提供的动作)
     * @note 由于框架无法在 C 风格函数入口处完成事件注册，则由插件提供者自行在 registerTirgger 中连接信号到此处
     */
    virtual void PluginTrigger() = 0;


    /**
     * @brief 由插件实现的一级菜单注册动作(当 MenuType 为 None 时有效)
     * @param action
     * @note 由于框架在 C 风格函数入口处使用 lambda 完成事件注册连接到 PluginTrigger，则不需要插件实现此接口
     */
    // virtual void registerTrigger(QAction *action) = 0;


    /**
     * @brief 由插件实现的二级菜单触发动作集合(默认提供的动作)
     * @param menu
     * @note 由框架在 C 风格函数入口处代为转交，则由插件提供者自行实现来完成提供内部的 QAction 集合
     */
    virtual void registerPluginActions(QMenu *rootMenu) = 0;

    /********************************************* 插件当前编辑器函数回调注册*/

    /**
     * @brief 由框架提供的获取当前编辑器的回调函数注册
     * @param curEdit
     * @note 由于框架无法在 C 风格函数入口处完成回调函数注册，则由插件提供者自行在 registerCurrentEditCallback 中连接信号到此处
     */
    virtual void registerCurrentEditCallback(std::function<QsciScintilla*(QWidget*)> get_cur_edit_callback) = 0;

//protected:
    /**
     * @brief s_getCurEdit 为回调函数，用于获取当前编辑器
     */
//    std::function<QsciScintilla*(QWidget*)> s_getCurEdit;
};

Q_DECLARE_INTERFACE(IPluginFramework, IPluginFramework_IID)

#endif  //!__IPLUGINFRAMEWORK__H__
