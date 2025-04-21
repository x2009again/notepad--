#include "ccnotepad.h"
#include "nddsetting.h"
#include "styleset.h"


#include <QtWidgets/QApplication>
#include <QTextCodec>
#include <QMessageBox>
#include <QSharedMemory>
#include <QFile>
#include <QStatusBar>
#include <qobject.h>
#include <QThread>
#include <QDir>

#ifdef Q_OS_UNIX
#include <QStyleFactory>
#include <signal.h>
#include <unistd.h>
#include <QDebug>
#include <QWidget>
#endif

#ifdef Q_OS_WIN
#pragma comment(lib, "user32.lib")
#if _DEBUG
// Bug: 在 Visual Studio 中引发无法构建的问题，此处为无法找到 qmyedit_x 库
//#pragma comment(lib, "qmyedit_qt5d.lib")
#else
//#pragma comment(lib, "qmyedit_qt5.lib")
#endif
#include <qt_windows.h>
const ULONG_PTR CUSTOM_TYPE = 10000;
const ULONG_PTR OPEN_NOTEPAD_TYPE = 10001;
const ULONG_PTR CUSTOM_TYPE_FILE_LINENUM = 10002;
bool s_isAdminAuth = false;
#endif

const QString c_strTitle = "Ndd";


#ifdef Q_OS_UNIX
#if defined(Q_OS_MAC)
QSharedMemory shared("CCNotebook123");;//mac下面后面带一个版本号，避免新的打不开
#else
QSharedMemory shared("CCNotebook");
#endif
QSharedMemory nppShared("notepad--");

static void sig_usr(int signo)
{
    if(signo  == SIGUSR1)
    {
        qlonglong winId;
        shared.lock();
        memcpy(&winId,shared.data(),sizeof(qlonglong));
        shared.unlock();

         QWidget *pMain = QWidget::find((WId)winId);
         CCNotePad* pNotePad = dynamic_cast<CCNotePad*>(pMain);
         if(pNotePad != nullptr)
         {
             QString filePath((char*)nppShared.data()+4);
             if(!filePath.isEmpty())
             {
             pNotePad->openFile(filePath);
             }
             pNotePad->activateWindow();
             pNotePad->showNormal();
             qDebug() << "sig_usr" << filePath;
         }
    }
}
#endif

#ifdef Q_OS_MAC

static void openfile(QString filePath)
{

    qlonglong winId;
    shared.lock();
    memcpy(&winId,shared.data(),sizeof(qlonglong));
    shared.unlock();

     QWidget *pMain = QWidget::find((WId)winId);
     CCNotePad* pNotePad = dynamic_cast<CCNotePad*>(pMain);
     if(pNotePad != nullptr)
     {
         if(!filePath.isEmpty())
         {
            pNotePad->openFile(filePath);
         }
         pNotePad->activateWindow();
         pNotePad->showNormal();
     }
}

class MyApplication : public QApplication
{
   public:
       MyApplication(int &argc, char **argv)
           : QApplication(argc, argv)
       {
       }

       bool event(QEvent *event)
       {
           if (event->type() == QEvent::FileOpen) {
               QFileOpenEvent *openEvent = static_cast<QFileOpenEvent *>(event);
               qDebug() << "Open file" << openEvent->file();
               s_openfile = openEvent->file();
               openfile(s_openfile);
           }

           return QApplication::event(event);
       }
       QString s_openfile;
   };
#endif

