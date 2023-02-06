#include "aboutndd.h"

AboutNdd::AboutNdd(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.label->setOpenExternalLinks(true);
	connect(ui.aboutPushButton, &QPushButton::clicked, this, &AboutNdd::onButtonOkayClicked);
}

AboutNdd::~AboutNdd()
{}

void AboutNdd::appendText(QString text)
{
	ui.nddMsgText->appendPlainText(text);
}

void AboutNdd::onButtonOkayClicked()
{
	close();
}
