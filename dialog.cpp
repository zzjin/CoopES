#include "dialog.h"
#include "ui_dialog.h"
///////////////////////
#include <QTimer>
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QPalette>
#include <QSyntaxHighlighter>
#include <QFile>
#include <QPainter>
#include <QBitmap>
#include <QDebug>

#include "qsvcolordef.h"
#include "qsvcolordeffactory.h"
#include "qsvlangdef.h"
#include "qsvsyntaxhighlighter.h"

//标题栏的长度
const static int pos_min_x = 0;
const static int pos_max_x = 800-40;
const static int pos_min_y = 0;
const static int pos_max_y = 20;

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    lastaddORdel(true),
    lastPosition(0),
    startPosition(0),
    isStart(true),
    lastLength(0),
    isMousePressed(false),
    ui(new Ui::Dialog)
{
    //初始化UI信息
    ui->setupUi(this);
    ui->statusInfo->hide();
    ui->versionList->hide();
    ui->onduButton->installEventFilter(this);
    ui->redoButton->installEventFilter(this);
    ui->printButton->installEventFilter(this);
    ui->saveButton->installEventFilter(this);
    ui->syntaxCombo->installEventFilter(this);
    ui->colorCombo->installEventFilter(this);
    ui->chatLine->installEventFilter(this);
    ui->chatPushButton->installEventFilter(this);

    //send button color problem
    //set with a white
    //    ui->chatPushButton->set

    this->setAttribute(Qt::WA_Hover);

    //取消系统自带的标题栏
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowMinMaxButtonsHint);

    //关联对应的hook到自己的槽(核心)
    connect(ui->mainTextEdit->document(),SIGNAL(contentsChange(int,int,int)),this,SLOT(getContent(int,int,int)));
    myText = ui->mainTextEdit->document();
    //    connect(ui->mainTextEdit,SIGNAL(cursorPositionChanged()),this,SLOT(pushCursor()));

    connect(ui->chatPushButton,SIGNAL(clicked()),this,SLOT(sendChat()));

    Log4Qt::LogManager::setHandleQtMessages(true);
    QTimer::singleShot(0,this,SLOT(programInit()));
}

Dialog::~Dialog()
{
    delete ui;
    versionDB.close();
    coopConn->disconnectHost ();
}

void Dialog::setFile(fileInfo fileName)
{
    fileIO.name=fileName.name;
    if (ZZTEST)
        fileIO.name="test";
    fileIO.path=fileName.path;
}

void Dialog::setUserName (QString userName)
{
    mUserName = userName;
    if (ZZTEST)
        mUserName="test";
}

void Dialog::sendChat ()
{
    QString pTemp = mUserName+":";
    pTemp+=ui->chatLine->text ();
    coopConn->send (10,pTemp);
}

void Dialog::getChat (int getType, QString data)
{
    if (getType==10)
    {
        chatListItem = new QListWidgetItem(data,ui->chatList);
        ui->chatList->addItem (chatListItem);
    }
    ui->chatLine->clear ();
}

bool Dialog::eventFilter(QObject *object, QEvent *event)
{
    if (event->type()!=QEvent::Enter && event->type()!=QEvent::Leave)
        return false;
    enterOrLeave=event->type() == QEvent::Enter?true:false;
    widgetTemp=((QWidget *)object);
    if (enterOrLeave)
        fitPairs[widgetTemp]=qMakePair(widgetTemp->pos(),widgetTemp->size());//插入默认的位置，大小信息
    //    QTimer::singleShot(0,this,SLOT(delayedAni()));
    delayedAni();
    return false;
}

void Dialog::delayedAni()
{
    sTemp=fitPairs[widgetTemp].second;
    pTemp=fitPairs[widgetTemp].first;
    if (enterOrLeave)
    {
        widgetTemp->resize((sTemp*1.2));
        //下面几行代码判断是否缩放之后是否出界
        int x=pTemp.x()-(sTemp.width()*0.1);
        int y=pTemp.y()-(sTemp.height()*0.1);
        x=x<0?0:x;
        x=x>(this->width()-sTemp.width()*1.2)?(this->width()-sTemp.width()*1.2):x;
        y=y<0?0:y;
        y=y>(this->height()-sTemp.height()*1.2)?(this->width()-sTemp.height()*1.2):y;
        //over
        widgetTemp->move(x,y);
    }
    else
    {
        widgetTemp->resize(sTemp);
        widgetTemp->move(pTemp);
    }
}

//自己实现的窗口拖动操作
void Dialog::mousePressEvent(QMouseEvent *event)
{
    mousePosition = event->pos();
    //只对标题栏范围内的鼠标事件进行处理
    if (mousePosition.x()<=pos_min_x)
        return;
    if ( mousePosition.x()>=pos_max_x)
        return;
    if (mousePosition.y()<=pos_min_y )
        return;
    if (mousePosition.y()>=pos_max_y)
        return;
    isMousePressed = true;
}

