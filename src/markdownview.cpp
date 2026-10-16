#include "markdownview.h"

#include <QTextDocument>
#include <QTextEdit>
#include <QPalette>
#include <QColor>
#include <QFont>
#include <QStringList>
#include <QApplication>

// ============================================================
// 预览用样式表
//
// 为什么需要它：
//   MdRenderer 只产出 HTML 片段，没有任何样式。
//   直接 setHtml() 会用 QTextEdit 的默认样式渲染，
//   标题没有层级、代码块没有底色、表格没有边框 —— 很丑。
//
// 为什么用 setDefaultStyleSheet 而不是在 HTML 里写 <style>：
//   QTextDocument 对 HTML 中 <style> 块的支持不完整，
//   setDefaultStyleSheet() 是 Qt 官方推荐做法，稳定生效。
//
// 注意 Qt 的富文本引擎只支持 CSS 的一个子集：
//   border-radius / overflow / transform 等无效，别写。
//   有效的主要有 color / background-color / font-* /
//   margin / padding / border(表格与块) / text-decoration。
// ============================================================
static QString buildPreviewCss(const QWidget* w)
{
	bool dark = false;
	if (w != nullptr) {
		const QColor bg = w->palette().color(QPalette::Base);
		if (bg.isValid())
			dark = (bg.lightness() < 128);
	}

	//深色配色以工具窗底色 #2D2D2D 为锚点（见 styleset.cpp 的 toolWinDeepStyle），
	//代码块/表头在其之上稍微提亮、边框用 #555555，层次才拉得开；
	//原来代码块用 #252526、引用用 #2a2a2b，都比页面底色更暗，看着像"凹坑"。
	const QString fg      = dark ? QStringLiteral("#e0e0e0") : QStringLiteral("#1e1e1e");
	const QString headFg  = dark ? QStringLiteral("#ffffff") : QStringLiteral("#1e1e1e");
	const QString muted   = dark ? QStringLiteral("#a0a0a0") : QStringLiteral("#6a6a6a");
	const QString pageBg  = dark ? QStringLiteral("#2d2d2d") : QStringLiteral("#ffffff");
	const QString codeBg  = dark ? QStringLiteral("#3a3a3a") : QStringLiteral("#f5f5f5");
	const QString codeFg = dark ? QStringLiteral("#e6e6e6") : QStringLiteral("#2b2b2b");
	const QString border  = dark ? QStringLiteral("#555555") : QStringLiteral("#d8d8d8");
	const QString accent = dark ? QStringLiteral("#4fc1ff") : QStringLiteral("#0a66c2");
	const QString quoteBg = dark ? QStringLiteral("#343434") : QStringLiteral("#f7f7f7");

	//深色下再补几处层次：标题下划线、代码块描边、引用左侧色条。浅色下都为空串，保持原有观感。
	const QString hDeco     = dark ? QStringLiteral(" padding-bottom: 4px; border-bottom: 1px solid %1;").arg(border) : QString();
	const QString preDeco   = dark ? QStringLiteral(" border: 1px solid %1; padding: 6px 8px;").arg(border) : QString();
	const QString quoteDeco = dark ? QStringLiteral(" border-left: 3px solid %1; padding: 2px 0 2px 10px;").arg(accent) : QString();

	QStringList c;
	//深色下显式给出页面底色，避免宿主窗口没套深色样式表时预览区留白。
	c << (dark
	      ? QStringLiteral("body { color: %1; background-color: %2; font-size: 11pt; }").arg(fg, pageBg)
	      : QStringLiteral("body { color: %1; font-size: 11pt; }").arg(fg));
	c << QStringLiteral("p { margin: 8px 0; }");
	c << QStringLiteral("h1 { color: %1; font-size: 21pt; font-weight: bold; margin: 16px 0 8px 0;%2 }").arg(headFg, hDeco);
	c << QStringLiteral("h2 { color: %1; font-size: 17pt; font-weight: bold; margin: 14px 0 7px 0;%2 }").arg(headFg, hDeco);
	c << QStringLiteral("h3 { color: %1; font-size: 14pt; font-weight: bold; margin: 12px 0 6px 0; }").arg(headFg);
	c << QStringLiteral("h4 { color: %1; font-size: 12pt; font-weight: bold; margin: 10px 0 5px 0; }").arg(headFg);
	c << QStringLiteral("h5 { color: %1; font-size: 11pt; font-weight: bold; margin: 9px 0 4px 0; }").arg(headFg);
	c << QStringLiteral("h6 { color: %1; font-size: 11pt; font-weight: bold; margin: 9px 0 4px 0; }").arg(muted);
	c << QStringLiteral("code { font-family: 'Consolas', monospace; font-size: 10pt;"
	                    " background-color: %1; color: %2; }").arg(codeBg, codeFg);
	c << QStringLiteral("pre { font-family: 'Consolas', monospace; font-size: 10pt;"
	                    " background-color: %1; color: %2;%3 margin: 10px 0; }").arg(codeBg, codeFg, preDeco);
	c << QStringLiteral("blockquote { color: %1; background-color: %2;%3 margin: 10px 0 10px 12px; }")
	        .arg(muted, quoteBg, quoteDeco);
	c << QStringLiteral("ul { margin: 6px 0 6px 22px; }");
	c << QStringLiteral("ol { margin: 6px 0 6px 22px; }");
	c << QStringLiteral("li { margin: 3px 0; }");
	c << QStringLiteral("a { color: %1; text-decoration: none; }").arg(accent);
	//hr 用 color 在 Qt 富文本里基本不生效，改用上边框。
	c << (dark
	      ? QStringLiteral("hr { border: none; border-top: 1px solid %1; margin: 14px 0; }").arg(border)
	      : QStringLiteral("hr { color: %1; }").arg(border));
	c << QStringLiteral("table { border-collapse: collapse; margin: 10px 0; }");
	c << QStringLiteral("th { color: %1; border: 1px solid %2; padding: 5px 9px; background-color: %3;"
	                    " font-weight: bold; }").arg(headFg, border, codeBg);
	c << QStringLiteral("td { border: 1px solid %1; padding: 5px 9px; }").arg(border);
	c << QStringLiteral("img { }");
	c << QStringLiteral(".ndd-empty { color: %1; }").arg(muted);
	return c.join(QStringLiteral("\n"));
}


