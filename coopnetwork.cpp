#include "coopnetwork.h"
#include <QtNetwork>

CoopNetwork::CoopNetwork(QObject *parent) :
    QObject(parent)
{

}

void CoopNetwork::set (QString host, int port)
{
    //test only ，output all avliable IP addrs
#ifdef ZZTEST
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i)
    {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                ipAddressesList.at(i).toIPv4Address())
        {
            mHost = ipAddressesList.at(i).toString();
            //                break;
            logger()->debug()<<mHost;
        }
    }
#endif
    //下面的自动获取ip地址在有v6和多网卡的情况下会出现问题
    if (host.isEmpty ())
    {
        // find out which IP to connect to
        //    QString ipAddress;
        QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
        // use the last non-localhost IPv4 address
        for (int i = ipAddressesList.size()-1;i>=0; --i)
        {
            if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                    ipAddressesList.at(i).toIPv4Address())
            {
                mHost = ipAddressesList.at(i).toString();
                break;
            }
        }
        // if we did not find one, use IPv4 localhost
        if (mHost.isEmpty())
            mHost = QHostAddress(QHostAddress::LocalHost).toString();
    }
    else
    {
        mHost=host;
    }
    mPort=port;
}

bool CoopNetwork::connectHost (networkConfigType config)
{
    if (config == CoopNetwork::DEFAULT)
    {
        mSocket = new QTcpSocket(this);
        mSocket->connectToHost (QHostAddress(mHost),mPort);
        connect (mSocket,SIGNAL(connected()),this,SLOT(connected()));
        connect(mSocket,SIGNAL(readyRead()),this,SLOT(dealData()));
        connect(mSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(errorInfo()));
        return true;
    }
    return false;
}

bool CoopNetwork::send (const char *data)
{
    //real send data
    int temp = mSocket->write(data);
    return (temp != -1)?true:false;
}

bool CoopNetwork::send (int sendType, int dataType, QString &data)
{
    //send other data
    QString temp = QString::number (sendType);
    temp.append ('\n');
    temp.append (QString::number (dataType));
    temp.append ('\n');
    temp.append (data);
    return send(temp.toStdString().c_str());
}

bool CoopNetwork::send (int sendType, QString &data)
{
    //send chat data
    QString temp = QString::number (sendType);
    temp.append ('\n');
    temp.append (data);
    return send(temp.toStdString().c_str());
//    mSocket->write (temp.toStdString().c_str());
//    return true;
}

void CoopNetwork::connected ()
{
    emit state(tr("connected!"));
}

void CoopNetwork::dealData ()
{
    QTextStream pData(mSocket);
    QString temp = pData.readAll ();
    QStringList pList = temp.split ('\n');
    qDebug ()<<pList;
    switch (pList.at(0).toInt())
    {
    case 0: break;
    case 1: break;
    case 10:
    {
        //Chat Data
        emit get (10,pList.at (1));
        break;
    }
    default: break;
    }
}

void CoopNetwork::errorInfo()
{
    emit error(mSocket->errorString());
}