void Dialog::mouseMoveEvent(QMouseEvent *event)
{
    if ( isMousePressed==true )
    {
        QPoint movePot = event->globalPos() - mousePosition;
        move(movePot);
    }
}

void Dialog::mouseReleaseEvent(QMouseEvent *event)
{
    isMousePressed=false;
}

void Dialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap temp;
    temp.load("./data/images/bg.png",0,Qt::AvoidDither|Qt::ThresholdDither|Qt::ThresholdAlphaDither);
    /*mask,使用图片的遮照来遮照窗口*/
    setMask(temp.mask());
    temp.scaled(800,580);
    //    painter.setOpacity(0.2);
    //暂时不开启
    painter.drawPixmap(0,0,temp);
}

int Dialog::programInit()
{
    //读取现有的配置信息
    defColors = NULL;
    defLang  = NULL;
    highlight  = NULL;
    QPalette pal = ui->mainTextEdit->palette();
    int tempColor =245;
    pal.setColor(QPalette::Background, QColor(tempColor, tempColor, tempColor));
    ui->mainTextEdit->setPalette(pal);
    //    dataPath  = QApplication::applicationDirPath();
    QTimer::singleShot( 0, this, SLOT(fillComboBoxes()));
    /////////////////////////////////////////////////////
    //创建数据库链接
    versionDB = QSqlDatabase::addDatabase("QSQLITE");
    versionDB.setDatabaseName("./user/version.db");
    //    if (ZZTEST)
    //        versionDB.setDatabaseName(":memory:");
    if (!(versionDB.open()))
        return -1;
    logger()->info()<<tr("open database")<<versionDB.databaseName()<<tr(":")<< versionDB.isOpen();
    QSqlQuery query;
    logger()->info()<<tr("create table if not exist :")
                    <<query.exec(QString("CREATE TABLE IF NOT EXISTS  %1 %2")
                                 .arg(fileIO.name)
                                 /*addORdel:1表示增加,0表示删除*/
                                 /*mode:1表示本地,0表示远程*/
                                 .arg("(index_id INTEGER PRIMARY KEY AUTOINCREMENT, position INT, length INT, word TEXT, addORdel SMALLINT, mode SMALLINT);"))
                    <<query.lastError().text();
    //不知道在哪出现了stringlist的重载啊。
    qDebug()<<versionDB.tables();
    //    logger()->info()<<temp;
    //init internet connect
    coopConn = new  CoopNetwork(this);
    coopConn->set ("59.64.139.155");
    //        coopConn->set ("118.229.175.27");
    //    coopConn->set ();
    connect(coopConn,SIGNAL(state(QString)),this,SLOT(pushStatusInfo(QString)));
    connect(coopConn,SIGNAL(get(int,QString)),this,SLOT(getChat(int,QString)));
    coopConn->connectHost();
    return 1;
}

////语法高亮的实现,不是我自己写的.//////////////////////////////////////////////
void Dialog::fillComboBoxes()
{
    disable_combo_updates = true;
    QDir dir;
    dir = QDir("./data/langs/", "*.lang");
    syntaxFiles = dir.entryList(QDir::Files);
    ui->syntaxCombo->addItems( syntaxFiles );

    dir = QDir("./data/colors/", "*.xml");
    colorFiles = dir.entryList(QDir::Files);
    ui->colorCombo->addItems( colorFiles );

    disable_combo_updates = false;
    // now set the default language to c++, and use kate color definitions
    ui->syntaxCombo->setCurrentIndex( ui->syntaxCombo->findText("cpp.lang" ) );
    ui->colorCombo->setCurrentIndex( ui->colorCombo->findText("kate.xml" ) );
    //显示当前状态    
    pushStatusInfo(tr("program init OK,set default syntax type C++"));
}

void Dialog::on_colorCombo_currentIndexChanged( const QString & text )
{
    QTextCursor cs = ui->mainTextEdit->textCursor();
    if (disable_combo_updates)
        return;
    QTimer::singleShot( 0, this, SLOT(update_syntax_color()) );
}

void Dialog::on_syntaxCombo_currentIndexChanged( const QString & text )
{
    if (disable_combo_updates)
        return;
    QTimer::singleShot( 0, this, SLOT(update_syntax_color()) );
}