int main(int argc, char *argv[])
{
    // 设置高DPI缩放策略
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
    // Qt 5.14及以上版本使用PassThrough策略,不对DPI进行舍入处理
    QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#elif (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
    #ifdef Q_OS_WIN
        // Qt 5.6-5.14版本在Windows下手动计算DPI缩放
        HDC hdc = CreateDC(L"display", NULL, NULL, NULL);
        int ndpi = GetDeviceCaps(hdc, LOGPIXELSY);
        qputenv("QT_SCALE_FACTOR", QString::number(ndpi / 96.0).toUtf8());
    #endif // Q_OS_WIN
#endif

    // Qt 6.0以下版本启用高DPI缩放
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    // 创建QApplication实例
#ifdef Q_OS_MAC
    // Mac平台使用自定义的MyApplication类来处理文件打开事件
    MyApplication a(argc, argv);
#else
    QApplication a(argc, argv);
#endif

    // Qt 6.0以下版本启用高DPI图标
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    a.setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif

	//不能开启，开启后相对路径打开文件失败
	//QDir::setCurrent(QCoreApplication::applicationDirPath());


    // Unix平台使用fusion风格
#if defined(Q_OS_UNIX)
    QApplication::setStyle(QStyleFactory::create("fusion"));
#endif

    // 设置应用程序名称和显示名称
    a.setApplicationDisplayName(c_strTitle);
    a.setApplicationName(c_strTitle);

    // 设置版本号(如果定义了NOTEPAD_VERSION)
#include <config.h>
#ifdef  NOTEPAD_VERSION
    a.setApplicationVersion(NOTEPAD_VERSION);
#endif

    // 获取命令行参数
    QStringList arguments = QCoreApplication::arguments();

    // 处理命令行参数中的相对路径(只有 1、2 需要处理短路径)
    // 支持三种命令行格式:
    // 1) ndd filepath
    // 2) ndd filepath -n linenum
    // 3) ndd -multi filepath
    if ((arguments.size() == 2) || (arguments.size() == 4))
    {
        QFileInfo fi(arguments[1]);
        if (fi.isRelative())
        {
            QString absDir = QDir::currentPath();
            arguments[1] = QString("%1/%2").arg(absDir).arg(arguments.at(1));
        }
    }

    // UOS系统设置默认字体
#ifdef uos
    QFont font("Noto Sans CJK SC,9,-1,5,50,0,0,0,0,0,Regular", 9);
    QApplication::setFont(font);
#endif

    // Mac系统设置默认字体
#ifdef Q_OS_MAC
	// 这里的字体大小，务必要和查找结果框的高度匹配，否则会结构字体拥挤
    QFont font("Courier New,11,-1,5,50,0,0,0,0,0,Regular", 11);
    QApplication::setFont(font);
#endif

    bool isGotoLine = false;

    // Windows平台特有的单实例和进程间通信处理
#ifdef Q_OS_WIN
    QSharedMemory shared("ccnotepad");

    if (arguments.size() > 2)
    {
        // 处理管理员权限多开
        if (arguments[1] == QString("-muti"))
        {
            s_isAdminAuth = true;
            QString title = QString(u8"%1 管理员").arg(c_strTitle);
            a.setApplicationDisplayName(title);
			//删除-muti这个参数
            arguments.removeAt(1);
			//管理员不占用共享标志。这样的目的是，当管理员窗口存在时
			//打开原来的文件，原来的文件可以占用共享标志，作为主窗口打开。
			//管理员窗口永远不做主窗口打开
            goto authAdmin;
        }
        // 处理跳转到指定行
        else if ((arguments.size() == 4) && arguments[2] == QString("-n"))
        {
            isGotoLine = true;
        }
    }
#else
    // 非Windows平台处理跳转到指定行
    if ((arguments.size() == 4) && (arguments[2] == QString("-n")))
    {
		//使用的是 file -n lineNums 方式。目前只有windows下支持 xxxfile -n linenum的格式
        isGotoLine = true;
    }
#endif

    // 处理共享内存,用于实现单实例
	// 在 linux / unix 中程序异常结束共享内存不会回收
    // 参考 https://gitee.com/zinface/infomation-tips/blob/master/src/main.cpp
    shared.attach();
    shared.detach();

    // 如果已经有实例在运行
    if (shared.attach())
    {
        // 处理命令行参数,将文件打开请求发送给现有实例
        if (arguments.size() > 1)
        {
            // 平台特定的进程间通信实现
        #if defined(Q_OS_WIN)
			int tryTimes = 0;
			do {
			qlonglong hwndId;
			shared.lock();
			memcpy(&hwndId, shared.data(), sizeof(qlonglong));
			shared.unlock();

			HWND hwnd = (HWND)hwndId;

			if (::IsWindow(hwnd))
			{
					if (!isGotoLine)
					{
						//就是ndd filepath的命令行格式
				//去掉第一个参数，后续的参数拼接起来。其实参数中间有空格还是需要使用""引用起来，避免空格参数分隔为多个
				arguments.takeFirst();

				QString filename = arguments.join("");
				QByteArray data = filename.toUtf8();

				COPYDATASTRUCT copydata;
				copydata.dwData = CUSTOM_TYPE; //自定义类型
				copydata.lpData = data.data();  //数据大小
				copydata.cbData = data.size();  // 指向数据的指针

				::SendMessage(hwnd, WM_COPYDATA, reinterpret_cast<WPARAM>(nullptr), reinterpret_cast<LPARAM>(&copydata));
			}
			else
			{
						//是 filepath -n linenums 方式。不考虑filepath含有空格的情况，因为前面做了严格判断
						
						QString para = QString("%1|%2").arg(arguments[1]).arg(arguments[3]);
						QByteArray data = para.toUtf8();

						COPYDATASTRUCT copydata;
						copydata.dwData = CUSTOM_TYPE_FILE_LINENUM; //自定义类型
						copydata.lpData = data.data();  //数据大小
						copydata.cbData = data.size();  // 指向数据的指针

						::SendMessage(hwnd, WM_COPYDATA, reinterpret_cast<WPARAM>(nullptr), reinterpret_cast<LPARAM>(&copydata));
					}

					break;
				}
				else
				{
					
					//20230304 右键多个文件同时打开，比如3个。此时只有第1个可获取锁，其余2个均走这里。
					//因为第个还没有来的及写入hwnd。此时不要goto drop_old。等一下再重试
					QThread::sleep(1); 
					++tryTimes;

					//2次识别后，没法了，只能通过继续往下走。
				//失败了，此时说明前一个窗口极可能状态错误了。如果不处理，则再也打不开程序了
					if (tryTimes > 2)
					{
				goto drop_old;
			}

				}
			} while (true);

        #elif defined (Q_OS_MAC)
        {
               //mac下面不需要，有他自身的机制保证
        }
        #else
            pid_t pid;

            arguments.takeFirst();
            QString filename = arguments.join("");
            QByteArray data = filename.toUtf8();

            nppShared.attach();
            nppShared.lock();
            memcpy(&pid, nppShared.data(), sizeof(pid_t));
            memset((char*)nppShared.data()+sizeof(pid_t),0, 1024-sizeof(pid_t));
            memcpy((char*)nppShared.data()+sizeof(pid_t),data.data(),data.size());
            nppShared.unlock();

            //if kill failed, then open a new process
            if(0 != kill(pid,SIGUSR1))
            {
                goto unix_goon;
            }
        #endif
		}
		else if (arguments.size() == 1)
		{
#if defined(Q_OS_WIN)
			//把窗口设置到最前
			qlonglong hwndId;
			shared.lock();
			memcpy(&hwndId, shared.data(), sizeof(qlonglong));
			shared.unlock();
			HWND hwnd = (HWND)hwndId;
			if (::IsWindow(hwnd))
			{
				QString filename("open");
				QByteArray data = filename.toUtf8();

				COPYDATASTRUCT copydata;
				copydata.dwData = OPEN_NOTEPAD_TYPE; //自定义类型
				copydata.lpData = data.data();  //数据大小
				copydata.cbData = data.size();  // 指向数据的指针

				::SendMessage(hwnd, WM_COPYDATA, reinterpret_cast<WPARAM>(nullptr), reinterpret_cast<LPARAM>(&copydata));
			}
			else
			{
				//失败了，此时说明前一个窗口极可能状态错误了。如果不处理，则再也打不开程序了
				//继续新开一个窗口，放弃之前的旧内容
				goto drop_old;
			}
#elif defined (Q_OS_MAC)
{
       //mac下面不需要，有他自身的机制保证
}
#else
            pid_t pid;
            nppShared.attach();
            nppShared.lock();
            memcpy(&pid, nppShared.data(), sizeof(pid_t));
            memset((char*)nppShared.data()+sizeof(pid_t),0, 1024-sizeof(pid_t));
            nppShared.unlock();
            qDebug()<<"empty file send";

            if(0 != kill(pid,SIGUSR1))
            {
                goto unix_goon;
            }
#endif
        }
        return 0;
    }

    // 创建共享内存,标记为第一个实例
#if defined(Q_OS_WIN)
    shared.create(32);
#elif defined (Q_OS_MAC)
	//mac下面不需要，有他自身的机制保证。当程序已经在线时，再打开程序，系统会自动调用已经存在的程序出现
	//不需要使用类似linux下面的机制。
    shared.create(32);
    nppShared.create(32);
#else
unix_goon:
    shared.create(32);
    nppShared.create(2048);
    // 注册SIGUSR1信号处理
    if(signal(SIGUSR1,sig_usr) == SIG_ERR)
    {
        qDebug()<<"linux create sign failed";
    }
#endif

#if defined(Q_OS_WIN)
authAdmin:
drop_old:
#endif

    // 确保最后一个窗口关闭时退出应用
	//20221009发现有小概率出现窗口没有，但是进程还在的诡异问题，加个保护一下
    QApplication::setQuitOnLastWindowClosed(true);

    // 初始化设置
    NddSetting::init();

    // 设置皮肤
    int id = NddSetting::getKeyValueFromNumSets(SKIN_KEY);
    StyleSet::setSkin(id);

    // 创建主窗口
    CCNotePad *pMainNotepad = new CCNotePad(true);
    pMainNotepad->setAttribute(Qt::WA_DeleteOnClose);
    pMainNotepad->setShareMem(&shared);
    pMainNotepad->quickshow();
    pMainNotepad->syncCurSkinToMenu(id);

    // 存储窗口ID到共享内存
#ifdef Q_OS_WIN
	//HWND hwnd = ::FindWindowA("Qt5QWindowIcon", "CCNotebook");
	//发现hwnd就是和effectiveWinId相等的，不需要查询了
	//管理员可以多开，暂时不把管理员的权限作为主窗口，因为其他用户没有权限右键菜单发送消息给管理员窗口去打开文件
    if (!s_isAdminAuth)
    {
        qlonglong winId = (qlonglong)pMainNotepad->effectiveWinId();
        shared.lock();
        memcpy(shared.data(), &winId, sizeof(qlonglong));
        shared.unlock();
    }
#else
    qlonglong winId = (qlonglong)pMainNotepad->effectiveWinId();
    shared.lock();
    memcpy(shared.data(), &winId, sizeof(qlonglong));
    shared.unlock();
    // 存储进程ID
    nppShared.attach();
    //get proceess id to share memory
    pid_t pid = getpid();
    nppShared.lock();
    memcpy(nppShared.data(), &pid, sizeof(pid_t));
    nppShared.unlock();
#endif

    // 恢复上次会话的文件
    // 如果没有要恢复的文件且没有命令行参数,则新建文件
#ifdef Q_OS_WIN
    if (!s_isAdminAuth)
    {
        if (0 == pMainNotepad->restoreLastFiles() && (arguments.size() == 1))
        {
            pMainNotepad->initTabNewOne();
        }
    }
#else
    if (0 == pMainNotepad->restoreLastFiles())
    {
        pMainNotepad->initTabNewOne();
    }
#endif

    // 处理命令行指定的文件
    if (arguments.size() == 2)
    {
#ifdef Q_OS_WIN
        if (!s_isAdminAuth)
        {
            pMainNotepad->openFile(arguments[1]);
        }
        else
        {
			//如果是管理员，还不能直接打开文件，需要恢复之前文件的修改内容
			//恢复不了，再直接打开
			pMainNotepad->tryRestoreFile(arguments[1]);
        }
#else
        pMainNotepad->openFile(arguments[1]);
#endif
    }
    // 处理跳转到指定行
    else if (isGotoLine)
    {
		//是filepath -n xxx 格式。
        bool ok = true;
        int lineNum = arguments[3].toInt(&ok);
        if (!ok)
        {
            lineNum = -1;
        }
        pMainNotepad->openFile(arguments[1], lineNum);
    }

    // Windows平台检查应用字体
#ifdef Q_OS_WIN
    pMainNotepad->checkAppFont();
#endif

    // 启动事件循环
    a.exec();

    // 清理设置
    NddSetting::close();

    return 0;
}
