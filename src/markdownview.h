#pragma once

#include <QMainWindow>
#include "ui_markdownview.h"

class MarkdownView : public QMainWindow
{
	Q_OBJECT

public:
	MarkdownView(QWidget *parent = nullptr);
	~MarkdownView();
	void viewMarkdown(QString& text);

	// 以 HTML 呈现。
	//
	// 为什么需要这个入口：
	//   viewMarkdown() 走 QTextDocument::setMarkdown()，
	//   渲染能力受 Qt 内置实现限制（GFM 表格、任务列表支持有限）。
	//   本工程自带自研渲染器 MdRenderer（src/mdrenderer.*），
	//   覆盖 GFM 表格 / 任务列表 / 嵌套列表等，先转成 HTML
	//   再交给这里显示，效果更好。
	void viewHtml(const QString& html);

private:
	Ui::MarkdownViewClass ui;
};