void Dialog::update_syntax_color()
{
    delete highlight;
    delete defLang;
    delete defColors;
    defColors = new QsvColorDefFactory("./data/colors/" + ui->colorCombo->currentText() );
    // set the background of the texteditor, to the same as the syntax
    // also set the default color of the syntax
    // this must be done before setting the new syntax highlighter
    QPalette p( ui->mainTextEdit->palette() );
    p.setColor( QPalette::Base, defColors->getColorDef("dsNormal").getBackground() );
    ui->mainTextEdit->setPalette( p );
    ui->mainTextEdit->setTextColor( defColors->getColorDef("dsNormal").getColor() );
    defLang   = new QsvLangDef("./data/langs/" + ui->syntaxCombo->currentText() );
    highlight = new QsvSyntaxHighlighter( ui->mainTextEdit->document(), defColors, defLang );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////

bool Dialog::pushStatusInfo(QString statusInfo)
{
    ui->statusInfo->setText(statusInfo);
    ui->statusInfo->resize(ui->statusInfo->sizeHint());
    ui->statusInfo->show();
    QTimer::singleShot( 3*1000, ui->statusInfo, SLOT(hide()));
    return true;
}

void Dialog::getContent(int position,int deleteLenght,int addLength)
{
    if (deleteLenght == addLength)/*我也不知道为什么会有那么多相等出现*/
        //直接返回.不处理
        return ;
    logger()->info()<<tr("cursor position")<<position<<tr("deleted lenght")<<deleteLenght<<tr("add lenght")<<addLength;
    /**
  *几种情况的说明
  *1.没有输入法的情况:
  *1.1 光标持续变化->连续的新增或者删除
  *1.1.1 add->add 不变,直接将变动字符插入
  *1.1.2 add->del 改变,写入数据库
  *1.1.3 del->del 改变,写入数据库
  *1.1.4 del->add 不变,直接将变动字符插入
  *1.2 改变了光标之后的改变->写数据库,记录新光标位置
  *2 有输入法,新增文字->直接将变化了的文字输入到数据库
 */
    //新增与删除两个部分分开
    /*判断是否输入了非英文字符*/
    /*使用输入发之后就会出现这个问题.*/
    if ((deleteLenght != 0) && /*删掉之前的所有文字*/
            (addLength != 0)){ /*新增多的文字*/
        //当删掉最后一个字的时候会产生"0,1,0"的意外情况直接处理.无视之
        //处理新增的文字
        position = position+deleteLenght-1;
        addLength = (uint)(addLength - deleteLenght);
        //        postInfo(position,addLength);
        //        lastText = myText->toPlainText();
        //        lastPosition = position;
        addORdel= true;
        return ;
    }
    if (addLength == 0){/*删除的话不知道有没有什么hook能显式的截获输出*/
        //只有在新增的时候保存一个string的备份.在删除的时候进行跟新了.
        //在这里处理删除操作的文字
        addLength = deleteLenght;
        logger()->info()<<tr("delete")<<(textChanged=lastText.mid(position,addLength));
        addORdel= false;
        //OK
    }
    else
    {
        logger()->info()<<(textChanged=myText->toPlainText().mid(position,addLength));
        addORdel=true;
    }

    if (!tryMerge(position,addLength))
    {
        postInfo(startPosition,addLength);
        textVersion = textChanged;
        lastLength = 1;
        startPosition = position;
        //        lastPosition = position;
    }
    else
    {
        //        firstPosition = position;

    }
    lastText = myText->toPlainText();
    lastPosition = position;
    lastaddORdel = addORdel;
    if (lastText.isEmpty())
        logger()->info()<<tr("text data empty!");
}

bool Dialog::postInfo(int position, int length)
{
    QSqlQuery query;
    logger()->info()<<tr("database operation:")
                    <<query.exec(QString("INSERT INTO %1 (position, length, word, addORdel, mode) VALUES (%2,%3,'%4',%5,1);")
                                 .arg(fileIO.name)
                                 .arg(position)
                                 .arg(lastLength)
                                 .arg(textVersion)
                                 .arg(lastaddORdel))
                    <<query.lastError().text();
    //接着广播消息什么的
    return true;
}

bool Dialog::getInfo(int position, int length)
{
    return true;
}

/*拟解决的问题:用户输入英文就直接开始记录数据库导致的数据库数据过多
  有两种方案:
  1. 按照时间把差不多时间的同操作合并,再写入数据库.
  2. 按照qt本来的实现方式,将相同的操作合并,再写入数据库.
  */

bool Dialog::tryMerge(int position, int length)
{    
    if (addORdel != lastaddORdel)
    {
        isStart =true;
        return false;
    }

    if ((lastaddORdel == true) &&
            (lastPosition + length == position) /*&&
                                                                (strPos + length == other.strPos)*/)
    {
        startPosition = isStart?position:startPosition;
        lastLength += length;
        textVersion += textChanged;
        isStart =false;
        return true;
    }

    // removal to the 'right' using 'Delete' key
    if ((lastaddORdel == false) &&
            (lastPosition == position) /*&&
                                                                (strPos + length == other.strPos)*/)
    {
        startPosition = isStart?position:startPosition;
        lastLength += length;
        textVersion += textChanged;
        isStart =false;
        return true;
    }

    // removal to the 'left' using 'Backspace'
    if ((lastaddORdel == false) &&
            (position + length == lastPosition) /*&&  (other.strPos + other.length == strPos)*/)
    {
        startPosition = position;
        lastLength += length;
        textVersion += textChanged;
        isStart =false;
        return true;
    }

    if ((lastPosition ==0) && (lastaddORdel ==true))
    {
        startPosition = isStart?position:startPosition;
        lastLength += length;
        textVersion += textChanged;
        isStart =false;
        return true;
    }
    return false;
}
