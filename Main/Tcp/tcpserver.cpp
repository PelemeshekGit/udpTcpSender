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

    connect( mTcp.data(), SIGNAL(newConnection()), SLOT(slotNewConnection()) );
}
//------------------------------------------------------------------------------
TcpServer::~TcpServer() {
    for (const auto& i : mMyClients.keys()) {
        if (mMyClients[i]->isOpen()) {
            mMyClients[i]->disconnect(SIGNAL(disconnected()));
            mMyClients[i]->disconnect(SIGNAL(readyRead()));
            mMyClients[i]->close();
        }

        mMyClients[i]->deleteLater();
        mMyClients.remove(i);
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
    return sendData(data, -1);
}
//------------------------------------------------------------------------------
bool TcpServer::sendData(const QByteArray& data, const qintptr& descriptor) {
    if (!isConnected()) {
        return false;
    }

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
    sendDataFast(data, -1);
}
//------------------------------------------------------------------------------
void TcpServer::sendDataFast(const QByteArray& data, const qintptr& descriptor) {
    if (!isConnected()) {
        return;
    }

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

        mMyClients.insert(idusersocs, clientSock);
        connect( mMyClients[idusersocs], SIGNAL(disconnected()), SLOT(slotConnectionLost()) );
        connect( mMyClients[idusersocs], SIGNAL(readyRead()), SLOT(slotReadClient()));
        emit signalClientConnected( idusersocs );
    }
}
//------------------------------------------------------------------------------
void TcpServer::slotReadClient() {
    // Получение объекта, который вызвал данный слот
    QTcpSocket* clientSocket = dynamic_cast<QTcpSocket*>(sender());

    mCounterReceiveData++;
    mReadedData.insert(mCounterReceiveData, clientSocket->readAll());

    // режим эхо-сервера
    if (getEchoServer()) {
        emit signalMsg(QString("echo dg"));
        sendDataFast(mReadedData[mCounterReceiveData], clientSocket->socketDescriptor());
    }

    emit signalReadData(mCounterReceiveData);
}
//------------------------------------------------------------------------------
void TcpServer::slotConnectionLost() {
    // Получение объекта, который вызвал данный слот
    QTcpSocket* clientSocket = dynamic_cast<QTcpSocket*>(sender());

    clientSocket->close();
    clientSocket->deleteLater();

    // Удалим объект сокета из карты сети
    for (auto& obj : mMyClients.values()) {
        if (obj == clientSocket) {
            // нахождение уникального ключа (ищем так, потому что дескриптор уже имеет значение -1)
            qintptr idusersocs = mMyClients.key(obj);
            mMyClients.remove(idusersocs);
            emit signalClientDisconnected( idusersocs );
            break;
        }
    }
}
//------------------------------------------------------------------------------
void TcpServer::resetConnect() {
    if ( mTcp.data()->isListening() ) {
        mTcp.data()->close();
    }
}
//------------------------------------------------------------------------------

}
