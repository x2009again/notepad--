#include "bigfilemessage.h"

BigFileMessage::BigFileMessage(QWidget *parent)
	: QDialog(parent), m_result(-1)
{
	ui.setupUi(this);
}

BigFileMessage::~BigFileMessage()
{}


void BigFileMessage::setTip(QString msg)
{
	ui.label->setText(msg);
}

void BigFileMessage::slot_okBt()
{
	if (ui.textMode->isChecked())
	{
		m_result = 0;
	}
	else if(ui.bigTextMode->isChecked())
	{
		m_result = 1;
	}
	else if (ui.hexMode->isChecked())
	{
		m_result = 2;
	}
	done(m_result);
}

void BigFileMessage::slot_cancelBt()
{
	m_result = -1;
	done(m_result);
}