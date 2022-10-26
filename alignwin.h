#pragma once

#include <QWidget>
#include "ui_alignwin.h"

class AlignWin : public QWidget
{
	Q_OBJECT

public:
	AlignWin(QWidget *parent = nullptr);
	~AlignWin();

signals:
	void alignLine(int type, int start, int end);

private slots:
	void slot_ok();
private:
	Ui::AlignWinClass ui;
};
