#include "udp.h"

Udp::Udp(QObject* parent) : Ethernet(parent) {
    mUdp.reset( new QUdpSocket() );

    connect(mUdp.data(), SIGNAL(readyRead()), SLOT(slotReadData()));
}
//------------------------------------------------------------------------------
Udp::~Udp() {
    mUdp.data()->close();
}
//------------------------------------------------------------------------------
void Udp::setSender(const QString& ip, int portSend) {
    mAddressSender = QHostAddress(ip);
    mPortSender = portSend;
    mIsConnected = true;
}
//------------------------------------------------------------------------------
bool Udp::sendData(const QByteArray& data) {
    if (mPortSender != PORT_UNASSUGNED) {
        return false;
    }

    qint64 size =  mUdp.data()->writeDatagram(data,
                   mAddressSender,
                   mPortSender);
    return (size == data.size());
}
//------------------------------------------------------------------------------
void Udp::sendDataFast(const QByteArray& data) {
    if (mPortSender != PORT_UNASSUGNED) {
        return;
    }

    mUdp.data()->writeDatagram(data,
                               mAddressSender,
                               mPortSender);
}
//------------------------------------------------------------------------------
bool Udp::setReceive(int portReceive) {
    resetConnect();

    mIsConnected = mUdp.data()->bind(QHostAddress::Any, portReceive, QUdpSocket::ShareAddress);
    emit signalConnectStatus(mIsConnected);
    return mIsConnected;
}
//------------------------------------------------------------------------------
bool Udp::setReceive(const QString& ip, int portReceive) {
    resetConnect();

    mIsConnected = mUdp.data()->bind(QHostAddress(ip), portReceive, QUdpSocket::ShareAddress);
    emit signalConnectStatus(mIsConnected);
    return mIsConnected;
}
//------------------------------------------------------------------------------
void Udp::resetConnect() {
    if ( mUdp.data()->isOpen() ) {
        mUdp.data()->close();
    }
}
//------------------------------------------------------------------------------
void Udp::slotReadData() {
    QByteArray readData = mUdp.data()->readAll();
    mCounterReceiveData++;
    mReadedData.insert(mCounterReceiveData, readData);
    emit signalReadData(mCounterReceiveData);
}
//------------------------------------------------------------------------------
QByteArray Udp::getData(qint64 id) {
    if (mReadedData.contains(id)) {
        return mReadedData.take(id);
    } else {
        return QByteArray();
    }
}
//------------------------------------------------------------------------------
bool Udp::isConnected() {
    return mIsConnected;
}
//------------------------------------------------------------------------------
