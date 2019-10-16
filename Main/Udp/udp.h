#ifndef UDP_H
#define UDP_H

#include <QScopedPointer>
#include <QUdpSocket>
#include "ethernet.h"

namespace ethernet {

class Udp : public Ethernet {
    Q_OBJECT
//-----------------------------------public-----------------------------------
public:
    explicit Udp(QObject* parent = nullptr);
    ~Udp();
    void setSettings(const QHostAddress& ipSender, int portSend,
                     int portReceive = PORT_UNASSUGNED) final override;
    void setSettings(int portManage) final override;

    bool sendData(const QByteArray& data) final override;
    void sendDataFast(const QByteArray& data) final override;

    QByteArray getData(qint64 id) final override;

    bool isConnected() const final override;

    /**
     * @brief Настройка отправщика данных
     * @param ip - адрес отправки
     * @param portSend - порт отправки
     */
    void setSender(const QHostAddress& ip, int portSend);

    /**
     * @brief Настройка приемника данных
     * @param portReceive - порт приема, адрес QHostAddress::Any
     */
    void setReceive(int portReceive);

    /**
     * @brief Настройка приемника данных
     * @param ip - адрес приемника данных
     * @param portReceive - порт приема
     */
    void setReceive(const QString& ip, int portReceive);

//--------------------------------private slots---------------------------------
private slots:
    void slotReadData();

//-----------------------------------private-----------------------------------
private:
    void resetConnect();


    QScopedPointer<QUdpSocket> mUdp;

    QHostAddress mAddressSender;
    /// Порт отправки, по умолчанию не назначен, необходим для проверки была ли инициализация через setSender()
    int mPortSender;

    qint64 mCounterReceiveData;

    bool mIsConnected = false;

    QHash<qint64, QByteArray> mReadedData;
};

}
#endif // UDP_H
