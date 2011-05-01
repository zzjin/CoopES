#include <QtGui/QApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QLocale>
#include <QDebug>
#include "dialog.h"
#include "fileinfo.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator appTrans;
    appTrans.load("CoopES_"+QLocale::system().name(),a.applicationDirPath());
    qDebug()<<"read translate file:"<<("CoopES_"+QLocale::system().name());
    a.installTranslator(&appTrans);
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    fileInfo fileInfo_temp;
    if (argc!=0)
        for (int i=1;argv[i];i++)
            if (QString(argv[i])=="-f")
                qDebug()<<"get file name"<<(fileInfo_temp.name=argv[i+1]);
    Dialog w;
    w.setFile(fileInfo_temp);
    qDebug()<<"show main window";
    w.show();
    return a.exec();
}
