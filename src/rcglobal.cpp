#include "rcglobal.h"
#include <QSettings>
#include <QFileInfo>
#include <QDir>
#include <QProcess>
#include <QUrl>
#include <QStandardPaths>

QString s_userLangDirPath;

QString getUserLangDirPath()
{
	if (s_userLangDirPath.isEmpty())
	{
		QString settingDir = QString("notepad/userlang/test");
		QSettings qs(QSettings::IniFormat, QSettings::UserScope, settingDir);
		QString qsSavePath = qs.fileName();
		QFileInfo fi(qsSavePath);
		s_userLangDirPath = fi.dir().absolutePath();
		//保存起来userLangPath，避免后续每次都要去查找。注意这个目录是在notepad/userlang这个级别
	}
	return s_userLangDirPath;
}

const int M_SIZE = 1024 * 1024;
const int G_SIZE = 1024 * 1024 * 1024;

//把字节大小文件，转换为M 或 G 单位
QString tranFileSize(qint64 fileSize)
{
	float num = 0.0f;
	QString unit;

	if (fileSize >= G_SIZE)
	{
		num = double(fileSize) / G_SIZE;
		unit = "GB";
	}
	else if (fileSize >= M_SIZE)
	{
		num = double(fileSize) / M_SIZE;
		unit = "MB";
	}
	else if (fileSize > 1024)
	{
		num = float(fileSize) / 1024;
		unit = "KB";
	}
	else
	{
		return QString("%1").arg(fileSize);
	}
	
	return QString("%1 %2").arg(num, 0, 'f' , 2).arg(unit);
}

#if defined(Q_OS_UNIX) && !defined(Q_OS_MAC)
//linux 下定位文件所在目录并选中该文件。
//不同发行版默认文件管理器不同，且各自依赖的库不一样，这里不引入任何额外库：
//先用 xdg-mime 查询系统默认文件管理器，按下表使用其自带的“定位选中”参数；
//表中没有的或找不到 xdg-mime 时，退化为用 xdg-open 直接打开所在目录（不选中）。
struct LinuxFileManager
{
	const char* desktopFile;  //xdg-mime 返回的默认文件管理器 desktop 文件名
	const char* program;      //对应的可执行程序
	const char* selectArg;    //定位并选中文件的参数；为空表示该管理器只支持打开所在目录
};

static const LinuxFileManager LINUX_FM_TABLE[] = {
	{ "peony.desktop",               "peony",           "-i" },          //麒麟 UKUI
	{ "nautilus.desktop",            "nautilus",        "--select" },    //GNOME
	{ "org.gnome.Nautilus.desktop",  "nautilus",        "--select" },
	{ "dolphin.desktop",             "dolphin",         "--select" },    //KDE
	{ "org.kde.dolphin.desktop",     "dolphin",         "--select" },
	{ "dde-file-manager.desktop",    "dde-file-manager", "--show-item" },//UOS/深度
	{ "deepin-file-manager.desktop", "dde-file-manager", "--show-item" },
	{ "nemo.desktop",                "nemo",            nullptr },       //Mint，无定位参数
	{ "caja.desktop",                "caja",            nullptr },       //Mate
	{ "thunar.desktop",              "thunar",          nullptr },       //XFCE
	{ "pcmanfm.desktop",             "pcmanfm",         nullptr },       //LXDE
	{ "pcmanfm-qt.desktop",          "pcmanfm-qt",      nullptr },       //LXQt
};

static bool programExists(const QString& program)
{
	return !QStandardPaths::findExecutable(program).isEmpty();
}

static void launchFileManager(const LinuxFileManager& fm, const QString& openDir, const QString& fileUri)
{
	if (fm.selectArg != nullptr)
	{
		QProcess::startDetached(fm.program, QStringList() << fm.selectArg << fileUri);
	}
	else
	{
		QProcess::startDetached(fm.program, QStringList() << openDir);
	}
}

static void showFileInLinuxManager(const QString& path)
{
	QFileInfo fi(path);
	if (!fi.exists())
	{
		return;
	}

	//要打开的目录：文件取所在目录，目录取其本身
	QString openDir = fi.isDir() ? fi.absoluteFilePath() : fi.absolutePath();
	QString fileUri = QString::fromUtf8(QUrl::fromLocalFile(fi.absoluteFilePath()).toEncoded());

	//xdg-mime 属于 xdg-utils，桌面发行版基本都自带
	QString defaultFm;
	QProcess xdgMime;
	xdgMime.start("xdg-mime", QStringList() << "query" << "default" << "inode/directory");
	xdgMime.closeWriteChannel();
	if (xdgMime.waitForFinished(3000))
	{
		defaultFm = QString::fromUtf8(xdgMime.readAllStandardOutput()).trimmed();
	}
	else
	{
		xdgMime.kill();
	}

	if (!defaultFm.isEmpty())
	{
		for (const LinuxFileManager& fm : LINUX_FM_TABLE)
		{
			if (defaultFm == fm.desktopFile && programExists(fm.program))
			{
				launchFileManager(fm, openDir, fileUri);
				return;
			}
		}
	}
	else
	{
		//没有 xdg-mime 可用时，按常见程度探测一个已安装的文件管理器
		for (const LinuxFileManager& fm : LINUX_FM_TABLE)
		{
			if (programExists(fm.program))
			{
				launchFileManager(fm, openDir, fileUri);
				return;
			}
		}
	}

	//兜底：xdg-open 会自动调用系统默认文件管理器打开所在目录（不选中）
	QProcess::startDetached("xdg-open", QStringList() << openDir);
}
#endif

void showFileInExplorer(QString path)
{
#ifdef _WIN32
	path = path.replace("/", "\\");
	QProcess::startDetached(QString("explorer.exe /select,%1").arg(path));
#elif defined(Q_OS_MAC)
	path = path.replace("\\", "/");
	QProcess::startDetached(QString("open -R %1").arg(path));
#else
	showFileInLinuxManager(path);
#endif
}
