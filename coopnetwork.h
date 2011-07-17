#ifndef COOPNETWORK_H
#define COOPNETWORK_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>

#include <QStringList>

#include "config.h"
#include "fileinfo.h"
#include "logger.h"
#include "logmanager.h"


//general server host name
const static QString debugHost="localhost";
//general server host ip
const static int debugPort =2047;
/** network
  *@author zzjin
  *@date 201106
  */

class CoopNetwork : public QObject
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
public:
    explicit CoopNetwork(QObject *parent = 0);
    enum networkConfigType {DEFAULT,KEEPALIVE,TESTCONN};

signals:
    //get the connect state
    void state(QString);
    /** the defalut get data method
      */
    void get(char*);
    void get(int,int,QString);
    void get(int,QString);
    void error(QString);

private slots:
    void connected();
    void dealData();
    void errorInfo();

private:
    QTcpSocket *mSocket;

public:
    /** set the host and port
      *@param   QString   host name
      *@param   int       host port
      *@return  void
      *@note if the host is not given, the 'localhost' will be used
      *@note if the port is not given, the '2047' will be used
      */
    void set(QString host=QString(),int port=debugPort);/*{mHost=host;mPort=port;}*/
    bool connectHost(networkConfigType config=CoopNetwork::DEFAULT);
    void disconnectHost (){mSocket->disconnectFromHost ();}
    bool send(const char* data);
    bool send(int sendType,QString &data);
    bool send(int sendType, int dataType, QString &data);
    QString mHost;
    int mPort;

};

#endif // COOPNETWORK_H
