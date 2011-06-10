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



const static QString debugHost="118.229.175.27";
const static int debugPort =2047;

class CoopNetwork : public QObject
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
public:
    explicit CoopNetwork(QObject *parent = 0);

signals:
    void state(QString);
    void get(char*);
    void get(int,int,QString);
    void get(int,QString);

public slots:
    void connected();
    void dealData();

public:
    QTcpSocket *mSocket;

public:
    void set(QString host=QString(),int port=debugPort);/*{mHost=host;mPort=port;}*/
    bool connectHost(int config=0);
    void disconnectHost (){mSocket->disconnectFromHost ();};
    bool send(char* data);
    bool send(int sendType,QString &data);
    bool send(int sendType, int dataType, QString &data);
    QString mHost;
    int mPort;

};

#endif // COOPNETWORK_H