MarkdownView::MarkdownView(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	// 预览区留出内边距，别让文字贴着窗口边缘
	ui.textEdit->setLineWrapMode(QTextEdit::WidgetWidth);
	ui.textEdit->document()->setDocumentMargin(16);
	ui.textEdit->setReadOnly(true);
}

MarkdownView::~MarkdownView()
{

}


void MarkdownView::viewMarkdown(QString& text)
{
#if (QT_VERSION < QT_VERSION_CHECK(5, 14, 0))
	QString tips = QString(
			"NOTE: Your Qt version is lower than 5.14, so you can't preview Markdown for the time being."
			"\n"
			"\n"
			"%1").arg(text);
	ui.textEdit->setPlainText(tips);
#else
	ui.textEdit->document()->setDefaultStyleSheet(buildPreviewCss(ui.textEdit));
	ui.textEdit->setMarkdown(text);
#endif
}

// 以 HTML 呈现：内容由自研渲染器 MdRenderer 生成
// （支持 GFM 表格、任务列表等），这里只负责套上样式。
void MarkdownView::viewHtml(const QString& html)
{
	// 样式通过 setDefaultStyleSheet 注入，而不是在 HTML 里写 <style>：
	// QTextDocument 对 <style> 块支持不完整，这个方式最稳。
	ui.textEdit->document()->setDefaultStyleSheet(buildPreviewCss(ui.textEdit));
	ui.textEdit->setHtml(html);
}
