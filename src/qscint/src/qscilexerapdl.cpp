#include "Qsci/qscilexerapdl.h"
#include "Qsci/qsciscintillabase.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>

//#if defined(Q_OS_WIN)
//	QFont QsciLexerApdl::s_defaultTxtFont("Courier New", QsciLexer::s_defaultFontSize);
//#elif defined(Q_OS_MAC)
//    QFont QsciLexerApdl::s_defaultTxtFont("STSong",14);
//#else
//    QFont QsciLexerApdl::s_defaultTxtFont("Courier 10 Pitch", 12);
//#endif


QsciLexerApdl::QsciLexerApdl(QObject *parent)
	: QsciLexer(parent)
{
	setLexerId(L_APDL);
	m_commentSymbol = "#";
}

QsciLexerApdl::~QsciLexerApdl()
{
}

// Returns the language name.
const char* QsciLexerApdl::language() const
{
	return "APDL";
}

// Returns the lexer name.
const char *QsciLexerApdl::lexer() const
{
	return "apdl";
}


// Returns the foreground colour of the text for a style.
QColor QsciLexerApdl::defaultColor(int style) const
{
	switch (style)
	{
	case DEFAULT:
		return QColor(0x80, 0x80, 0x80);

	case COMMENT:
	case COMMENTBLOCK:
		return QColor(0x00, 0x7f, 0x00);

	case NUMBER:
		return QColor(0xff, 0x80, 0x00);

	case PROCESSOR:
	case COMMAND:
	case SLASHCOMMAND:
	case STARCOMMAND:
		return QColor(0x00, 0x00, 0xff);

	case WORD:
	case STRING:
		return QColor(0x7f, 0x00, 0x7f);
	default:
		break;

	}

	return QsciLexer::defaultColor(style);
}

// Returns the end-of-line fill for a style.
bool QsciLexerApdl::defaultEolFill(int style) const
{
	//if (style == VerbatimString)
	//	return true;

	return QsciLexer::defaultEolFill(style);
}

// Returns the font of the text for a style.
QFont QsciLexerApdl::defaultFont(int style) const
{
	switch (style)
	{
	case PROCESSOR:
	case COMMAND:
	case SLASHCOMMAND:
	case STARCOMMAND:
	{
        QFont f(QsciLexer::s_defaultLangFont);
		f.setBold(true);
		return f;
	}
		break;
	default:
		break;
	}
	return QsciLexer::s_defaultLangFont;
}


// Returns the set of keywords.
const char* QsciLexerApdl::keywords(int set)
{
	if (set == 1)
		return "cfopen cfclose dim do dowhile else endif enddo finish get if then vwread vwrite";
	return 0;
}

// Returns the user name of a style.
QString QsciLexerApdl::description(int style) const
{
	switch (style)
	{
	case DEFAULT:
		return tr("Default");
	case COMMENT:
		return tr("Comment");
	case COMMENTBLOCK:
		return tr("Comment Block");
	case NUMBER:
		return tr("Number");
	case STRING:
		return tr("String");
	case OPERATOR:
		return tr("Operator");
	case WORD:
		return tr("Word");
	case PROCESSOR:
		return tr("Processor");
	case COMMAND:
		return tr("Command");
	case SLASHCOMMAND:
		return tr("Slash Command");
	case STARCOMMAND:
		return tr("Star Command");
	case ARGUMENT:
		return tr("Argument");
	case FUNCTION:
		return tr("Function");
	default:
		break;
	}
	return QString();
}


// Returns the background colour of the text for a style.
QColor QsciLexerApdl::defaultPaper(int style) const
{
	//if (style == VerbatimString)
	//	return QColor(0xe0, 0xff, 0xe0);

	return QsciLexer::defaultPaper(style);
}

// Return the lexer identifier.
//int QsciLexerApdl::lexerId() const
//{
//	return L_TXT;
//}
