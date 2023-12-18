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

LexicalClass lexicalClasses[] = {
	// Lexer Python SCLEX_PYTHON SCE_P_:
	0, "SCE_TXT_DEFAULT", "default", "utf8 char",
	1, "SCE_TXT_ASCII", "Ascii", "Ascii",
	2, "SCE_TXT_KEYWORD", "keyword", "keyword",
};

enum literalsAllowed { litNone = 0, litU = 1, litB = 2, litF = 4 };

// Options used for LexerPython
struct OptionsTxt {
	bool ascii;
	bool utf8;
	bool keyword;

	OptionsTxt() {
		ascii = true;
		utf8 = true;
		keyword = true;
	}

};


struct OptionSetTxt : public OptionSet<OptionsTxt> {
	OptionSetTxt() {
		DefineProperty("lexer.txt.utf8string", &OptionsTxt::utf8,
			"text Unicode string");

		DefineProperty("lexer.txt.asciistring", &OptionsTxt::ascii,
			"text literals ascii string");

		DefineProperty("lexer.txt.keyword", &OptionsTxt::keyword,
			"text keyword");
	}
};

class LexTXT :public DefaultLexer
{
	WordList keywords;
	//SubStyles subStyles;
	OptionsTxt options;
	OptionSetTxt osTxt;
	bool isUserDefMode;
public:
	explicit LexTXT() :
		DefaultLexer(lexicalClasses, ELEMENTS(lexicalClasses)), isUserDefMode(false)/*,
		subStyles(styleSubable, 0x80, 0x40, 0)*/ {
	}
	virtual ~LexTXT() {}

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

	//int SCI_METHOD AllocateSubStyles(int styleBase, int numberStyles) override {
	//	return subStyles.Allocate(styleBase, numberStyles);
	//}
	//int SCI_METHOD SubStylesStart(int styleBase) override {
	//	return subStyles.Start(styleBase);
	//}
	//int SCI_METHOD SubStylesLength(int styleBase) override {
	//	return subStyles.Length(styleBase);
	//}
	//int SCI_METHOD StyleFromSubStyle(int subStyle) override {
	//	const int styleBase = subStyles.BaseStyle(subStyle);
	//	return styleBase;
	//}
	int SCI_METHOD PrimaryStyleFromStyle(int style) override {
		return style;
	}
	//void SCI_METHOD FreeSubStyles() override {
	//	subStyles.Free();
	//}
	//void SCI_METHOD SetIdentifiers(int style, const char *identifiers) override {
	//	subStyles.SetIdentifiers(style, identifiers);
	//}
	int SCI_METHOD DistanceToSecondaryStyles() override {
		return 0;
	}
	//const char *SCI_METHOD GetSubStyleBases() override {
	//	return styleSubable;
	//}

	static ILexer *LexerFactoryTxt() {
		return new LexTXT();
	}
};

Sci_Position SCI_METHOD LexTXT::PropertySet(const char *key, const char *val) {
	if (osTxt.PropertySet(&options, key, val)) {
		return 0;
	}
	return -1;
}

Sci_Position SCI_METHOD LexTXT::WordListSet(int n, const char *wl) {

	WordList *wordListN = 0;
	switch (n) {
	case 0:
		wordListN = &keywords;
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
		isUserDefMode = (wordListN->Length() > 0);
	}
	return firstModification;
}

const int indicatorWhitespace = 1;

//inline bool IsAWordChar(int ch, bool unicodeIdentifiers) {
//	if (ch < 0x80)
//		return (isalnum(ch) || ch == '.' || ch == '_');
//
//	if (!unicodeIdentifiers)
//		return false;
//
//	// Python uses the XID_Continue set from unicode data
//	return IsXidContinue(ch);
//}

inline bool IsAAsciiChar(int ch) {
	return (ch < 0x80);
}

