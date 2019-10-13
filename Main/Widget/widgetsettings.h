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

signals:
    void setUdp(QString ip, int portSend, int portReceive);
    void setTcpClient(QString ip, int port);
    void setTcpServer(QString ip, int port);

public slots:
    /// TODO Сообщить о результате примененных настроек (true - применены)
    void slotSettingsIsAccept(bool result);

private:
    Ui::WidgetSettings* ui;

private slots:
    void slotUpdateSettings();

    void slotChangeSetting();
};

#endif // WIDGETSETTINGS_H
