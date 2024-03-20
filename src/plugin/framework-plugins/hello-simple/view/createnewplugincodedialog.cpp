#include "createnewplugincodedialog.h"
#include "ui_createnewplugincodedialog.h"

CreateNewPluginCodeDialog::CreateNewPluginCodeDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreateNewPluginCodeDialog)
{
    ui->setupUi(this);
}

CreateNewPluginCodeDialog::~CreateNewPluginCodeDialog()
{
    delete ui;
}

QString CreateNewPluginCodeDialog::getName()
{
    return ui->edit_name->text();
}

QString CreateNewPluginCodeDialog::getClassName()
{
    if (ui->edit_class_name->text().isEmpty()) {
        return ui->edit_name->text();
    }
    return ui->edit_class_name->text();;
}

QString CreateNewPluginCodeDialog::getVersion()
{
    return ui->edit_version->text();
}

QString CreateNewPluginCodeDialog::getAuthor()
{
    return ui->edit_author->text();
}

QString CreateNewPluginCodeDialog::getComment()
{
    QString d = ui->edit_descript->toPlainText();
    return d.replace("\\", "\\\\").replace("\"", "\\\"");
}

bool CreateNewPluginCodeDialog::getMenuType()
{
    return ui->r_none->isChecked();
}

bool CreateNewPluginCodeDialog::getSaveType()
{
    return ui->r_save_edit->isChecked();
}

bool CreateNewPluginCodeDialog::getQuoteCmake()
{
    return ui->r_cmake_quote->isChecked();
}

void CreateNewPluginCodeDialog::on_r_save_edit_clicked()
{
    return ui->groupBox_4->setEnabled(false);
}

void CreateNewPluginCodeDialog::on_r_save_dir_clicked()
{
    return ui->groupBox_4->setEnabled(true);
}

