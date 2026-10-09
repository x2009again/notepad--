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

	const QString fg     = dark ? QStringLiteral("#dcdcdc") : QStringLiteral("#1e1e1e");
	const QString muted  = dark ? QStringLiteral("#9a9a9a") : QStringLiteral("#6a6a6a");
	const QString codeBg = dark ? QStringLiteral("#252526") : QStringLiteral("#f5f5f5");
	const QString codeFg = dark ? QStringLiteral("#e6e6e6") : QStringLiteral("#2b2b2b");
	const QString border = dark ? QStringLiteral("#454545") : QStringLiteral("#d8d8d8");
	const QString accent = dark ? QStringLiteral("#4fc1ff") : QStringLiteral("#0a66c2");
	const QString quoteBg = dark ? QStringLiteral("#2a2a2b") : QStringLiteral("#f7f7f7");

	QStringList c;
	c << QStringLiteral("body { color: %1; font-size: 11pt; }").arg(fg);
	c << QStringLiteral("p { margin: 8px 0; }");
	c << QStringLiteral("h1 { font-size: 21pt; font-weight: bold; margin: 16px 0 8px 0; }");
	c << QStringLiteral("h2 { font-size: 17pt; font-weight: bold; margin: 14px 0 7px 0; }");
	c << QStringLiteral("h3 { font-size: 14pt; font-weight: bold; margin: 12px 0 6px 0; }");
	c << QStringLiteral("h4 { font-size: 12pt; font-weight: bold; margin: 10px 0 5px 0; }");
	c << QStringLiteral("h5 { font-size: 11pt; font-weight: bold; margin: 9px 0 4px 0; }");
	c << QStringLiteral("h6 { font-size: 11pt; font-weight: bold; color: %1; margin: 9px 0 4px 0; }").arg(muted);
	c << QStringLiteral("code { font-family: 'Consolas', monospace; font-size: 10pt;"
	                    " background-color: %1; color: %2; }").arg(codeBg, codeFg);
	c << QStringLiteral("pre { font-family: 'Consolas', monospace; font-size: 10pt;"
	                    " background-color: %1; color: %2; margin: 10px 0; }").arg(codeBg, codeFg);
	c << QStringLiteral("blockquote { color: %1; background-color: %2; margin: 10px 0 10px 12px; }")
	        .arg(muted, quoteBg);
	c << QStringLiteral("ul { margin: 6px 0 6px 22px; }");
	c << QStringLiteral("ol { margin: 6px 0 6px 22px; }");
	c << QStringLiteral("li { margin: 3px 0; }");
	c << QStringLiteral("a { color: %1; text-decoration: none; }").arg(accent);
	c << QStringLiteral("hr { color: %1; }").arg(border);
	c << QStringLiteral("table { border-collapse: collapse; margin: 10px 0; }");
	c << QStringLiteral("th { border: 1px solid %1; padding: 5px 9px; background-color: %2;"
	                    " font-weight: bold; }").arg(border, codeBg);
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
