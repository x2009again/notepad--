#pragma once
#include <QObject>
#include <Qsci/qsciglobal.h>
#include <Qsci/qscilexer.h>


class QsciScintilla;
class QsciStyle;

class QSCINTILLA_EXPORT QsciLexerGlobal  : public QsciLexer
{
	Q_OBJECT

public:
	QsciLexerGlobal(QObject *parent=0);
	virtual ~QsciLexerGlobal();

	int lexerId() const;
	const char* language() const;
	QString description(int style) const;
	QFont defaultFont(int style) const;
};
