#ifndef WIDGETSETTINGS_H
#define WIDGETSETTINGS_H

#include <QWidget>

namespace Ui {
class WidgetSettings;
}

/**
 * @brief Графический класс настроек соединения
 */
class WidgetSettings : public QWidget {
    Q_OBJECT

public:
    explicit WidgetSettings(QWidget* parent = nullptr);
    ~WidgetSettings();

    /// Обновить состояние настроек и выслать сигнал текущей настройки
    void updateSettings();

signals:
    void setUdp(QString ip, int portSend, int portReceive);
    void setTcpClient(QString ip, int port);
    void setTcpServer(QString ip, int port);

    /**
     * @brief Дополнительные настройки
     * @param fEcho - вкл/выкл эхо-сервер
     */
    void signalAdditionalSetings(bool fEcho);

public slots:
    /// Сообщить о результате примененных настроек (true - применены)
    void slotSettingsIsAccept(bool result);

private:
    Ui::WidgetSettings* ui;

private slots:
    void slotUpdateSettings();

    void slotChangeSetting();

    void slotAdditionalSetting();
};

#endif // WIDGETSETTINGS_H
