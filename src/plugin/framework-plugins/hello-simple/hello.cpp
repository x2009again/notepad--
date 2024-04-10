#include "hello.h"
#include "pluginframeworkhelper.h"
#include "view/createnewplugincodedialog.h"

#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextCodec>

#include <utils/pathutil.h>

Hello::Hello(QObject *parent)
    : QObject{parent}
{

}

Hello &Hello::instance()
{
    static Hello hello;
    return hello;
}

QString Hello::PluginName()
{
    return "Hello";
}

QString Hello::PluginVersion()
{
    return "0.1";
}

QString Hello::PluginAuthor()
{
    return "zinface";
}

QString Hello::PluginComment()
{
    return "hello 插件";
}

IPluginFramework::MenuType Hello::PluginMenuType()
{
    return IPluginFramework::MenuType::SecondaryMenu;
}

void Hello::registerNotepad(QWidget *notepad)
{
    s_notepad = notepad;
}

void Hello::registerStrFileName(QString str_file_name)
{
    s_str_file_name = str_file_name;
}

void Hello::PluginTrigger()
{
    QMessageBox::information(nullptr, "Tip", "This is Hello Plugin");
}

void Hello::registerPluginActions(QMenu *rootMenu)
{
    rootMenu->addAction("First Plugin Action", this, [](){
        QMessageBox::information(nullptr, "Tip", "This is Hello Plugin(First Action)");
    });
    rootMenu->addAction("Second Plugin Action", this, [](){
        QMessageBox::information(nullptr, "Tip", "This is Hello Plugin(Second Action)");
    });
    rootMenu->addAction("Three Plugin Action", this, [this](){


//        QsciScintilla *edit = s_get_cur_edit_callback(s_notepad);

//        QMessageBox::information(nullptr, "Tip", edit->text());

        QVariant editName = PluginFrameworkHelper::DoNewEdit(s_notepad, s_plugin_callback);
        QMessageBox::information(nullptr, "New Edit", editName.toString());
    });
    rootMenu->addAction("Monitor textChanged", this, [=](){
        connect(s_get_cur_edit_callback(s_notepad), &QsciScintilla::textChanged, this, [](){
            QMessageBox::information(nullptr, "Tip", "textChanged!");
        });
    });

    rootMenu->addAction("Create New Plugin Code Template", this, [this](){
        // 1. Dialog to PluginName and ClassName
        CreateNewPluginCodeDialog dialog;

        if (dialog.exec() == QDialog::Accepted) {

            /**  H  **/
            QFile file_h("://template/plugintemplate.h");
            file_h.open(QIODevice::ReadOnly);
            QString h = file_h.readAll();
            file_h.close();

            // ifdef/class
            h.replace("PLUGINTEMPLATE", dialog.getClassName().toUpper());
            h.replace("PluginTemplate", dialog.getClassName());

            /**  CPP  **/
            QFile file_cpp("://template/plugintemplate.cpp");
            file_cpp.open(QIODevice::ReadOnly);
            QString cpp = file_cpp.readAll();
            file_cpp.close();

            // include/class
            cpp.replace("plugintemplate", dialog.getClassName().toLower());
            cpp.replace("PluginTemplate", dialog.getClassName());

            // 1. 处理插件名称、版本、作者、简介说明
#if _MSC_VER
    QTextCodec *codec = QTextCodec::codecForName("GBK");
#else
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
#endif
            // QTextCodec::setCodecForLocale(codec);

            cpp.replace("//Name", dialog.getName());
            cpp.replace("//Version", dialog.getVersion());
            cpp.replace("//Author", dialog.getAuthor());
            cpp.replace("//Comment", dialog.getComment());

            // 2. 处理插件预期触发操作
            if (dialog.getMenuType() == true) {
                cpp.replace("//Trigger", "QMessageBox::information(nullptr, \"tip\", \"This is default tip message.\");");
                cpp.replace("//Actions", "//Actions: Will never enter here.");
            } else {
                cpp.replace("MenuType::None", "MenuType::SecondaryMenu");
                cpp.replace("//Trigger", "//Trigger: Will never enter here.");

                cpp.replace("//Actions","rootMenu->addAction(\"Default\", this, [this](){\n"
                                    "        QMessageBox::information(nullptr, \"tip\", \"This is default tip message.\");\n"
                                    "    });");
            }
            // 3. 解锁 NDD_DECLARE_PLUGIN 宏注释
            cpp.replace("//NDD_DECLARE_PLUGIN", "NDD_DECLARE_PLUGIN");


            /** CMake **/
            QFile file_txt("://template/plugintemplate.txt");
            file_txt.open(QIODevice::ReadOnly);
            QString txt = file_txt.readAll();
            file_txt.close();

            // 1. 处理对 CMakeLists.txt 的构建解释, 目标名称
            txt.replace("plugintemplate", dialog.getClassName().toLower());
            // 2. 这是在模板中的一个路径占位，但原始内容被构建解释改变了
            QString txtPlacePath = QString("#framework-plugins/%1").arg(dialog.getClassName().toLower());
            // 3. 这是在模板中的一个路径占位，使用引用的方式才会替换
            QString txtIncludePath = QString("path/to/plugin.cmake");

            /** -------------------------------- */

            // 1. 创建编辑器
            QVariant editName = PluginFrameworkHelper::DoNewEdit(s_notepad, s_plugin_callback);
            // 2. 获取刚才创建的编辑器
            QsciScintilla *curEdit = s_get_cur_edit_callback(s_notepad);

            // 3. 将代码设置到内部，或打开对话框保存到目录
            if (dialog.getSaveType() == true) {
                curEdit->setText(h + "\n\n\n" + cpp + "\n\n\n" + txt);
            } else {
                // 获取保存目录路径
                QString existDir = QFileDialog::getExistingDirectory(nullptr, "Open Save Dir", PathUtil::execDir());
                if (existDir.isEmpty() == false) {
                    QMessageBox::information(nullptr, "Note", QString("已保存到目录:\n%1").arg(existDir));
                    QString header = QString("%1/%2.h").arg(existDir);
                    QString source = QString("%1/%2.cpp").arg(existDir);
                    QString plugincmake = QString("%1/plugin.cmake").arg(existDir);

                    QFile fh(header.arg(dialog.getClassName().toLower()));
                    QFile fcpp(source.arg(dialog.getClassName().toLower()));
                    QFile fcmake(plugincmake);

                    fh.open(QIODevice::WriteOnly);
                    // fh.write(h.toLocal8Bit()); // 存在乱码情况，使用 QTextStream
                    QTextStream fhout(&fh);
                    fhout.setCodec("utf-8");
                    fhout.setGenerateByteOrderMark(true); // with Bom
                    fhout << h;
                    fh.close();

                    fcpp.open(QIODevice::WriteOnly);
                    // fcpp.write(cpp.toLocal8Bit()); // 存在乱码情况，使用 QTextStream
                    QTextStream fcppout(&fcpp);
                    fcppout.setCodec("utf-8");
                    fcppout.setGenerateByteOrderMark(true); // with Bom
                    fcppout << cpp;
                    fcpp.close();


                    // 可能逻辑: 如果存放在源代码目录树中，则处理掉路径前缀部分(替换占位)/或直接使用存储目录(替换占位)
                    QString posiblePath = QString("plugin/framework-plugins");
                    if (existDir.contains(posiblePath) == true) {
                        int posibleIndex = existDir.indexOf("framework-plugins");
                        if (posibleIndex > 0) {
                            txt.replace(txtPlacePath, existDir.mid(posibleIndex));
                            txt.replace(txtIncludePath, existDir.mid(posibleIndex) + "/plugin.cmake");
                        }
                    } else {
                        txt.replace(txtPlacePath, existDir);
                        txt.replace(txtIncludePath, existDir + "/plugin.cmake");
                    }
                    curEdit->setText(txt);

                    // 引用逻辑: 如果使用引用 plugin.cmake 的方式，那么将自动从 txt 模板中取8-18行的内容
                    if (dialog.getQuoteCmake()) {
                        fcmake.open(QIODevice::WriteOnly);
                        // fcpp.write(cpp.toLocal8Bit()); // 存在乱码情况，使用 QTextStream
                        QTextStream fcmakeout(&fcmake);
                        fcmakeout.setCodec("utf-8");
                        fcmakeout.setGenerateByteOrderMark(true); // with Bom
                        fcmakeout << txt.split("\n").mid(29,17).join("\n").replace(existDir, "${CMAKE_CURRENT_LIST_DIR}");
                        fcmakeout << txt.split("\n").mid(8,10).join("\n").replace(existDir, "${CMAKE_CURRENT_LIST_DIR}");
                        fcmake.close();
                    }
                } else {
                    QMessageBox::information(nullptr, "Note", "未选中存储目录，本次操作被忽略");
                    QString content = QString("未选中存储目录，本次操作被忽略，但保留了本次内容\n\n\n") + h + "\n\n\n" + cpp;
                    curEdit->setText(content);
                }
            }
        }
    });
}

void Hello::registerCurrentEditCallback(std::function<QsciScintilla *(QWidget *)> get_cur_edit_callback)
{
    s_get_cur_edit_callback = get_cur_edit_callback;
}

void Hello::registerPluginCallBack(std::function<bool (QWidget *, int, void *)> plugin_callback)
{
    s_plugin_callback = plugin_callback;
}


NDD_DECLARE_PLUGIN(Hello::instance())
