#pragma once
#include <QObject>
#include <QWidget>
#include <QPointer>
#include "qttestclass.h"

class QMenu;

class InstanceObj :public QObject
{
public:
	//外面Ndd释放时，会自动释放该对象。
	InstanceObj(QWidget* pNotepad);
	~InstanceObj();

public slots:
	void doMainWork();

public:
	QWidget* m_pNotepad;
	QPointer<QtTestClass> m_pMainToolWin;
private:
	InstanceObj(const InstanceObj& other) = delete;
	InstanceObj& operator=(const InstanceObj& other) = delete;
};