#include "Qsci/qscilexerintelhex.h"
#include "Qsci/qsciscintillabase.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>


QsciLexerIntelHex::QsciLexerIntelHex(QObject *parent)
	: QsciLexer(parent)
{
	setLexerId(L_IHEX);
	m_commentSymbol = ";";
}

QsciLexerIntelHex::~QsciLexerIntelHex()
{
}

// Returns the language name.
const char* QsciLexerIntelHex::language() const
{
	return "IHEX";
}

// Returns the lexer name.
const char *QsciLexerIntelHex::lexer() const
{
	return "ihex";
}


// Returns the foreground colour of the text for a style.
QColor QsciLexerIntelHex::defaultColor(int style) const
{
	switch (style)
	{
	case DEFAULT:
		return QColor();

	case RECSTART:
	case RECTYPE:
	case RECTYPE_UNKNOWN:
		return QColor(127, 0, 0);

	case BYTECOUNT:
		return QColor(127,127,0);

	case BYTECOUNT_WRONG:
		return QColor(255,255,0);

	case NOADDRESS:
		return QColor(127,0,255);

	case DATAADDRESS:
	case STARTADDRESS:
	case EXTENDEDADDRESS:
		return QColor(0, 127, 255);

	case CHECKSUM:
		return QColor(0,191,0);

	case CHECKSUM_WRONG:
		return QColor(255,255,0);

	case GARBAGE:
		return QColor();
	default:
		break;
		
	}

	return QsciLexer::defaultColor(style);
}

// Returns the end-of-line fill for a style.
bool QsciLexerIntelHex::defaultEolFill(int style) const
{

	return QsciLexer::defaultEolFill(style);
}

// Returns the font of the text for a style.
QFont QsciLexerIntelHex::defaultFont(int style) const
{
	switch (style)
	{
	case DATA_ODD:
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
	case GARBAGE:
	case DATA_UNKNOWN:
	case ADDRESSFIELD_UNKNOWN:
	case RECTYPE_UNKNOWN:
#if defined(Q_OS_MAC)
	{
		QFont f("Courier New", 18);
		f.setItalic(true);
		return f;
	}
#else
	{
		QFont f(s_defaultLangFont);
		f.setItalic(true);
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
const char* QsciLexerIntelHex::keywords(int set)
{
	return 0;
}

// Returns the user name of a style.
QString QsciLexerIntelHex::description(int style) const
{
	switch (style)
	{
		case DEFAULT:
			return("Default");

		case RECSTART:
			return ("Recstart");

		case RECTYPE:
			return ("Rectype");

		case RECTYPE_UNKNOWN:
			return ("Rectype Unknown");

		case BYTECOUNT:
			return ("Byte Count");

		case BYTECOUNT_WRONG:
			return ("Bytecount Wrong");

		case NOADDRESS:
			return ("Noaddress");

		case DATAADDRESS:
			return ("Data Address");

		case RECCOUNT:
			return ("Reccount");

		case STARTADDRESS:
			return ("Start address");

		case ADDRESSFIELD_UNKNOWN:
			return ("Addressfield Unknown");

		case EXTENDEDADDRESS:
			return ("Extended Address");

		case DATA_ODD:
			return ("Data Odd");

		case DATA_EVEN:
			return ("Data Even");

		case DATA_UNKNOWN:
			return ("Data Unknown");

		case DATA_EMPTY:
			return ("Data Empty");

		case CHECKSUM:
			return ("Checksum");

		case CHECKSUM_WRONG:
			return ("Checksum Wrong");

		case GARBAGE:
			return ("Garbage");
	}
	return QString();
}


// Returns the background colour of the text for a style.
QColor QsciLexerIntelHex::defaultPaper(int style) const
{
	switch(style)
	{
	case BYTECOUNT_WRONG:
	case CHECKSUM_WRONG:
		return QColor(255, 0, 0);

	default:
		break;
	}
	return QsciLexer::defaultPaper(style);
}
