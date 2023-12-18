#pragma once
#include <QObject>
#include <Qsci/qsciglobal.h>
#include <Qsci/qscilexer.h>

class QSCINTILLA_EXPORT QsciLexerPowerShell : public QsciLexer
{
	Q_OBJECT

public:
	enum {
		DEFAULT = 0,
        COMMENT,
        STRING,
        CHARACTER,
        NUMBER,
        VARIABLE,
        OPERATOR,
        IDENTIFIER,
        KEYWORD,
        CMDLET,
        ALIAS,
        FUNCTION,
        USER1,
        COMMENTSTREAM,
        HERE_STRING,
        HERE_CHARACTER,
        COMMENTDOCKEYWORD,
	};

	QsciLexerPowerShell(QObject *parent=0);
	virtual ~QsciLexerPowerShell();

	//! Returns the name of the language.
	const char* language() const;

	const char * lexer() const;

	//! Returns the foreground colour of the text for style number \a style.
	//!
	//! \sa defaultPaper()
	QColor defaultColor(int style) const;

	//! Returns the end-of-line fill for style number \a style.
	bool defaultEolFill(int style) const;

	//! Returns the font for style number \a style.
	QFont defaultFont(int style) const;

	//! Returns the background colour of the text for style number \a style.
	//!
	//! \sa defaultColor()
	QColor defaultPaper(int style) const;

	//int lexerId() const;

	//! Returns the set of keywords for the keyword set \a set recognised
	//! by the lexer as a space separated string.
	const char* keywords(int set);

	//! Returns the descriptive name for style number \a style.  If the
	//! style is invalid for this language then an empty QString is returned.
	//! This is intended to be used in user preference dialogs.
	QString description(int style) const;

private:
	QsciLexerPowerShell(const QsciLexerPowerShell&);
	QsciLexerPowerShell& operator=(const QsciLexerPowerShell&);
};
