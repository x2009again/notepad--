#include "Qsci/qscilexerlog.h"
#include "Qsci/qsciscintillabase.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>

//#if defined(Q_OS_WIN)
//	QFont QsciLexerLog::s_defaultTxtFont("Courier New", QsciLexer::s_defaultFontSize);
//#elif defined(Q_OS_MAC)
//    QFont QsciLexerLog::s_defaultTxtFont("STSong",14);
//#else
//    QFont QsciLexerLog::s_defaultTxtFont("Courier 10 Pitch", 12);
//#endif


QsciLexerLog::QsciLexerLog(QObject *parent)
	: QsciLexer(parent)
{
	setLexerId(L_LOG);
	m_commentSymbol = "#";
}

QsciLexerLog::~QsciLexerLog()
{
}

// Returns the language name.
const char* QsciLexerLog::language() const
{
	return "LOG";
}

// Returns the lexer name.
const char *QsciLexerLog::lexer() const
{
	return "log";
}


// Returns the foreground colour of the text for a style.
QColor QsciLexerLog::defaultColor(int style) const
{
	switch (style)
	{
	case Num:
		return QColor(0, 0, 255);
	case Keyword:
		return QColor(163, 21, 21);
	case Date:
		return QColor(0, 128, 0);

	default:
		break;

	}

	return QsciLexer::defaultColor(style);
}

// Returns the end-of-line fill for a style.
bool QsciLexerLog::defaultEolFill(int style) const
{
	//if (style == VerbatimString)
	//	return true;

	return QsciLexer::defaultEolFill(style);
}

// Returns the font of the text for a style.
QFont QsciLexerLog::defaultFont(int style) const
{
	switch (style)
	{
	case Default:
		return QsciLexer::s_defaultLangFont;
	case Num:
	case Date:
		return QsciLexer::s_defaultLangFont;
	case Keyword:
#if defined(Q_OS_WIN)
		return QsciLexer::s_defaultLangFont;
#elif defined(Q_OS_MAC)
	{
        QFont f(QsciLexer::s_defaultLangFont);
		f.setBold(true);
		return f;
	}
#else
	{
        QFont f(s_defaultLangFont);
		f.setBold(true);
		return f;
	}
#endif
			break;
	default:
		break;
}
	return QsciLexer::s_defaultLangFont;
}


// Returns the set of keywords.
const char* QsciLexerLog::keywords(int set)
{
	if (set == 1)
		return "info error debug warn ";
	return 0;
}

// Returns the user name of a style.
QString QsciLexerLog::description(int style) const
{
	switch (style)
	{
	case Default:
		return tr("Default");
	case Num:
		return tr("Num");
	case Date:
		return tr("Date");
	case Keyword:
		return tr("Keyword");
	default:
		break;
	}
	return QString();
}


// Returns the background colour of the text for a style.
QColor QsciLexerLog::defaultPaper(int style) const
{
	//if (style == VerbatimString)
	//	return QColor(0xe0, 0xff, 0xe0);

	return QsciLexer::defaultPaper(style);
}

// Return the lexer identifier.
//int QsciLexerLog::lexerId() const
//{
//	return L_TXT;
//}
