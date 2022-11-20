#include "Qsci/qscilexerglobal.h"
#include "Qsci/qsciscintilla.h"

QsciLexerGlobal::QsciLexerGlobal(QObject *parent)
	: QsciLexer(parent)
{}

QsciLexerGlobal::~QsciLexerGlobal()
{}

int QsciLexerGlobal::lexerId() const
{
	return L_GLOBAL;
}

// Returns the language name.
const char* QsciLexerGlobal::language() const
{
	return "Gloabl";
}

QString QsciLexerGlobal::description(int style) const
{
	if (style == 0)
	{
		return "All Language Modify";
	}

	return QString();
}

QFont QsciLexerGlobal::defaultFont(int style) const
{
	switch (style)
	{
	case 0:
		return QsciLexer::s_defaultLangFont;
		break;
	default:
		break;
	}	
	return QsciLexer::s_defaultLangFont;
}
