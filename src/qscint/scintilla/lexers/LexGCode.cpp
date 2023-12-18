#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <ctype.h>

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <regex>

#include "ILexer.h"
#include "Scintilla.h"
#include "SciLexer.h"

#include "StringCopy.h"
#include "WordList.h"
#include "LexAccessor.h"
#include "Accessor.h"
#include "StyleContext.h"
#include "CharacterSet.h"
#include "CharacterCategory.h"
#include "LexerModule.h"
#include "OptionSet.h"
#include "SubStyles.h"
#include "DefaultLexer.h"
#include "LexAccessor.h"

#include "UniConversion.h"


using namespace Scintilla;

//const char styleSubable[] = { SCE_P_IDENTIFIER, 0 };

//Default = 0,//中文
//Ascii = 1,//英文
//Keyword = 2, //关键字，只有以TXT为母版的

static LexicalClass lexicalClasses[] = {
	// Lexer Python SCLEX_PYTHON SCE_P_:
	0, "SCE_GCODE_DEFAULT", "default", "default",
	1, "SCE_GCODE_KEYWORD", "keyword", "keyword",
	2, "SCE_GCODE_KEYWORD1", "keyword", "keyword",
	3, "SCE_GCODE_COMMNET", "Comment", "Comment",
	4, "SCE_GCODE_POSITION", "Position", "Position",
	5, "SCE_GCODE_VELOCITY","velocity","velocity",
	
};

enum LOG_STATUS { 
	SCE_GCODE_DEFAULT=0,
	SCE_GCODE_KEYWORD, //关键字
	SCE_GCODE_KEYWORD1, //关键字
	SCE_GCODE_COMMNET, //数字
	SCE_GCODE_POSITION_X, //位置 X Y Z 
	SCE_GCODE_POSITION_Y, //位置 X Y Z 
	SCE_GCODE_POSITION_Z, //位置 X Y Z 
	SCE_GCODE_OFFSET,//I J K 偏移
	SCE_GCODE_VELOCITY,//速度 F
	SCE_GCODE_TIMES,// P 次数
	SCE_GCODE_IDENTIFIER,
	SCE_GCODE_IDENTIFIER_KEYWORD,
	SCE_GCODE_IDENTIFIER_KEYWORD1,
	SCE_GCODE_IDENTIFIER_POSITION_X,
	SCE_GCODE_IDENTIFIER_POSITION_Y,
	SCE_GCODE_IDENTIFIER_POSITION_Z,
	SCE_GCODE_IDENTIFIER_OFFSET,
	SCE_GCODE_IDENTIFIER_VELOCITY,
	SCE_GCODE_IDENTIFIER_TIMES,
	SCE_GCODE_IDENTIFIER_COMMENT,
};

// Options used for LexerPython
struct OptionsTxt {
	bool num;
	bool data;
	bool keyword;

	OptionsTxt() {
		num = true;
		data = true;
		keyword = true;
	}

};

//获取行，最多获取1024个字符串
static std::string GetLineContents(LexAccessor& styler, Sci_Position start, const int len = 1024) {
	std::string lineContent;
	Sci_Position i = 0;
	char ch = styler.SafeGetCharAt(start, '\n');
	const Sci_Position endLine = styler.LineEnd(styler.GetLine(start));
	while (((start + i) < endLine) && (i < len)) {
		const char chNext = styler.SafeGetCharAt(start + i + 1, '\n');
		lineContent += ch;
		i++;
		ch = chNext;
	}
	return lineContent;
}

struct OptionSetTxt : public OptionSet<OptionsTxt> {
	OptionSetTxt() {
		DefineProperty("lexer.log.num", &OptionsTxt::num,
			"text Unicode string");

		DefineProperty("lexer.txt.keyword", &OptionsTxt::keyword,
			"text keyword");
	}
};

class LexGCode :public DefaultLexer
{
	WordList keywords;
	WordList keywords1;
	OptionsTxt options;
	OptionSetTxt osTxt;
	CharacterSet setWord;
	CharacterSet setWordStart;
public:
	explicit LexGCode() :
		DefaultLexer(lexicalClasses, ELEMENTS(lexicalClasses)),
		setWord(CharacterSet::setDigits, "GMXYZPIJKF-", 0x80, true),//字母、数字、下划线、. 单词的字符集。
		setWordStart(CharacterSet::setNone, "GMXYZPIJKF", 0x80, true)
	{
		
	}
	virtual ~LexGCode() {}

	void SCI_METHOD Release() override {
		delete this;
	}
	int SCI_METHOD Version() const override {
		return lvSubStyles;
	}
	const char *SCI_METHOD PropertyNames() override {
		return osTxt.PropertyNames();
	}
	int SCI_METHOD PropertyType(const char *name) override {
		return osTxt.PropertyType(name);
	}
	const char *SCI_METHOD DescribeProperty(const char *name) override {
		return osTxt.DescribeProperty(name);
	}
	Sci_Position SCI_METHOD PropertySet(const char *key, const char *val) override;

