#include "ndd_plugin_implement.h"

#include <qsciscintilla.h>
#include <QMenu>
#include <QDebug>

NddPluginImplement::NddPluginImplement(QWidget *parent, QsciScintilla *pEdit) : QWidget(parent)
  , currentEdit(pEdit)
{
    
}

NddPluginImplement::~NddPluginImplement()
{
    
}

void NddPluginImplement::setMenuActions(QMenu *menu)
{
  QAction *action1 = menu->addAction(u8"第一项打印");
  connect(action1, &QAction::triggered, this, [](){
    // 普通的菜单项
    qDebug() << "action1";
  });

  QAction *action2 = menu->addAction(u8"第二项打印");
  connect(action2, &QAction::triggered, this, [](){
    // 普通的菜单项
    qDebug() << "action2";
  });

  QAction *action3 = menu->addAction(u8"打印默认编辑器内容(源代码项注意)");
  connect(action3, &QAction::triggered, this, [=](){
    // FAQ: 当第一次使用 currentEdit 时，使用时可能会引发错误。
    //      此时 currentEdit 可能是空的。
    if (currentEdit == nullptr) {
      qDebug() << "Warring: currentEdit == nullptr!";
      return;
    }
    qDebug() << currentEdit->text();
  });

  QAction *action4 = menu->addAction(u8"设置当前编辑器为默认");
  connect(action4, &QAction::triggered, this, [=](){
    // 获取当前编辑器，并作为当前默认编辑器
    currentEdit =  getCurrentEditFunc();
    qDebug() << u8"设置完成";
  });

  QAction *action5 = menu->addAction(u8"打印当前编辑器内容");
  connect(action5, &QAction::triggered, this, [=](){
    // 获取当前编辑器，并直接打印编辑器内容
    QsciScintilla *edit =  getCurrentEditFunc();
    qDebug() << edit->text();
  });
}
