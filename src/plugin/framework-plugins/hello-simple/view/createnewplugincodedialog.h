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
    bool getQuoteCmake();

private slots:
    void on_r_save_edit_clicked();
    void on_r_save_dir_clicked();

private:
    Ui::CreateNewPluginCodeDialog *ui;
};

#endif // CREATENEWPLUGINCODEDIALOG_H
