#pragma once

#include <QObject>
#include <QLabel>


class CompareDirs;
class CompareWin;
class chatSocket;

#if defined(Q_OS_MAC)
const int VERIFY_CODE = 12;
#elif defined(Q_OS_UNIX)
const int VERIFY_CODE = 9;
#elif defined(Q_OS_WIN)
const int VERIFY_CODE = 8; //验证注册.windows 8 linux 9 MAC 12 (10,11给了我的写作）
#endif
static short version_num = 23;


//这个类完全简化了，只保护网络验证的功能。其余UI操作放入到ccnotepad中去了

class NetRegister : public QObject
{
	Q_OBJECT

public:
	NetRegister(QObject *parent = Q_NULLPTR);
	virtual ~NetRegister();
	static QString getLocalFirstMac();

private:
	static QStringList getLocalMac();
	void quit();
};