inline bool IsAWordStart(int ch, bool unicodeIdentifiers) {
	if (ch < 0x80)
		return (isalpha(ch) || ch == '_');

	if (!unicodeIdentifiers)
		return false;

	// Python uses the XID_Start set from unicode data
	return IsXidStart(ch);
}

//是否是单词分割符号，对于ascii中的数字和字母以外的字符，比如 . @ \t 等，都作为一个单词的分割符号。
inline bool IsWordSplitChar(int ch) noexcept {
	//return ch == ' ' || ch == '\t';
	return (ch < 0x80) && !isalnum(ch);
}

inline bool IsLineEol(int ch) noexcept {
	return ch == '\n' || ch == '\r';
}

//只识别中文和英文两种单词的状态
void SCI_METHOD LexTXT::Lex(Sci_PositionU startPos, Sci_Position length, int initStyle, IDocument *pAccess) {
	Accessor styler(pAccess, NULL);

	const Sci_Position endPos = startPos + length;

	//const WordClassifier& classifierIdentifiers = subStyles.Classifier(SCE_TXT_IDENTIFIER);

	StyleContext sc(startPos, endPos - startPos, initStyle, styler);

	Sci_Position startIndicator = sc.currentPos;

	if (!isUserDefMode)
	{
		for (; sc.More();) {
			// Check for a new state starting character
			if (sc.state == SCE_TXT_DEFAULT)
			{
				if (IsAAsciiChar(sc.ch))
				{
					sc.SetState(SCE_TXT_ASCII);
				}
			}
			else if (sc.state == SCE_TXT_ASCII)
			{
				if (!IsAAsciiChar(sc.ch))
				{
					sc.SetState(SCE_TXT_DEFAULT);
				}
			}
			sc.Forward();
		}
	}
	else
	{
		for (; sc.More();) {
			// Check for a new state starting character
			if (sc.state == SCE_TXT_DEFAULT)
			{
				if (IsAAsciiChar(sc.ch))
				{
					//如果要识别关键字，必须要跳过当前空白字符。否则 中文空格后面第一个关键字无法高亮
					if (IsWordSplitChar(sc.ch) && IsAWordStart(sc.chNext, false))
					{
						//这里切换的时候，ForwardSetState跳过当前空白字符
						sc.ForwardSetState(SCE_TXT_ASCII);
						continue;
					}
					sc.SetState(SCE_TXT_ASCII);
				}
			}
			else if (sc.state == SCE_TXT_ASCII)
			{
				if (!IsAAsciiChar(sc.ch))
				{
					sc.SetState(SCE_TXT_DEFAULT);
				}
				else //存在自定义关键字，说明是用户自定义语法。
				{
					//下面是识别关键字的逻辑。在英文逻辑下，对单词进行识别。
					//所谓单词，一定是包围在前后空格或\t中的英文单词
					if ((IsWordSplitChar(sc.ch) /*|| IsLineEol(sc.ch)*/) && isalpha(sc.chPrev))
					{
						char s[512];
						sc.GetCurrent(s, sizeof(s));
						if (keywords.InList(s))
						{
							sc.ChangeState(SCE_TXT_KEYWORD);
						}
						sc.SetState(SCE_TXT_ASCII);
					}


					if (IsWordSplitChar(sc.ch) && IsAWordStart(sc.chNext, false))
					{
						//这里切换的时候，ForwardSetState跳过当前空白字符
						sc.ForwardSetState(SCE_TXT_ASCII);
						continue;
						//sc.SetState(SCE_TXT_ASCII);
					}

				}
			}
			sc.Forward();
		}
	}
	
	//最后一段不能遗漏，也需要识别
	sc.SetState(SCE_TXT_DEFAULT);

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
void SCI_METHOD LexTXT::Fold(Sci_PositionU startPos, Sci_Position length, int /*initStyle - unused*/, IDocument *pAccess) {
	return;
}

static const char *const txtWordListDesc[] = {
	0
};

LexerModule lmTxt(SCLEX_TXT, LexTXT::LexerFactoryTxt, "txt", txtWordListDesc);
