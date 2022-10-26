#pragma once

#include <QDialog>
#include "ui_userregister.h"

class UserRegister : public QDialog
{
	Q_OBJECT

public:
	UserRegister(QWidget *parent = nullptr);
	~UserRegister();

signals:
	void signSendRegisterKey(QString key);

public slots:
	void slot_registerReplay(int code);
private slots:
	void slot_register();
	

private:
	Ui::UserRegisterClass ui;
	int m_isNetReplayCome;
	int m_regeisterStatus;
};
