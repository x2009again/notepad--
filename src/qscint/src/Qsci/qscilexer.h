﻿// This defines the interface to the QsciLexer class.
//
// Copyright (c) 2023 Riverbank Computing Limited <info@riverbankcomputing.com>
// 
// This file is part of QScintilla.
// 
// This file may be used under the terms of the GNU General Public License
// version 3.0 as published by the Free Software Foundation and appearing in
// the file LICENSE included in the packaging of this file.  Please review the
// following information to ensure the GNU General Public License version 3.0
// requirements will be met: http://www.gnu.org/copyleft/gpl.html.
// 
// If you do not wish to use this file under the terms of the GPL version 3.0
// then you may purchase a commercial license.  For more information contact
// info@riverbankcomputing.com.
// 
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.


#ifndef QSCILEXER_H
#define QSCILEXER_H

#include <QColor>
#include <QFont>
#include <QMap>
#include <QObject>
#include <QString>

#include <Qsci/qsciglobal.h>


QT_BEGIN_NAMESPACE
class QSettings;
QT_END_NAMESPACE

class QsciAbstractAPIs;
class QsciScintilla;


//要注意与common.h的保持一致。
enum LangType {
	L_UNKNOWN=-1,L_PHP=0, L_C, L_CPP, L_CS, L_OBJC, L_JAVA, L_RC, \
	L_HTML, L_XML, L_MAKEFILE, L_PASCAL, L_BATCH, L_INI, L_ASCII, L_USER, \
	L_ASP, L_SQL, L_VB, L_JS, L_CSS, L_PERL, L_PYTHON, L_LUA, \
	L_TEX, L_FORTRAN, L_BASH, L_FLASH, L_NSIS, L_TCL, L_LISP, L_SCHEME, \
	L_ASM, L_DIFF, L_PROPS, L_PS, L_RUBY, L_SMALLTALK, L_VHDL, L_KIX, L_AU3, \
	L_CAML, L_ADA, L_VERILOG, L_MATLAB, L_HASKELL, L_INNO, L_SEARCHRESULT, \
	L_CMAKE, L_YAML, L_COBOL, L_GUI4CLI, L_D, L_POWERSHELL, L_R, L_JSP, \
	L_COFFEESCRIPT, L_JSON, L_JAVASCRIPT, L_FORTRAN_77, L_BAANC, L_SREC, \
	L_IHEX, L_TEHEX, L_SWIFT, \
	L_ASN1, L_AVS, L_BLITZBASIC, L_PUREBASIC, L_FREEBASIC, \
	L_CSOUND, L_ERLANG, L_ESCRIPT, L_FORTH, L_LATEX, \
	L_MMIXAL, L_NIM, L_NNCRONTAB, L_OSCRIPT, L_REBOL, \
	L_REGISTRY, L_RUST, L_SPICE, L_TXT2TAGS, L_VISUALPROLOG, L_TYPESCRIPT, \
	L_EDIFACT, L_MARKDOWN, L_OCTAVE, L_PO, L_POV, L_IDL, L_GO, L_GLOBAL, L_LOG, L_GCode, L_APDL, L_TXT,\
	// Don't use L_JS, use L_JAVASCRIPT instead
	// The end of enumated language type, so it should be always at the end
	L_EXTERNAL = 100, L_USER_DEFINE=200,L_USER_TXT,L_USER_CPP //用户自定义顺序与UserLangMother保存一致
};

