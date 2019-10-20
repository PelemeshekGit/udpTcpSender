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

    /**
     * @brief Отправка данных конкретному клиенту с проверкой отправлены ли данные
     * @param data - данные
     * @param descriptor - уникальный ключ клиента кому отправляется посылка, -1 - отправка всем
     * @return - результат отправки (true - отправлено хотя бы одному клиенту, false - не отправлено всем)
     */
    bool sendData(const QByteArray& data, const qintptr& descriptor);

    void sendDataFast(const QByteArray& data) final override;

    /**
     * @brief Отправка данных конкретному клиенту данных без проверки. Быстрее чем sendData()
     * @param data - данные
     * @param descriptor - уникальный ключ клиента кому отправляется посылка, -1 - отправка всем
     * @return - результат отправки (true - отправлено, false - не отправлено)
     */
    void sendDataFast(const QByteArray& data, const qintptr& descriptor);

    QByteArray getData(qint64 id) final override;

    bool isConnected() const final override;

    /**
     * @brief Включение режима ответа клиентом того что они отправляют
     * @param on - true - включить переотправку, false - отключить
     * @details по умолчанию false
     */
    void setEchoServer(bool on);

//-------------------------------------signals----------------------------------
signals:
    /// Присоединился новый клиент
    void signalClientConnected(qintptr);

    /// Клиент отсоединился
    void signalClientDisconnected(qintptr);

//----------------------------------private slots------------------------------
private slots:
    /// Подключился новый клиент
    void slotNewConnection();

    /// Чтение данных от клиента
    void slotReadClient();

    /// Клиент отключился
    void slotConnectionLost();

//-----------------------------------private-----------------------------------
private:
    void resetConnect();

    //------data------//
    /** @brief Список присоединившихся клиентов (карта сети)
     * @param key - номер дискриптора клиента
     * @param value - объект клиента
     * */
    QMap<qintptr, QTcpSocket*> mMyClients;

    QScopedPointer<QTcpServer> mTcp;

    qint64 mCounterReceiveData;

    bool mIsConnected;

    QHash<qint64, QByteArray> mReadedData;

    bool mEchoServer;

};

}
#endif // TCPSERVER_H
