#ifndef ETHERNET_H
#define ETHERNET_H

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QtNetwork/QAbstractSocket>
#include "typederivedclass.h"

namespace ethernet {

static const int PORT_UNASSUGNED = -1;

class Ethernet : public QObject {
    Q_OBJECT
public:
    explicit Ethernet(QObject* parent);
    virtual ~Ethernet();

    /**
     * @brief Настройка отправщика и приемника данных
     * @param ip - адрес отправки (UDP) или Сервера (TCP)
     * @param portSend - порт отправки (UDP) или управления (TCP)
     * @param portReceive - порт приема данных (UDP), для TCP не используется
     */
    virtual void setSettings(const QHostAddress& ipSender, int portSend,
                             int portReceive = PORT_UNASSUGNED) = 0;

    /**
     * @brief Настройка приемника данных
     * @param portManage - порт приема данных (UDP) или управления (TCP)
     * @details по умолчанию устанавливается ip-адрес QHostAddress::Any
     */
    virtual void setSettings(int portManage) = 0;

    /**
     * @brief Отправка данных с проверкой отправлены ли данные
     * @param data - данные
     * @return - результат отправки (true - отправлено, false - не отправлено)
     */
    virtual bool sendData(const QByteArray& data) = 0;

    /**
     * @brief Отправка данных без проверки. Быстрее чем sendData()
     * @param data - данные
     */
    virtual void sendDataFast(const QByteArray& data) = 0;

    /**
     * @brief Получить принятые данные
     * @param id - идентификатор данных
     */
    virtual QByteArray getData(qint64 id) = 0;

    /**
     * @brief Статус создания соединения (true - соединения создано успешно,
     * false - соединения не создано)
     */
    virtual bool isConnected() const = 0;

    /**
     * @brief Включение режима ответа того что было принято
     * @param on - true - включить переотправку, false - отключить
     * @details по умолчанию false
     */
    void setEchoServer(bool on);
    bool getEchoServer() const;

    /// возвращает тип производного класса
    TypeDerivedClass getTypeClass() const;
    void setTypeClass(const TypeDerivedClass& type);

signals:
    /// Пришли данные и готовы для чтения
    void signalReadData(int id);

    /// Статус соединения как у isConnected()
    void signalConnectStatus(bool);

    /// Сообщение с ошибкой
    void signalErrorMsg(QString);

    /// Сообщение с информацией, отличной от ошибки
    void signalMsg(QString);

protected:
    TypeDerivedClass mTypeClass = TypeDerivedClass::Undefined;

private:
    bool mEchoServer = false;

};

}
#endif // ETHERNET_H