//! \brief The QsciLexer class is an abstract class used as a base for language
//! lexers.
//!
//! A lexer scans the text breaking it up into separate language objects, e.g.
//! keywords, strings, operators.  The lexer then uses a different style to
//! draw each object.  A style is identified by a style number and has a number
//! of attributes, including colour and font.  A specific language lexer will
//! implement appropriate default styles which can be overriden by an
//! application by further sub-classing the specific language lexer.
//!
//! A lexer may provide one or more sets of words to be recognised as keywords.
//! Most lexers only provide one set, but some may support languages embedded
//! in other languages and provide several sets.
//!
//! QsciLexer provides convenience methods for saving and restoring user
//! preferences for fonts and colours.
//!
//! If you want to write a lexer for a new language then you can add it to the
//! underlying Scintilla code and implement a corresponding QsciLexer sub-class
//! to manage the different styles used.  Alternatively you can implement a
//! sub-class of QsciLexerCustom.
class QSCINTILLA_EXPORT QsciLexer : public QObject
{
    Q_OBJECT

public:
	struct StyleData {
		QFont font;
		QColor color;
		QColor paper;
		bool eol_fill;
	};
    //! Construct a QsciLexer with parent \a parent.  \a parent is typically
    //! the QsciScintilla instance.
    QsciLexer(QObject *parent = 0);

    //! Destroy the QSciLexer.
    virtual ~QsciLexer();

	static void setProLangeDefaultFont(const QFont & font);

    //! Returns the name of the language.  It must be re-implemented by a
    //! sub-class.
    virtual const char *language() const = 0;

    //! Returns the name of the lexer.  If 0 is returned then the lexer's
    //! numeric identifier is used.  The default implementation returns 0.
    //!
    //! \sa lexerId()
    virtual const char *lexer() const;

	void setLexerId(int id);

	void setLexerTag(QString tag);

	//对外的显示名称。比如html可以解析asp，使用html的lexer()，但是对外tag是asp
	QString lexerTag();

    //! Returns the identifier (i.e. a QsciScintillaBase::SCLEX_* value) of the
    //! lexer.  This is only used if lexer() returns 0.  The default
    //! implementation returns QsciScintillaBase::SCLEX_CONTAINER.
    //!
    //! \sa lexer()
    virtual int lexerId() const;

    //! Returns the current API set or 0 if there isn't one.
    //!
    //! \sa setAPIs()
    QsciAbstractAPIs *apis() const;

    //! Returns the characters that can fill up auto-completion.
    virtual const char *autoCompletionFillups() const;

    //! Returns the list of character sequences that can separate
    //! auto-completion words.  The first in the list is assumed to be the
    //! sequence used to separate words in the lexer's API files.
    virtual QStringList autoCompletionWordSeparators() const;

    //! Returns the auto-indentation style.  The default is 0 if the
    //! language is block structured, or QsciScintilla::AiMaintain if not.
    //!
    //! \sa setAutoIndentStyle(), QsciScintilla::AiMaintain,
    //! QsciScintilla::AiOpening, QsciScintilla::AiClosing
    int autoIndentStyle();

    //! Returns a space separated list of words or characters in a particular
    //! style that define the end of a block for auto-indentation.  The style
    //! is returned via \a style.
    virtual const char *blockEnd(int *style = 0) const;

    //! Returns the number of lines prior to the current one when determining
    //! the scope of a block when auto-indenting.
    virtual int blockLookback() const;

    //! Returns a space separated list of words or characters in a particular
    //! style that define the start of a block for auto-indentation.  The style
    //! is returned via \a style.
    virtual const char *blockStart(int *style = 0) const;

    //! Returns a space separated list of keywords in a particular style that
    //! define the start of a block for auto-indentation.  The style is
    //! returned via \a style.
    virtual const char *blockStartKeyword(int *style = 0) const;

    //! Returns the style used for braces for brace matching.
    virtual int braceStyle() const;

    //! Returns true if the language is case sensitive.  The default is true.
    virtual bool caseSensitive() const;

    //! Returns the foreground colour of the text for style number \a style.
    //! The default colour is that returned by defaultColor().
    //!
    //! \sa defaultColor(), paper()
    virtual QColor color(int style) const;

    //! Returns the end-of-line for style number \a style.  The default is
    //! false.
    virtual bool eolFill(int style) const;

