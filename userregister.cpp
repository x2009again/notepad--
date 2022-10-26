#include "userregister.h"
#include "jsondeploy.h"

#include <qmessagebox.h>
#include <QTimer>

UserRegister::UserRegister(QWidget *parent)
	: QDialog(parent), m_isNetReplayCome(-1)
{
	ui.setupUi(this);
	JsonDeploy::init();

	int status = JsonDeploy::getKeyValueFromNumSets(SOFT_STATUS);

	m_regeisterStatus = status;

	if (0 == status)
	{
		ui.status->setText(tr("Free Trial"));
	}
	else if (1 == status)
	{
		ui.status->setText(tr("Registered Version"));
	}
	else if (2 == status)
	{
		ui.status->setText(tr("License Expired"));
	}
	else if (3 == status)
	{
		ui.status->setText(tr("License Error"));
	}

	QString mac = JsonDeploy::getKeyValueFromSets("mac");
	ui.machineId->setText(mac);

	QString softKey = JsonDeploy::getKeyValueFromSets(SOFT_KEY);
	if (!softKey.isEmpty() && softKey != "0")
	{
		ui.licenceKey->setText(softKey);
	}
	
}

UserRegister::~UserRegister()
{
	JsonDeploy::close();
}

//服务器返回而来的注册验证消息
void UserRegister::slot_registerReplay(int code)
{
	if (code == 1)
	{
		//如果是正版，给出正版提示。
		ui.status->setText(tr("Registered Version"));

		m_regeisterStatus = 1;
	}
	else
	{

	}
	m_isNetReplayCome = code;
}

void UserRegister::slot_register()
{
	if (m_regeisterStatus == 1)
	{
		QMessageBox::warning(this, tr("Licence Key"), tr("It is already a registered version."));
		return;
	}

	if (ui.licenceKey->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Licence Key"), tr("Please scanning the donation, Write your email address in the message area.\nYou will get the registration code!"));
	}
	else
	{
		QString key = ui.licenceKey->text();
		if (key.size() != 12)
		{
			QMessageBox::warning(this, tr("Licence Key"), tr("Please enter the correct registration code!"));
			return;
		}

		emit signSendRegisterKey(key);

		//QMessageBox::information(this, tr("Licence Key"), tr("Processing succeeded. We will process your registration code in the background. It may take 1-3 days."));

		QString oldKey = JsonDeploy::getKeyValueFromSets(SOFT_KEY);
		if (oldKey.isEmpty() || oldKey == "0" || oldKey != key)
		{
			JsonDeploy::updataKeyValueFromSets(SOFT_KEY, key);
		}

		QEventLoop loop(this);
		QTimer::singleShot(5000, &loop, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数
		loop.exec(QEventLoop::ExcludeUserInputEvents);

		//表示来了消息
		if (m_isNetReplayCome != -1)
		{
			if (m_isNetReplayCome == 1)
			{
				QMessageBox::information(this, tr("Licence Key"), tr("Congratulations on your successful registration."));
	}
			else
			{
				QMessageBox::information(this, tr("Licence Key"), tr("Registration failed. Please try again later."));
}
		}
		else
		{
			QMessageBox::information(this, tr("Licence Key"), tr("Registration failed. Please try again later."));
		}

	}
}
