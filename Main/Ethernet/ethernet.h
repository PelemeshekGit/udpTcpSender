#ifndef ETHERNET_H
#define ETHERNET_H

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QtNetwork/QAbstractSocket>

class Ethernet : public QObject {
    Q_OBJECT
public:
    explicit Ethernet(QObject* parent);
    virtual ~Ethernet();

    /**
     * @brief Настройка отправщика данных
     * @param ip - адрес
     * @param portSend - порт отправки
     */
    virtual void setSender(const QString& ip, int portSend) = 0;

    /**
     * @brief Отправка данных с проверкой отправлены ли данные
     * @param data - данные
     * @return - результат отправки (true - отправлено, false - не отправлено)
     */
    virtual bool sendData(const QByteArray& data) = 0;

    /**
     * @brief Отправка данных без проверки. Быстрее чем sendData()
     * @param data - данные
     * @return - результат отправки (true - отправлено, false - не отправлено)
     */
    virtual void sendDataFast(const QByteArray& data) = 0;

    /**
     * @brief Перегруженный метод. Настройка приемника данных
     * @param portReceive - порт приема
     * @return - удалось ли создать приемник данных (true - всё ок)
     */
    virtual bool setReceive(int portReceive) = 0;
    /**
     * @brief Перегруженный метод. Настройка приемника данных
     * @param ip - адрес
     * @param portReceive - порт приема
     * @return - удалось ли создать приемник данных (true - всё ок)
     */
    virtual bool setReceive(const QString& ip, int portReceive) = 0;

    /**
     * @brief Статус создания соединения (true - соединения создано успешно,
     * false - соединения не создано)
     */
    virtual bool isConnected() = 0;

    /**
     * @brief Получить принятые данные
     * @param id - идентификатор данных
     */
    virtual QByteArray getData(qint64 id) = 0;

signals:
    /// Пришли данные и готовы для чтения
    void signalReadData(int id);

    /// Статус соединения
    void signalConnectStatus(bool);
};

#endif // ETHERNET_H