    //! Returns the font for style number \a style.  The default font is
    //! that returned by defaultFont().
    //!
    //! \sa defaultFont()
    virtual QFont font(int style) const;

    //! Returns the view used for indentation guides.
    virtual int indentationGuideView() const;

    //! Returns the set of keywords for the keyword set \a set recognised
    //! by the lexer as a space separated string.  Keyword sets are numbered
    //! from 1.  0 is returned if there is no such set.
    virtual const char *keywords(int set);

	void setIsUserDefineKeywords(bool isUserDefine=false); //使用用户自定义的关键字

	const char* getUserDefineKeywords();//获取用户自定义关键字

    //! Returns the number of the style used for whitespace.  The default
    //! implementation returns 0 which is the convention adopted by most
    //! lexers.
    virtual int defaultStyle() const;

    //! Returns the descriptive name for style number \a style.  For a valid
    //! style number for this language a non-empty QString must be returned.
    //! If the style number is invalid then an empty QString must be returned.
    //! This is intended to be used in user preference dialogs.
    virtual QString description(int style) const = 0;

    //! Returns the background colour of the text for style number
    //! \a style.
    //!
    //! \sa defaultPaper(), color()
    virtual QColor paper(int style) const;

    //! Returns the default text colour.
    //!
    //! \sa setDefaultColor()
    QColor defaultColor() const;

    //! Returns the default text colour for style number \a style.
    virtual QColor defaultColor(int style) const;

    //! Returns the default end-of-line for style number \a style.  The default
    //! is false.
    virtual bool defaultEolFill(int style) const;

    //! Returns the default font.
    //!
    //! \sa setDefaultFont()
    QFont defaultFont() const;

    //! Returns the default font for style number \a style.
    virtual QFont defaultFont(int style) const;

    //! Returns the default paper colour.
    //!
    //! \sa setDefaultPaper()
    QColor defaultPaper() const;

    //! Returns the default paper colour for style number \a style.
    virtual QColor defaultPaper(int style) const;

    //! Returns the QsciScintilla instance that the lexer is currently attached
    //! to or 0 if it is unattached.
    QsciScintilla *editor() const {return attached_editor;}

    //! The current set of APIs is set to \a apis.  If \a apis is 0 then any
    //! existing APIs for this lexer are removed.
    //!
    //! \sa apis()
    void setAPIs(QsciAbstractAPIs *apis);

    //! The default text colour is set to \a c.
    //!
    //! \sa defaultColor(), color()
    void setDefaultColor(const QColor &c);

    //! The default font is set to \a f.
    //!
    //! \sa defaultFont(), font()
    void setDefaultFont(const QFont &f);

    //! The default paper colour is set to \a c.
    //!
    //! \sa defaultPaper(), paper()
    void setDefaultPaper(const QColor &c);

    //! \internal Set the QsciScintilla instance that the lexer is attached to.
    virtual void setEditor(QsciScintilla *editor);

    //! The colour, paper, font and end-of-line for each style number, and
    //! all lexer specific properties are read from the settings \a qs.
    //! \a prefix is prepended to the key of each entry.  true is returned
    //! if there was no error.
    //!
    //! \sa writeSettings(), QsciScintilla::setLexer()
    bool readSettings(QSettings &qs,const char *prefix = "/Scintilla");

    //! Causes all properties to be refreshed by emitting the
    //! propertyChanged() signal as required.
    virtual void refreshProperties();

    //! Returns the number of style bits needed by the lexer.  Normally this
    //! should only be re-implemented by custom lexers.  This is deprecated and
    //! no longer has any effect.
    virtual int styleBitsNeeded() const;

    //! Returns the string of characters that comprise a word.  The default is
    //! 0 which implies the upper and lower case alphabetic characters and
    //! underscore.
    virtual const char *wordCharacters() const;

    //! The colour, paper, font and end-of-line for each style number, and
    //! all lexer specific properties are written to the settings \a qs.
    //! \a prefix is prepended to the key of each entry.  true is returned
    //! if there was no error.
    //!
    //! \sa readSettings()
    bool writeSettings(QSettings &qs,
               const char *prefix = "/Scintilla") const;

