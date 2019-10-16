#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QScopedPointer>

#include "widgetdg.h"
#include "widgetlog.h"
#include "widgetsettings.h"
#include "ethernet.h"

namespace Ui {
class MainWidget;
}
/**
 * @brief Основной класс отображения и управления
 */
class MainWidget : public QWidget {
    Q_OBJECT

public:
    explicit MainWidget(QWidget* parent = 0);
    ~MainWidget();

private:
    Ui::MainWidget* ui;

    WidgetDg* mWidgetDg;
    WidgetLog* mWidgetLog;
    WidgetSettings* mWidgetSettings;

    QScopedPointer<ethernet::Ethernet> mEthernet;

    void createConnect(const ethernet::TypeDerivedClass& typeConnect, QString ip,
                       int portManage, int portReceive = ethernet::PORT_UNASSUGNED);

private slots:
    void slotSendData();
    void slotReadData(int id);

    void slotCreateConnectTcpClient(QString ip, int portManage);
    void slotCreateConnectTcpServer(QString ip, int portManage);
    void slotCreateConnectUdp(QString, int, int portReceive);

};

#endif // MAINWIDGET_H
