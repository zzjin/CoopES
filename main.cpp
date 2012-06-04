#include <QtGui/QApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QLocale>
#include <QDebug>
//#include <QSettings>
#include <QFileInfo>
#include <QDateTime>

#include "dialog.h"
#include "fileinfo.h"

#include "logger.h"
#include "fileappender.h"
#include "consoleappender.h"
#include "ttcclayout.h"
#include "logmanager.h"

void setupLog4Qt()
{
    //    Log4Qt::LogManager::rootLogger();
    Log4Qt::Logger *logger = Log4Qt::LogManager::rootLogger();
    logger->removeAllAppenders();
    //FileAppender
    Log4Qt::FileAppender *fileappender=new Log4Qt::FileAppender();
    fileappender->setName("FileAppender");
    //    QString temp("logs/");
    //    temp=temp.append(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")).append(".log");
    QFileInfo fileinfo(QString("logs/%1%2").arg( QDateTime::currentDateTime().toString("yyyyMMdd_hhmm"),".log"));
    fileappender->setFile(fileinfo.filePath());
    Log4Qt::TTCCLayout *filelayout = new Log4Qt::TTCCLayout(Log4Qt::TTCCLayout::ISO8601);
    fileappender->setLayout(filelayout);
    fileappender->activateOptions();
    logger->addAppender(fileappender);
#ifdef ZZTEST
    //ConsoleAppender
    Log4Qt::ConsoleAppender *consoleappender = new Log4Qt::ConsoleAppender();
    consoleappender->setName("ConsoleAppender");
    consoleappender->setTarget(Log4Qt::ConsoleAppender::STDOUT_TARGET);
    //Log4Qt::SimpleLayout *consolelayout = new Log4Qt::SimpleLayout();
    Log4Qt::TTCCLayout *consolelayout = new Log4Qt::TTCCLayout(Log4Qt::TTCCLayout::ISO8601);
    consoleappender->setLayout(consolelayout);
    consoleappender->activateOptions();
    logger->addAppender(consoleappender);
#endif
    Log4Qt::LogManager::setHandleQtMessages(true);
}

// Log4Qt::Logger *logger= Log4Qt::Logger::logger("CoopES");
int main(int argc, char *argv[])
{
    setupLog4Qt();
    QApplication a(argc, argv);
    a.setApplicationName("CoopES");
    a.setOrganizationName("ZZJIN@BUPT");
    a.setOrganizationDomain("blog.zzjin.co.cc/CoopES");
    Log4Qt::Logger::logger("CoopES")->info("CoopES start");
    //load the application's translate file
    QTranslator appTrans;
    appTrans.load("CoopES_"+QLocale::system().name(),a.applicationDirPath());
    Log4Qt::LogManager::rootLogger()->logger("Init")->info()
            <<a.tr("read translate file:")<<("CoopES_"+QLocale::system().name());
    a.installTranslator(&appTrans);
    //load the qt's default tranlate file
    QTranslator qtTrans;
    qtTrans.load("qt_"+QLocale::system().name(),a.applicationDirPath());
    Log4Qt::LogManager::rootLogger()->logger("Init")->info()
            <<a.tr("read translate file:")<<("qt_"+QLocale::system().name());
    a.installTranslator(&qtTrans);
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    fileInfo fileInfo_temp;
    QString userName;
    QString hostAddr;
    int hostPort;
    if (argc!=0)
        for (int i=1;argv[i];i++)
        {
            QString temp(argv[i]);
            //get the file name
            if (temp == "-f")
                Log4Qt::LogManager::rootLogger()->logger("Init Input:")->info()
                        <<"get file name:"<<(fileInfo_temp.name=argv[i+1]);
            //get the user name
            if (temp == "-u")
                Log4Qt::LogManager::rootLogger()->logger("Init Input:")->info()
                        <<"get user name"<<(userName=QString(argv[i+1]));
            //get the host ip address
            if (temp == "-h")
                Log4Qt::LogManager::rootLogger()->logger("Init Input:")->info()
                        <<"get host ip addr"<<(hostAddr=QString(argv[i+1]));
            //get the host port
            if (temp == "-p")
                Log4Qt::LogManager::rootLogger()->logger("Init Input:")->info()
                        /*重要的转换方式*/
                        <<"get host port"<<(QString::number (hostPort= (QString(argv[i+1]).toInt ())));
        }
    Dialog w;
    w.setFile(fileInfo_temp);
    w.setUserName(userName);
    Log4Qt::LogManager::rootLogger()->logger("Init")->info()
            <<a.tr("show main window");
    w.show();
    return a.exec();
}