	StyleData &styleData(int style) const;
    StyleData& setThemesDefaultStyleData(int style) const;

	void resetStyleDefaults();

    QByteArray getCommentLineSymbol();
    void setCommentLineSymbol(QByteArray comment);

    QByteArray getCommentStart();
    QByteArray getCommentEnd();

    void setCommentStart(QByteArray commentStart);
    void setCommentEnd(QByteArray commentEnd);

    static void setCurThemes(int themesId);

public slots:
    //! The auto-indentation style is set to \a autoindentstyle.
    //!
    //! \sa autoIndentStyle(), QsciScintilla::AiMaintain,
    //! QsciScintilla::AiOpening, QsciScintilla::AiClosing
    virtual void setAutoIndentStyle(int autoindentstyle);

    //! The foreground colour for style number \a style is set to \a c.  If
    //! \a style is -1 then the colour is set for all styles.
    virtual void setColor(const QColor &c,int style = -1);

    //! The end-of-line fill for style number \a style is set to
    //! \a eoffill.  If \a style is -1 then the fill is set for all styles.
    virtual void setEolFill(bool eoffill,int style = -1);

    //! The font for style number \a style is set to \a f.  If \a style is
    //! -1 then the font is set for all styles.
    virtual void setFont(const QFont &f,int style = -1);

    //! The background colour for style number \a style is set to \a c.  If
    //! \a style is -1 then the colour is set for all styles.
    virtual void setPaper(const QColor &c,int style = -1);

signals:
    //! This signal is emitted when the foreground colour of style number
    //! \a style has changed.  The new colour is \a c.
    void colorChanged(const QColor &c,int style);

    //! This signal is emitted when the end-of-file fill of style number
    //! \a style has changed.  The new fill is \a eolfilled.
    void eolFillChanged(bool eolfilled,int style);

    //! This signal is emitted when the font of style number \a style has
    //! changed.  The new font is \a f.
    void fontChanged(const QFont &f,int style);

    //! This signal is emitted when the background colour of style number
    //! \a style has changed.  The new colour is \a c.
    void paperChanged(const QColor &c,int style);

    //! This signal is emitted when the value of the lexer property \a prop
    //! needs to be changed.  The new value is \a val.
    void propertyChanged(const char *prop, const char *val);

protected:
    //! The lexer's properties are read from the settings \a qs.  \a prefix
    //! (which has a trailing '/') should be used as a prefix to the key of
    //! each setting.  true is returned if there is no error.
    //!
    virtual bool readProperties(QSettings &qs,const QString &prefix);

    //! The lexer's properties are written to the settings \a qs.
    //! \a prefix (which has a trailing '/') should be used as a prefix to
    //! the key of each setting.  true is returned if there is no error.
    //!
    virtual bool writeProperties(QSettings &qs,const QString &prefix) const;

	bool m_isUserDefineKeyword; //是否使用用户自定义关键字。默认false

	QByteArray m_userDefineKeyword;//用户自定义的关键字

    QByteArray m_commentSymbol;
    QByteArray m_commentStart;
    QByteArray m_commentEnd;

    //当前主题id
    static int m_themesId;

private:


    struct StyleDataMap {
        bool style_data_set;
        QMap<int, StyleData> style_data;
    };

    StyleDataMap *style_map;

    int autoIndStyle;
    QFont defFont;
    QColor defColor;
    QColor defPaper;
    QsciAbstractAPIs *apiSet;
    QsciScintilla *attached_editor;

	QString m_tagName;


    void setStyleDefaults() const;

    QsciLexer(const QsciLexer &);
    QsciLexer &operator=(const QsciLexer &);

	int m_lexerId;

public:
	static int s_defaultFontSize;
	static QFont s_defaultLangFont;
};

#endif
