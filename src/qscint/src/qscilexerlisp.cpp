#include "Qsci/qscilexerlisp.h"
#include "Qsci/qsciscintillabase.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>


QsciLexerLisp::QsciLexerLisp(QObject *parent)
	: QsciLexer(parent)
{
	setLexerId(L_LISP);
	m_commentSymbol = ";";
}

QsciLexerLisp::~QsciLexerLisp()
{
}

// Returns the language name.
const char* QsciLexerLisp::language() const
{
	return "LISP";
}

// Returns the lexer name.
const char *QsciLexerLisp::lexer() const
{
	return "lisp";
}


// Returns the foreground colour of the text for a style.
QColor QsciLexerLisp::defaultColor(int style) const
{
	switch (style)
	{
	case COMMENT:
		return QColor(0, 128, 0);
	case NUMBER:
		return QColor(255, 128, 0);
	case KEYWORD:
	case KEYWORD_KW:
		return QColor(0, 0, 255);
	case SYMBOL:
		return QColor(0, 0, 128);
	case STRING:
	case MULTI_COMMENT:
		return QColor(128, 128, 128);
	case OPERATOR:
		return QColor(0, 128, 192);
	case SPECIAL:
		return QColor(128, 0, 0);
	default:
		break;

	}

	return QsciLexer::defaultColor(style);
}

// Returns the end-of-line fill for a style.
bool QsciLexerLisp::defaultEolFill(int style) const
{

	return QsciLexer::defaultEolFill(style);
}

// Returns the font of the text for a style.
QFont QsciLexerLisp::defaultFont(int style) const
{
	switch (style)
	{
	case KEYWORD:
	case SYMBOL:
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
const char* QsciLexerLisp::keywords(int set)
{
	if (set == 1)
		return "not defun + - * / = < > <= >= princ eval apply funcall quote identity function complement backquote lambda set "
		"setq setf defmacro gensym make symbol intern name value plist get getf putprop remprop hash array aref "
		"car cdr caar cadr cdar cddr caaar caadr cadar caddr cdaar cdadr cddar cdddr caaaar caaadr caadar caaddr cadaar "
		"cadadr caddar cadddr cdaaar cdaadr cdadar cdaddr cddaar cddadr cdddar cddddr cons list append reverse last nth nthcdr "
		"member assoc subst sublis nsubst nsublis remove length mapc mapcar mapl maplist mapcan mapcon rplaca rplacd nconc delete "
		"atom symbolp numberp boundp null listp consp minusp zerop plusp evenp oddp eq eql equal cond case and or let l if "
		"prog prog1 prog2 progn go return do dolist dotimes catch throw error cerror break continue errset baktrace evalhook "
		"truncate float rem min max abs sin cos tan expt exp sqrt random logand logior logxor lognot bignums logeqv lognand lognor "
		"logorc2 logtest logbitp logcount integer nil";

	return 0;
}

// Returns the user name of a style.
QString QsciLexerLisp::description(int style) const
{
	switch (style)
	{
		case DEFAULT:
			return("Default");

		case COMMENT:
			return ("Comment");

		case NUMBER:
			return ("Number");

		case KEYWORD:
			return ("Keyword");

		case KEYWORD_KW:
			return ("Keyword_kw");

		case SYMBOL:
			return ("Symbol");

		case STRING:
			return ("String");

		case STRINGEOL:
			return ("String Eol");

		case IDENTIFIER:
			return ("Identifier");

		case OPERATOR:
			return ("Operator");

		case SPECIAL:
			return ("Special");

		case MULTI_COMMENT:
			return ("Multi Comment");
	}
	return QString();
}


// Returns the background colour of the text for a style.
QColor QsciLexerLisp::defaultPaper(int style) const
{

	return QsciLexer::defaultPaper(style);
}
