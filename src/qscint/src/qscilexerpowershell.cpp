#include "Qsci/qscilexerpowershell.h"
#include "Qsci/qsciscintillabase.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>


QsciLexerPowerShell::QsciLexerPowerShell(QObject *parent)
	: QsciLexer(parent)
{
	setLexerId(L_POWERSHELL);
	m_commentSymbol = ";";
}

QsciLexerPowerShell::~QsciLexerPowerShell()
{
}

// Returns the language name.
const char* QsciLexerPowerShell::language() const
{
	return "POWERSHELL";
}

// Returns the lexer name.
const char *QsciLexerPowerShell::lexer() const
{
	return "powershell";
}


// Returns the foreground colour of the text for a style.
QColor QsciLexerPowerShell::defaultColor(int style) const
{
	switch (style)
	{
	case COMMENT:
		return QColor(0, 128, 0);

	case CHARACTER:
	case STRING:
		return QColor(128, 128, 128);

	case NUMBER:
		return QColor(255, 128, 0);

	case OPERATOR:
		return QColor(0, 0, 128);

	case KEYWORD:
		return QColor(0, 0, 255);

	case CMDLET:
		return QColor(128, 0, 255);

	case ALIAS:
		return QColor(0, 128, 255);

	case COMMENTSTREAM:
		return QColor(0, 128, 128);

	case HERE_STRING:
	case HERE_CHARACTER:
	case COMMENTDOCKEYWORD:
		return QColor(0, 128, 128);

	default:
		break;

	}

	return QsciLexer::defaultColor(style);
}

// Returns the end-of-line fill for a style.
bool QsciLexerPowerShell::defaultEolFill(int style) const
{

	return QsciLexer::defaultEolFill(style);
}

// Returns the font of the text for a style.
QFont QsciLexerPowerShell::defaultFont(int style) const
{
	switch (style)
	{
	case VARIABLE:
	case OPERATOR:
	case KEYWORD:
	case COMMENTDOCKEYWORD:
#if defined(Q_OS_MAC)
	{
		QFont f("Courier New", 18);
		f.setBold(true);
		return f;
	}
#else
	{
		QFont f(s_defaultLangFont);
		//f.setBold(true);
		return f;
	}
#endif
	default:
		break;
	}
	return QsciLexer::s_defaultLangFont;
}


// Returns the set of keywords.
const char* QsciLexerPowerShell::keywords(int set)
{
	if (set == 1)
		return "break continue do else elseif filter for foreach function if in return switch until where while";

	if (set == 2)
		return "add-content add-history add-member add-pssnapin clear-content clear-item clear-itemproperty clear-variable "
		"compare-object convertfrom-securestring convert-path convertto-html convertto-securestring copy-item copy-itemproperty "
		"export-alias export-clixml export-console export-csv foreach-object format-custom format-list format-table format-wide "
		"get-acl get-alias get-authenticodesignature get-childitem get-command get-content get-credential get-culture get-date "
		"get-eventlog get-executionpolicy get-help get-history get-host get-item get-itemproperty get-location get-member "
		"get-pfxcertificate get-process get-psdrive get-psprovider get-pssnapin get-service get-tracesource get-uiculture "
		"get-unique get-variable get-wmiobject group-object import-alias import-clixml import-csv invoke-expression invoke-history "
		"invoke-item join-path measure-command measure-object move-item move-itemproperty new-alias new-item new-itemproperty "
		"new-object new-psdrive new-service new-timespan new-variable out-default out-file out-host out-null out-printer out-string "
		"pop-location push-location read-host remove-item remove-itemproperty remove-psdrive remove-pssnapin remove-variable "
		"rename-item rename-itemproperty resolve-path restart-service resume-service select-object select-string set-acl set-alias "
		"set-authenticodesignature set-content set-date set-executionpolicy set-item set-itemproperty set-location set-psdebug "
		"set-service set-tracesource set-variable sort-object split-path start-service start-sleep start-transcript stop-process "
		"stop-service stop-transcript suspend-service tee-object test-path trace-command update-formatdata update-typedata "
		"where-object write-debug write-error write-host write-output write-progress write-verbose write-warning";

	if (set == 3)
		return "ac asnp clc cli clp clv cpi cpp cvpa diff epal epcsv fc fl foreach ft fw gal gc gci gcm gdr ghy gi gl gm gp gps group gsv "
		"gsnp gu gv gwmi iex ihy ii ipal ipcsv mi mp nal ndr ni nv oh rdr ri rni rnp rp rsnp rv rvpa sal sasv sc select si sl sleep "
		"sort sp spps spsv sv tee where write cat cd clear cp h history kill lp ls mount mv popd ps pushd pwd r rm rmdir echo cls chdir " 
		"copy del dir erase move rd ren set type";

	if (set == 6)
		return "component description example externalhelp forwardhelpcategory forwardhelptargetname functionality inputs link notes outputs parameter remotehelprunspace role synopsis";

	return 0;
}

// Returns the user name of a style.
QString QsciLexerPowerShell::description(int style) const
{
	switch (style)
	{
	case DEFAULT:
		return "Default";
	case COMMENT:
		return "Comment";

	case STRING:
		return "String";

	case CHARACTER:
		return "Character";

	case NUMBER:
		return "Number";

	case VARIABLE:
		return "Variable";

	case OPERATOR:
		return "Operator";

	case KEYWORD:
		return "Keyword";

	case CMDLET:
		return "Cmdlet";

	case ALIAS:
		return "Alias";

	case FUNCTION:
		return "Function";

	case USER1:
		return "User1";

	case COMMENTSTREAM:
		return "Comment stream";

	case HERE_STRING:
		return "Here String";

	case HERE_CHARACTER:
		return "Here Character";

	case COMMENTDOCKEYWORD:
		return "Comment Doc Keyword";

	}
	return QString();
}


// Returns the background colour of the text for a style.
QColor QsciLexerPowerShell::defaultPaper(int style) const
{

	return QsciLexer::defaultPaper(style);
}
