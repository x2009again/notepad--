#include "qttestclass.h"
#include <qsciscintilla.h>

extern std::function<QsciScintilla* (QWidget*)> s_getCurEdit;


QtTestClass::QtTestClass(QWidget *parent): QWidget(parent)
{
	ui.setupUi(this);

	m_pNotepad = parent;
}

QtTestClass::~QtTestClass()
{}

void QtTestClass::on_upper()
{
	QsciScintilla* pEdit = s_getCurEdit(m_pNotepad);
	if (pEdit != nullptr)
	{
		QString text = pEdit->text();

		text = text.toUpper();

		pEdit->setText(text);
	}
}

void QtTestClass::on_lower()
{
	QsciScintilla* pEdit = s_getCurEdit(m_pNotepad);
	if (pEdit != nullptr)
	{
		QString text = pEdit->text();

		text = text.toLower();

		pEdit->setText(text);
	}
}