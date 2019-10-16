#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QMap>
#include <QTcpServer>
#include "ethernet.h"

namespace ethernet {

//------------------------------------------------------------------------------
/**
\brief       Реализация тср-сервера.
\details     Подсоединиться может несколько клиентов
 */
class TcpServer : public Ethernet {
    Q_OBJECT
//-----------------------------------public------------------------------------
public:
    //-------methods-------//
    /// Конструктор
    explicit TcpServer( QObject* parent = NULL );
    /// Деструктор
    ~TcpServer();

    void setSettings(const QHostAddress& ipSender, int portSend,
                     int portReceive = PORT_UNASSUGNED) final override;
    void setSettings(int portManage) final override;

    bool sendData(const QByteArray& data) final override;
    void sendDataFast(const QByteArray& data) final override;

    QByteArray getData(qint64 id) final override;

    bool isConnected() const final override;

//----------------------------------private slots------------------------------
private slots:
    /// Подключился новый клиент
    void slotNewConnection();

    /// Чтение данных от клиента
    void slotReadClient();

    /// Клиент отключился
    void slotConnectionLost(); // when clien is disconnected

//-----------------------------------private-----------------------------------
private:
    void resetConnect();

    //------data------//
    /** @brief Список присоединившихся клиентов
     * @param key - номер дискриптора клиента
     * @param value - объект клиента
     * */
    QMap<qintptr, QTcpSocket*> mMyClients;

    QScopedPointer<QTcpServer> mTcp;

    qint64 mCounterReceiveData;

    bool mIsConnected;

    QHash<qint64, QByteArray> mReadedData;

};

}
#endif // TCPSERVER_H
