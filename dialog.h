#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTextDocument>
#include <QTextDocumentFragment>
#include <QMouseEvent>
#include <QEvent>
#include <QListWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include <QHash>
#include <QPair>

#include "config.h"
#include "fileinfo.h"
#include "coopnetwork.h"
#include "logger.h"
#include "logmanager.h"

namespace Ui {
class Dialog;
}
class QsvColorDefFactory;
class QsvLangDef;
class QsvSyntaxHighlighter;
class QStringList;
class QPlainTextDocumentLayout;
class QMouseEvent;
class QEvent;

//extern Log4Qt::Logger *logger;

class Dialog : public QDialog
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

    public:
        explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    Ui::Dialog *ui;
public:
    //set the input name
    void setFile(fileInfo fileName);
    // set the user name
    void setUserName(QString userName);
    //    void setupLog4Qt();
private:
    //save the mouse position
    QPoint pos0,last;
    //local DataBase
    QSqlDatabase versionDB;

    //network connect
    CoopNetwork *coopConn;

    fileInfo fileIO;
    QString mUserName;
    QListWidgetItem *chatListItem;

    bool tryMerge(int position, int length);/*尝试合并操作*/
    bool getInfo(int position, int length);/*收到的操作*/
    bool postInfo(int position, int length);/*要进行记录的操作*/

    QTextDocument *myText;/*使用自己的文档类方便控制*/
    QString textChanged;/*保存删除文字之前的数据*/
    QString textVersion;/*保存要插入数据库的数据*/
    bool addORdel;/*保存操作类型*/

    int lastPosition;/*上次光标位置*/
    int startPosition;/*开始记录数据的位置*/
    bool isStart;/*只在第一次保存开始位置*/
    int lastLength;/*上次修改长度*/
    QString lastText; /*在任何改动生效之前保存上一次的字符串*/
    bool lastaddORdel;/*上次的操作类型*/
private slots:
    int programInit();
    void delayedAni();
    //    void pushCursor();
private slots:
    //获取数据改动
    void getContent(int position,int deleteLenght,int addLength);
    //push the status bar info
    bool pushStatusInfo(QString statusInfo);

    //send chat messages
    void sendChat();
    //push remote messages to chat list
    void getChat(int getType,QString data);

protected:
    //自己重新实现拖动操作
    void mouseMoveEvent ( QMouseEvent * event );
    void mousePressEvent ( QMouseEvent * event );
    void mouseReleaseEvent(QMouseEvent *);
    void paintEvent (QPaintEvent *);
    //自己实现的时间注册以实现动态效果
    bool eventFilter(QObject *object, QEvent *event);
private:
    QPoint mousePosition;
    bool isMousePressed;
private:
    //动画效果的实现方式。多个私有指针
    QHash<QWidget *, QPair<QPoint,QSize> > fitPairs;
    bool enterOrLeave;
    QWidget *widgetTemp;
    QSize sTemp;
    QPoint pTemp;
    //下面是语法高亮要用到的//////////////////////////////////////
public slots:
    void fillComboBoxes();
    void on_colorCombo_currentIndexChanged( const QString & text );
    void on_syntaxCombo_currentIndexChanged( const QString & text );
    void update_syntax_color();
private:
    QsvColorDefFactory	*defColors;
    QsvLangDef		*defLang;
    QsvSyntaxHighlighter	*highlight;
    QStringList		colorFiles, syntaxFiles;
    bool disable_combo_updates;
    //        QString dataPath;
    //////////////////////////////////////////////////////////
};

#endif // DIALOG_H
