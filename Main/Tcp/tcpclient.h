#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include "ethernet.h"

namespace ethernet {

//------------------------------------------------------------------------------
/*!
 * @brief   Реализация тср-клиента.
 * @details Является одним клиентом из множества. При соединении с сервером
 * получает свой id и по нему общается
 */
class TcpClient : public Ethernet {
    Q_OBJECT
//-----------------------------------public------------------------------------
public:
    //-------methods-------//
    /// Конструктор
    explicit TcpClient( QObject* parent = NULL );
    /// Деструктор
    ~TcpClient();

    void setSettings(const QHostAddress& ipSender, int portSend,
                     int portReceive = PORT_UNASSUGNED) final override;
    void setSettings(int portManage) final override;

    bool sendData(const QByteArray& data) final override;
    void sendDataFast(const QByteArray& data) final override;

    QByteArray getData(qint64 id) final override;

    bool isConnected() const final override;

    qintptr getDescriptor() const;

//--------------------------------private slots---------------------------------
private slots:
    /// Ошибка соединения
    void slotError( QAbstractSocket::SocketError err );

    /// Соединение установлено
    void slotConnected();

    /// Соединение разорвано
    void slotDisconnected();

    /// Прочитать входные данные
    void slotReadyRead();

//-----------------------------------private-----------------------------------
private:
    void resetConnect();


    QScopedPointer<QTcpSocket> mTcp;

    qint64 mCounterReceiveData;

    bool mIsConnected;

    QHash<qint64, QByteArray> mReadedData;

};

}
#endif // TCPCLIENT_H
