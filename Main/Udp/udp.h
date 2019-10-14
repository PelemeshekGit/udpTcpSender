#ifndef UDP_H
#define UDP_H

#include <QScopedPointer>
#include <QUdpSocket>
#include "ethernet.h"

static const int PORT_UNASSUGNED = -1;

class Udp : public Ethernet {
    Q_OBJECT
public:
    explicit Udp(QObject* parent = nullptr);
    ~Udp();
    void setSender(const QString& ip, int portSend) final override;

    bool sendData(const QByteArray& data) final override;
    void sendDataFast(const QByteArray& data) final override;

    bool setReceive(int portReceive) final override;
    bool setReceive(const QString& ip, int portReceive) final override;

    QByteArray getData(qint64 id);

    bool isConnected();

private slots:
    void slotReadData();

private:
    void resetConnect();


    QScopedPointer<QUdpSocket> mUdp;

    QHostAddress mAddressSender;
    /// Порт отправки, по умолчанию не назначен, необходим для проверки была ли инициализация через setSender()
    int mPortSender = PORT_UNASSUGNED;

    qint64 mCounterReceiveData = 0;

    bool mIsConnected = false;

    QHash<qint64, QByteArray> mReadedData;
};

#endif // UDP_H
