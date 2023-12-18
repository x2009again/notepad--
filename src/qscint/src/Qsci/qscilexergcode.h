#pragma once
#include <QObject>
#include <Qsci/qsciglobal.h>
#include <Qsci/qscilexer.h>

class QSCINTILLA_EXPORT QsciLexerGCode : public QsciLexer
{
	Q_OBJECT

public:
	enum {
		DEFAULT = 0,
		KEYWORD, //关键字
		KEYWORD1, //关键字
		COMMNET,
		POSITIONX, //位置 X Y Z 
		POSITIONY, //位置 X Y Z 
		POSITIONZ, //位置 X Y Z 
		OFFSET,//I J K 偏移
		VELOCITY,//速度 F
		TIMES,// P 次数
	};
	QsciLexerGCode(QObject *parent=0);
	virtual ~QsciLexerGCode();

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
	QsciLexerGCode(const QsciLexerGCode&);
	QsciLexerGCode& operator=(const QsciLexerGCode&);
};