	const char *SCI_METHOD DescribeWordListSets() override {
		return "";
	}
	Sci_Position SCI_METHOD WordListSet(int n, const char *wl) override;

	void SCI_METHOD Lex(Sci_PositionU startPos, Sci_Position length, int initStyle, IDocument *pAccess) override;
	void SCI_METHOD Fold(Sci_PositionU startPos, Sci_Position length, int initStyle, IDocument *pAccess) override;
	void *SCI_METHOD PrivateCall(int, void *) override {
		return 0;
	}
	int SCI_METHOD LineEndTypesSupported() override {
		return SC_LINE_END_TYPE_UNICODE;
	}

	int SCI_METHOD PrimaryStyleFromStyle(int style) override {
		return style;
	}

	int SCI_METHOD DistanceToSecondaryStyles() override {
		return 0;
	}

	static ILexer * LexerFactoryGCode() {
		return new LexGCode();
	}
};

Sci_Position SCI_METHOD LexGCode::PropertySet(const char *key, const char *val) {
	if (osTxt.PropertySet(&options, key, val)) {
		return 0;
	}
	return -1;
}

Sci_Position SCI_METHOD LexGCode::WordListSet(int n, const char *wl) {

	WordList *wordListN = 0;
	switch (n) {
	case 0:
		wordListN = &keywords;
		break;
	case 1:
		wordListN = &keywords1;
		break;
	}
	Sci_Position firstModification = -1;
	if (wordListN) {
		WordList wlNew;
		wlNew.Set(wl);
		if (*wordListN != wlNew) {
			wordListN->Set(wl);
			firstModification = 0;
		}
	}
	return firstModification;
}

const int indicatorWhitespace = 1;

inline bool IsAAsciiChar(int ch) {
	return (ch < 0x80);
}

inline bool IsAWordStart(int ch, bool unicodeIdentifiers) {
	if (ch < 0x80)
		return (isalpha(ch) || ch == '_');

	if (!unicodeIdentifiers)
		return false;

	return IsXidStart(ch);
}

//是否是单词分割符号，对于ascii中的数字和字母以外的字符，比如 . @ \t 等，都作为一个单词的分割符号。
inline bool IsWordSplitChar(int ch) noexcept {
	return (ch < 0x80) && !isalnum(ch);
}

inline bool IsLineEol(int ch) noexcept {
	return ch == '\n' || ch == '\r';
}





