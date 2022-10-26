#include "netregister.h"
#include "jsondeploy.h"

#include "donate.h"
#include "doctypelistview.h"
#include "rcglobal.h"

#include <QNetworkInterface>
#include <QFontDatabase>
#include <qmessagebox.h>
#include <QTimer>

//5 1.3
//6 1.4 20211027日
#define uos
#if defined(Q_OS_MAC)
    #undef uos
#endif

#if defined(Q_OS_WIN)
#undef uos
#endif

#ifdef uos
QString loadFontFromFile(QString path,int code)
{
    QString font;
    static QMap<int,QString> codelist;

    if(codelist.contains(code))
    {
        return codelist.value(code);
    }

    int loadedFontID = QFontDatabase::addApplicationFont(path);
    QStringList loadedFontFamilies = QFontDatabase::applicationFontFamilies(loadedFontID);
    if(!loadedFontFamilies.empty())
    {
        if ((code == 0) && loadedFontFamilies.size() >=7)
        {
            font = loadedFontFamilies.at(6);
        }
        else
        {
            font = loadedFontFamilies.at(0);
        }
    }

    codelist.insert(code,font);

    return font;
}
#endif

RC_LINE_FORM getLineEndType(QString line)
{
    if (line.endsWith("\r\n"))
    {
        return DOS_LINE;
    }
    else if (line.endsWith("\n"))
    {
        return UNIX_LINE;
    }
    else if (line.endsWith("\r"))
    {
        return MAC_LINE;
    }

    return UNKNOWN_LINE;
}

 RC_LINE_FORM getLineEndType(const LineNode& lines)
{
    if (lines.lineText.isEmpty())
    {
        return UNKNOWN_LINE;
    }

    return getLineEndType((lines.lineText.last().text));
}

NetRegister::NetRegister(QObject *parent): QObject(parent)
{
#ifdef uos
    //QString fontName = loadFontFromFile("/usr/share/fonts/opentype/noto/NotoSansCJK-Regular.ttc",0);
    QFont font("Noto Sans CJK JP,9,-1,5,50,0,0,0,0,0,Regular",9);
    //qDebug() << "font name uos" << fontName;
    QApplication::setFont(font);
#endif
#ifdef Q_OS_MAC
    //这里的字体大小，务必要和查找结果框的高度匹配，否则会结构字体拥挤
    QFont font("Courier New,11,-1,5,50,0,0,0,0,0,Regular",11);
   // qDebug() << "font name mac";
    QApplication::setFont(font);
   // qDebug() << QApplication::font().toString();
#endif
#ifdef Q_OS_WIN
#if 0 //不在这里做了，避免影响启动速度。也不方便弹出消息
	QFont srcFont = QApplication::font();
	//qDebug() << QApplication::font().toString();
	//win11发现字体是楷体。检测并设置一下
	if (QString("SimSun") != srcFont.family())
	{

		if (!setAppFont(QString("SimSun")))
		{
			if (!setAppFont(QString("Courier")))
			{
				if (!setAppFont(QString("Times New Roman")))
				{
					//让用户安装字体。这里可以弹出qmessage,这里还是在主线程中
					QMessageBox::warning(nullptr, tr("Font Lost"), tr("Please Install System Font [Courier/SimSun/Times New Roman].The interface font will display exceptions"));
				}
			}
		}
	}
#endif
#endif
	//qDebug() << QApplication::font().toString();
	JsonDeploy::init();
}







//退出做回收工作。通知socket停止监听
void NetRegister::quit()
{
	JsonDeploy::close();
}


NetRegister::~NetRegister()
{
	quit();
}

QStringList NetRegister::getLocalMac()
{
	QStringList mac_list;
	QString strMac;
	QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces();
	for (int i = 0; i < ifaces.count(); i++)
	{
		QNetworkInterface iface = ifaces.at(i);
		//过滤掉本地回环地址、没有开启的地址
		if (iface.flags().testFlag(QNetworkInterface::IsUp) && \
			iface.flags().testFlag(QNetworkInterface::IsRunning) && \
			iface.flags().testFlag(QNetworkInterface::CanBroadcast) && \
			!iface.flags().testFlag(QNetworkInterface::IsLoopBack))
		{
			//过滤掉虚拟地址
			if (!(iface.humanReadableName().contains("VMware", Qt::CaseInsensitive)))
			{
				strMac = iface.hardwareAddress();
				if (!strMac.isEmpty() && strMac.compare("00:00:00:00:00:00") != 0)
				{
					strMac.replace(":", "");
					mac_list.append(strMac);
					break;
				}
			}
		}
	}
	//实在没有找到合适的，就放宽条件选第一个
	if (mac_list.isEmpty() && !ifaces.isEmpty())
	{
		for (int i = 0; i < ifaces.count(); i++)
		{
			QNetworkInterface iface = ifaces.at(i);
			//过滤掉本地回环地址、没有开启的地址
			if (!iface.flags().testFlag(QNetworkInterface::IsLoopBack))
			{
				//过滤掉虚拟地址
				if (!(iface.humanReadableName().contains("VMware", Qt::CaseInsensitive)))
				{
					strMac = iface.hardwareAddress();
					if (!strMac.isEmpty() && strMac.compare("00:00:00:00:00:00") != 0)
					{
						strMac.replace(":", "");
						mac_list.append(strMac);
						break;
					}
				}
			}
		}
	}
	return mac_list;
}

QString NetRegister::getLocalFirstMac()
{
	QString key("mac");
	QString v = JsonDeploy::getKeyValueFromSets(key);

	if (v.isEmpty() || v == "0")
	{

		QStringList macList = getLocalMac();
		if (!macList.isEmpty())
		{
			JsonDeploy::updataKeyValueFromSets(key, macList[0]);
			return macList[0];
		}
		return QString();
	}

	return v;
}

