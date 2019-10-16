#include "tcpserver.h"

#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

namespace ethernet {

TcpServer::TcpServer(QObject* parent) :
    Ethernet(parent),
    mCounterReceiveData(0),
    mIsConnected(false) {

    setTypeClass(TypeDerivedClass::TcpServer);

    mTcp.reset( new QTcpServer(parent) );

    connect( mTcp.data(), SIGNAL(newConnection()),
             SLOT(slotNewConnection()) );
}
//------------------------------------------------------------------------------
TcpServer::~TcpServer() {
    for (const auto& i : mMyClients.keys()) {
        if (mMyClients[i]->isOpen()) {
            mMyClients[i]->close();
        } else {
            mMyClients.remove(i);
        }
    }

    mTcp.data()->close();
}
//------------------------------------------------------------------------------
void TcpServer::setSettings(const QHostAddress& ipSender, int portSend, int portReceive) {
    Q_UNUSED(portReceive);

    resetConnect();

    if ( mTcp.data()->listen(ipSender, portSend) ) {
        mIsConnected = true;
    } else {
        mIsConnected = false;
        mTcp.data()->close();
        emit signalErrorMsg(mTcp.data()->errorString());
    }

    emit signalConnectStatus(mIsConnected);
}
//------------------------------------------------------------------------------
void TcpServer::setSettings(int portManage) {
    setSettings(QHostAddress::Any, portManage);
}
//------------------------------------------------------------------------------
bool TcpServer::sendData(const QByteArray& data) {
    qintptr descriptor = -1;

    bool fResult = false;

    // если не указан дескриптор, значит отправляем всем
    if (descriptor == -1) {
        for (const auto& descr : mMyClients.keys()) {
            auto sizeSend = mMyClients[descr]->write( data );

            if ( sizeSend == data.size() ) {
                fResult = true;
            }
        }
    } else {
        for (const auto& descr : mMyClients.keys()) {
            if ( descr == descriptor) {
                auto sizeSend = mMyClients[descr]->write( data );

                if ( sizeSend == data.size() ) {
                    fResult = true;
                }
            }
        }
    }

    // указан неверный дескриптор, данные не отправлены
    return fResult;
}
//------------------------------------------------------------------------------
void TcpServer::sendDataFast(const QByteArray& data) {
    qintptr descriptor = -1;

    // если не указан дескриптор, значит отправляем всем
    if (descriptor == -1) {
        for (const auto& descr : mMyClients.keys()) {
            mMyClients[descr]->write( data );
        }
    } else {
        for (const auto& descr : mMyClients.keys()) {
            if ( descr == descriptor) {
                mMyClients[descr]->write( data );
            }
        }
    }
}
//------------------------------------------------------------------------------
QByteArray TcpServer::getData(qint64 id) {
    if (mReadedData.contains(id)) {
        return mReadedData.take(id);
    } else {
        return QByteArray();
    }
}
//------------------------------------------------------------------------------
bool TcpServer::isConnected() const {
    return mIsConnected;
}
//------------------------------------------------------------------------------
void TcpServer::slotNewConnection() {
    while (true) {
        QTcpSocket* clientSock = mTcp.data()->nextPendingConnection();

        if (clientSock == nullptr) {
            break;
        }

        qintptr idusersocs = clientSock->socketDescriptor();
        mMyClients.insert(idusersocs,  clientSock);
        connect( mMyClients[idusersocs], SIGNAL(disconnected()),
                 SLOT(slotConnectionLost()) );
        connect( mMyClients[idusersocs], SIGNAL(readyRead()),
                 SLOT(slotReadClient()));
//        emit signalConnected( QString::number(idusersocs) );
    }
}
//------------------------------------------------------------------------------
void TcpServer::slotReadClient() {
    // Получение объекта, который вызвал данный слот
    QTcpSocket* clientSocket = (QTcpSocket*)sender();

    // TODO запись уникального ключа
    //auto nowDescr = clientSocket->socketDescriptor();

    mCounterReceiveData++;
    mReadedData.insert(mCounterReceiveData, clientSocket->readAll());
    emit signalReadData(mCounterReceiveData);
}
//------------------------------------------------------------------------------
void TcpServer::slotConnectionLost() {
    // Получение объекта, который вызвал данный слот
    QTcpSocket* clientSocket = dynamic_cast<QTcpSocket*>(sender());
    // нахождение уникального ключа
    qintptr idusersocs;

    for (const auto& descr : mMyClients.keys()) {
        if (mMyClients[descr] == clientSocket) {
            idusersocs = descr;
        }
    }

//    emit signalDisconnected( QString::number(idusersocs) );

    clientSocket->close();
    clientSocket->deleteLater();
    // Удалим объект сокета из карты
    mMyClients.remove(idusersocs);
    // пересоздание подключения
//    QHostAddress address;

//    if ( itsIp.isEmpty() ) {
//        address = QHostAddress::Any;
//    } else {
//        address.setAddress( itsIp );
//    }

//    if ( !mTcp.data()->listen(address, itsPort) ) {
//        mTcp.data()->close();
//        emit signalErrorMsg(mTcp.data()->errorString());
//    }
}
//------------------------------------------------------------------------------
void TcpServer::resetConnect() {
    if ( mTcp.data()->isListening() ) {
        mTcp.data()->close();
    }
}
//------------------------------------------------------------------------------

}
