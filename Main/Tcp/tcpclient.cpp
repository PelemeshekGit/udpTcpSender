#include "tcpclient.h"
#include <QHostAddress>

namespace ethernet {

TcpClient::TcpClient(QObject* parent) :
    Ethernet(parent),
    mCounterReceiveData(0),
    mIsConnected(false),
    mWaitForConnected(2000) {

    setTypeClass(TypeDerivedClass::TcpClient);

    mTcp.reset( new QTcpSocket(parent) );

    connect(mTcp.data(), SIGNAL(connected()), SLOT(slotConnected()));
    connect(mTcp.data(), SIGNAL(disconnected()), SLOT(slotDisconnected()));
    connect(mTcp.data(), SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(mTcp.data(), SIGNAL(error(QAbstractSocket::SocketError)),
            SLOT(slotError(QAbstractSocket::SocketError)));
}
//------------------------------------------------------------------------------
TcpClient::~TcpClient() {
    mTcp.data()->abort();
}
//------------------------------------------------------------------------------
void TcpClient::setSettings(const QHostAddress& ipSender, int portSend, int portReceive) {
    Q_UNUSED(portReceive);
    resetConnect();
    // если не произошло соединения в течении двух секунд,
    // то не пытаться ждать дольше
    mTcp.data()->waitForConnected(mWaitForConnected);

    mTcp.data()->connectToHost( ipSender, portSend );
}
//------------------------------------------------------------------------------
void TcpClient::setSettings(int portManage) {
    setSettings(QHostAddress::Any, portManage);
}
//------------------------------------------------------------------------------
bool TcpClient::sendData(const QByteArray& data) {
    int sizeSendData = static_cast<int>( mTcp.data()->write( data ) );

    if ( sizeSendData != data.size() ) {
        return false;
    } else {
        return true;
    }
}
//------------------------------------------------------------------------------
void TcpClient::sendDataFast(const QByteArray& data) {
    mTcp.data()->write( data );
}
//------------------------------------------------------------------------------
QByteArray TcpClient::getData(qint64 id) {
    if (mReadedData.contains(id)) {
        return mReadedData.take(id);
    } else {
        return QByteArray();
    }
}
//------------------------------------------------------------------------------
bool TcpClient::isConnected() const {
    return mIsConnected;
}
//------------------------------------------------------------------------------
qintptr TcpClient::getDescriptor() const {
    if (!mTcp.isNull()) {
        return mTcp.data()->socketDescriptor();
    } else {
        return qintptr();
    }
}
//------------------------------------------------------------------------------
void TcpClient::setWaitForConnected(int msecs) {
    mWaitForConnected = msecs;
}
//------------------------------------------------------------------------------
void TcpClient::slotConnected() {
    mIsConnected = true;
    emit signalConnectStatus(mIsConnected);
}
//------------------------------------------------------------------------------
void TcpClient::slotDisconnected() {
    mIsConnected = false;
    emit signalConnectStatus(mIsConnected);
    emit signalErrorMsg(QString("TCP Client is disconnected"));
}
//------------------------------------------------------------------------------
void TcpClient::slotError(QAbstractSocket::SocketError err) {
    emit signalConnectStatus(false);

    mIsConnected = false;

    QString errMess;

    switch (err) {
    case QAbstractSocket::RemoteHostClosedError: {
        errMess = QString::fromUtf8("Connected is closed");
        break;
    }

    case QAbstractSocket::HostNotFoundError: {
        errMess = QString::fromUtf8("Time wait connect is away");
        break;
    }

    case QAbstractSocket::ConnectionRefusedError: {
        errMess = QString::fromUtf8("Connect is break (timeout connect)");
        break;
    }

    case QAbstractSocket::NetworkError: {
        errMess = QString::fromUtf8("Error network");
        break;
    }

    default: {
        errMess = mTcp.data()->errorString();
        break;
    }
    }

    emit signalErrorMsg( errMess );
}
//------------------------------------------------------------------------------
void TcpClient::slotReadyRead() {
    QByteArray readData = mTcp.data()->readAll();

    mCounterReceiveData++;
    mReadedData.insert(mCounterReceiveData, readData);
    emit signalReadData(mCounterReceiveData);
}
//------------------------------------------------------------------------------
void TcpClient::resetConnect() {
    mTcp.data()->close();
}
//------------------------------------------------------------------------------

}
