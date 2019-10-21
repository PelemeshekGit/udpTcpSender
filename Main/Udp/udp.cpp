#include "udp.h"

namespace ethernet {

Udp::Udp(QObject* parent) : Ethernet(parent),
    mPortSender(PORT_UNASSUGNED),
    mCounterReceiveData(0),
    mIsConnected(false) {

    setTypeClass(TypeDerivedClass::Udp);

    mUdp.reset( new QUdpSocket(parent) );

    connect(mUdp.data(), SIGNAL(readyRead()), SLOT(slotReadData()));
}
//------------------------------------------------------------------------------
Udp::~Udp() {
    mUdp.data()->abort();
}
//------------------------------------------------------------------------------
void Udp::setSettings(const QHostAddress& ipSender, int portSend, int portReceive) {
    setSender(ipSender, portSend);

    if (portReceive != PORT_UNASSUGNED) {
        setReceive(portReceive);
    }
}
//------------------------------------------------------------------------------
void Udp::setSettings(int portManage) {
    setReceive(portManage);
}
//------------------------------------------------------------------------------
void Udp::setSender(const QHostAddress& ip, int portSend) {
    mAddressSender = ip;
    mPortSender = portSend;
    mIsConnected = true;
}
//------------------------------------------------------------------------------
bool Udp::sendData(const QByteArray& data) {
    if (mPortSender == PORT_UNASSUGNED) {
        return false;
    }

    qint64 size = mUdp.data()->writeDatagram(data,
                  mAddressSender,
                  mPortSender);

    return (size == data.size());
}
//------------------------------------------------------------------------------
void Udp::sendDataFast(const QByteArray& data) {
    if (mPortSender == PORT_UNASSUGNED) {
        return;
    }

    mUdp.data()->writeDatagram(std::move(data),
                               mAddressSender,
                               mPortSender);
}
//------------------------------------------------------------------------------
void Udp::setReceive(int portReceive) {
    resetConnect();

    mIsConnected = mUdp.data()->bind(QHostAddress::Any, portReceive, QUdpSocket::ShareAddress);
    emit signalConnectStatus(mIsConnected);
}
//------------------------------------------------------------------------------
void Udp::setReceive(const QString& ip, int portReceive) {
    resetConnect();

    mIsConnected = mUdp.data()->bind(QHostAddress(ip), portReceive, QUdpSocket::ShareAddress);
    emit signalConnectStatus(mIsConnected);
}
//------------------------------------------------------------------------------
void Udp::resetConnect() {
    mUdp.data()->close();
}
//------------------------------------------------------------------------------
void Udp::slotReadData() {
    QByteArray readData;

    while (mUdp.data()->hasPendingDatagrams()) {
        readData.resize(mUdp.data()->pendingDatagramSize());
        mUdp.data()->readDatagram(readData.data(), readData.size());
    }

    mCounterReceiveData++;
    mReadedData.insert(mCounterReceiveData, std::move(readData) );

    // режим эхо-сервера
    if (getEchoServer()) {
        emit signalMsg(QString("echo dg"));
        sendDataFast(mReadedData[mCounterReceiveData]);
    }

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
bool Udp::isConnected() const {
    return mIsConnected;
}
//------------------------------------------------------------------------------

}