//只识别中文和英文两种单词的状态
void SCI_METHOD LexGCode::Lex(Sci_PositionU startPos, Sci_Position length, int initStyle, IDocument *pAccess) {
	Accessor styler(pAccess, NULL);

	const Sci_Position endPos = startPos + length;

	StyleContext sc(startPos, endPos - startPos, initStyle, styler);

	Sci_Position startIndicator = sc.currentPos;

	bool isDataMatch = false;

	for (; sc.More();) {

		switch (sc.state)
		{
			case SCE_GCODE_DEFAULT:
			{
				if (!sc.atLineEnd && (sc.ch == ';'))
				{
					sc.SetState(SCE_GCODE_IDENTIFIER_COMMENT);
				}
				else if (!sc.atLineEnd && setWordStart.Contains(sc.ch)) {
					//如果不在行尾，而且当前字符是一个单词的开头标识字符，则进入标识符识别状态。
					if (sc.Match('G') && IsADigit(sc.chNext))
					{
						sc.SetState(SCE_GCODE_IDENTIFIER_KEYWORD);
						sc.Forward();
					}
					else if (sc.Match('M') && IsADigit(sc.chNext))
					{
						sc.SetState(SCE_GCODE_IDENTIFIER_KEYWORD1);
						sc.Forward();
					}
					else if (sc.Match('X')&& (IsADigit(sc.chNext) || (sc.chNext == '-')))
					{
						sc.SetState(SCE_GCODE_IDENTIFIER_POSITION_X);
						sc.Forward();
					}
					else if (sc.Match('Y') && (IsADigit(sc.chNext) || (sc.chNext == '-')))
					{
						sc.SetState(SCE_GCODE_IDENTIFIER_POSITION_Y);
						sc.Forward();
					}
					else if (sc.Match('Z') && (IsADigit(sc.chNext) || (sc.chNext == '-')))
					{
						sc.SetState(SCE_GCODE_IDENTIFIER_POSITION_Z);
						sc.Forward();
					}
					else if ((sc.Match('I') || sc.Match('J') || sc.Match('K')) && (IsADigit(sc.chNext) || (sc.chNext == '-')))
					{
						sc.SetState(SCE_GCODE_IDENTIFIER_OFFSET);
						sc.Forward();
					}
					else if (sc.Match('F') && IsADigit(sc.chNext))
					{
						sc.SetState(SCE_GCODE_IDENTIFIER_VELOCITY);
						sc.Forward();
					}
					else if (sc.Match('P') && IsADigit(sc.chNext))
					{
						sc.SetState(SCE_GCODE_IDENTIFIER_TIMES);
						sc.Forward();
					}
				}
				
			}
			break;
			case SCE_GCODE_IDENTIFIER_COMMENT:
			{
				if (sc.atLineEnd)
				{
					sc.ChangeState(SCE_GCODE_COMMNET);
					sc.SetState(SCE_GCODE_DEFAULT);
				}
			}
				break;
			case SCE_GCODE_IDENTIFIER_KEYWORD:
			case SCE_GCODE_IDENTIFIER_KEYWORD1:
			{
				if (sc.atLineStart || sc.atLineEnd || !setWord.Contains(sc.ch))
				{
					char s[1000];
					//取出当前标识符，注意标识符不会超过512，这是预计，不会有人傻到取名一个超过1000的字符串变量。
					sc.GetCurrent(s, sizeof(s));

					if (keywords.InList(s)) {
						sc.ChangeState(SCE_GCODE_KEYWORD);
					}
					else if (keywords1.InList(s)) {
						sc.ChangeState(SCE_GCODE_KEYWORD1);
					}
					else
					{
						sc.ChangeState(SCE_GCODE_DEFAULT);
					}

					sc.SetState(SCE_GCODE_DEFAULT);
				}
			}
			break;
			case SCE_GCODE_IDENTIFIER_POSITION_X:
			{
				if (sc.atLineStart || sc.atLineEnd || !setWord.Contains(sc.ch))
				{
					sc.ChangeState(SCE_GCODE_POSITION_X);
					sc.SetState(SCE_GCODE_DEFAULT);
				}
			}
			break;
			case SCE_GCODE_IDENTIFIER_POSITION_Y:
			{
				if (sc.atLineStart || sc.atLineEnd || !setWord.Contains(sc.ch))
				{
					sc.ChangeState(SCE_GCODE_POSITION_Y);
					sc.SetState(SCE_GCODE_DEFAULT);
				}
			}
			break;
			case SCE_GCODE_IDENTIFIER_POSITION_Z:
			{
				if (sc.atLineStart || sc.atLineEnd || !setWord.Contains(sc.ch))
				{
					sc.ChangeState(SCE_GCODE_POSITION_Z);
					sc.SetState(SCE_GCODE_DEFAULT);
				}
			}
			break;
			case SCE_GCODE_IDENTIFIER_OFFSET:
			{
				if (sc.atLineStart || sc.atLineEnd || !setWord.Contains(sc.ch))
				{
					sc.ChangeState(SCE_GCODE_OFFSET);
					sc.SetState(SCE_GCODE_DEFAULT);
				}
			}
			break;
			case SCE_GCODE_IDENTIFIER_VELOCITY:
			{
				if (sc.atLineStart || sc.atLineEnd || !setWord.Contains(sc.ch))
				{
					sc.ChangeState(SCE_GCODE_VELOCITY);
					sc.SetState(SCE_GCODE_DEFAULT);
				}
			}
			break;
			case SCE_GCODE_IDENTIFIER_TIMES:
			{
				if (sc.atLineStart || sc.atLineEnd || !setWord.Contains(sc.ch))
				{
					sc.ChangeState(SCE_GCODE_TIMES);
					sc.SetState(SCE_GCODE_DEFAULT);
				}
			}
				break;
		}

		sc.Forward();
	}
	
	styler.IndicatorFill(startIndicator, sc.currentPos, indicatorWhitespace, 0);
	sc.Complete();

}

static bool IsCommentLine(Sci_Position line, Accessor &styler) {
	Sci_Position pos = styler.LineStart(line);
	const Sci_Position eol_pos = styler.LineStart(line + 1) - 1;
	for (Sci_Position i = pos; i < eol_pos; i++) {
		const char ch = styler[i];
		if (ch == '#')
			return true;
		else if (ch != ' ' && ch != '\t')
			return false;
	}
	return false;
}

static bool IsQuoteLine(Sci_Position line, const Accessor &styler) {
	const int style = styler.StyleAt(styler.LineStart(line)) & 31;
	return false;
}


//不处理任何折叠
void SCI_METHOD LexGCode::Fold(Sci_PositionU startPos, Sci_Position length, int /*initStyle - unused*/, IDocument *pAccess) {
	return;
}

static const char *const txtWordListDesc[] = {
	0
};

LexerModule lmGCode(SCLEX_GCode, LexGCode::LexerFactoryGCode, "gcode", txtWordListDesc);
