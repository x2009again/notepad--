#include "Qsci/qscilexergcode.h"
#include "Qsci/qsciscintillabase.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>

QsciLexerGCode::QsciLexerGCode(QObject *parent)
	: QsciLexer(parent)
{
	setLexerId(L_GCode);
	m_commentSymbol = ";";
}

QsciLexerGCode::~QsciLexerGCode()
{
}

// Returns the language name.
const char* QsciLexerGCode::language() const
{
	return "GCode";
}

// Returns the lexer name.
const char *QsciLexerGCode::lexer() const
{
	return "gcode";
}


// Returns the foreground colour of the text for a style.
QColor QsciLexerGCode::defaultColor(int style) const
{
	switch (style)
	{
	case KEYWORD:
		return QColor(0, 0, 255);
	case KEYWORD1:
		return QColor(0, 0, 128);
	case POSITIONX:
		return QColor(128, 0, 0);
	case POSITIONY:
		return QColor(0, 128, 0);
	case POSITIONZ:
		return QColor(0, 0, 128);
	case OFFSET:
		return QColor(255, 128, 0);
	case VELOCITY:
		return QColor(255, 0, 0);
	case COMMNET:
		return QColor(0, 128, 0);
	case TIMES:
		return QColor(127, 127, 0);
	default:
		break;
	}

	return QsciLexer::defaultColor(style);
}

// Returns the end-of-line fill for a style.
bool QsciLexerGCode::defaultEolFill(int style) const
{
	return QsciLexer::defaultEolFill(style);
}

// Returns the font of the text for a style.
QFont QsciLexerGCode::defaultFont(int style) const
{
	switch (style)
	{
	case KEYWORD:
	case KEYWORD1:
	case VELOCITY:
#if defined(Q_OS_MAC)
	{
		QFont f("Courier New", 18);
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
const char* QsciLexerGCode::keywords(int set)
{
	if (set == 1)
		return "G00 G01 G1 G02 G2 G03 G04 G05 G06 G07 G08 G09 G10 G16 G17 G18 G19 G20 " 
	"G21 G22 G22 G23 G23 G24 G25 G26 G28 G30 G31 G32 G33 G34 G35 G40 G41 G42 G43 G44 "
		"G45 G46 G47 G48 G49 G50 G51 G52 G53 G54 G55 G56 G57 G58 G59 G60 G61 G62 G63 "
	"G68 G69 G70 G71 G74 G75 G76 G80 G81 G33 G90 G91 G92 G93 G94 G95 G96 G97";

	if (set == 2)
		return "M00 M104 M02 M109 M03 M140 M04 M190 M05 M106 M06 M08 M09 M30";

	return 0;
}

// Returns the user name of a style.
QString QsciLexerGCode::description(int style) const
{
	switch (style)
	{
	case DEFAULT:
		return tr("Default");
	case KEYWORD:
		return tr("keyword");
	case KEYWORD1:
		return tr("Keyword1");
	case COMMNET:
		return tr("Comment");
	case POSITIONX:
		return tr("Position-X");
	case POSITIONY:
		return tr("Position-Y");
	case POSITIONZ:
		return tr("Position-Z");
	case OFFSET:
		return tr("Offset");
	case VELOCITY:
		return tr("Velocity");
	case TIMES:
		return tr("Times");
	default:
		break;
	}
	return QString();
}


// Returns the background colour of the text for a style.
QColor QsciLexerGCode::defaultPaper(int style) const
{
	//if (style == VerbatimString)
	//	return QColor(0xe0, 0xff, 0xe0);

	return QsciLexer::defaultPaper(style);
}

// Return the lexer identifier.
//int QsciLexerGCode::lexerId() const
//{
//	return L_TXT;
//}
