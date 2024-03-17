#ifndef CREATENEWPLUGINCODEDIALOG_H
#define CREATENEWPLUGINCODEDIALOG_H

#include <QDialog>

namespace Ui {
class CreateNewPluginCodeDialog;
}

class CreateNewPluginCodeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateNewPluginCodeDialog(QWidget *parent = nullptr);
    ~CreateNewPluginCodeDialog();

    QString getName();
    QString getClassName();
    QString getVersion();
    QString getAuthor();
    QString getComment();

    bool getMenuType();

    bool getSaveType();

private:
    Ui::CreateNewPluginCodeDialog *ui;
};

#endif // CREATENEWPLUGINCODEDIALOG_H
