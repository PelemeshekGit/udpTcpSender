#include "widgetsettings.h"
#include "ui_widgetsettings.h"
#include <QRegExp>
#include "indicators.h"
#include <QDebug>
using namespace Widget;

WidgetSettings::WidgetSettings(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::WidgetSettings) {
    ui->setupUi(this);

    // настройки для ввода IP адреса
    ui->edIp->setValidator(new QRegExpValidator(QRegExp("\\d{1,3}[.]\\d{1,3}[.]\\d{1,3}[.]\\d{1,3}"),
                           0));

    slotChangeSetting();

    connect(ui->rbSettingsUdp, SIGNAL(clicked(bool)), SLOT(slotChangeSetting()));
    connect(ui->rbSettingsTcp, SIGNAL(clicked(bool)), SLOT(slotChangeSetting()));
    connect(ui->rbSettingsTcpClient, SIGNAL(clicked(bool)), SLOT(slotChangeSetting()));
    connect(ui->rbSettingsTcpServer, SIGNAL(clicked(bool)), SLOT(slotChangeSetting()));

    connect(ui->edIp, SIGNAL(editingFinished()), SLOT(slotUpdateSettings()));
    connect(ui->edPortTcp, SIGNAL(editingFinished()), SLOT(slotUpdateSettings()));
    connect(ui->edPortSend, SIGNAL(editingFinished()), SLOT(slotUpdateSettings()));
    connect(ui->edPortReceive, SIGNAL(editingFinished()), SLOT(slotUpdateSettings()));

    connect(ui->btnReconnect, SIGNAL(clicked(bool)), SLOT(slotChangeSetting()));
}
//------------------------------------------------------------------------------
WidgetSettings::~WidgetSettings() {
    delete ui;
}
//------------------------------------------------------------------------------
void WidgetSettings::updateSettings() {
    slotChangeSetting();
}
//------------------------------------------------------------------------------
void WidgetSettings::slotSettingsIsAccept(bool result) {
    if (result) {
        ui->imgResultConnect->setPixmap( *INDICATOR.getOk() );
    } else {
        ui->imgResultConnect->setPixmap( *INDICATOR.getError() );
    }
}
//------------------------------------------------------------------------------
void WidgetSettings::slotChangeSetting() {
    ui->widgetTcpManager->setVisible(ui->rbSettingsTcp->isChecked());
    ui->framPortTcp->setVisible(ui->rbSettingsTcp->isChecked());
    ui->framePortUdpSend->setVisible(ui->rbSettingsUdp->isChecked());
    ui->framePortUdpReceive->setVisible(ui->rbSettingsUdp->isChecked());

    slotUpdateSettings();
}
//------------------------------------------------------------------------------
void WidgetSettings::slotUpdateSettings() {
    ui->imgResultConnect->setPixmap( *INDICATOR.getCurrent() );

    if (ui->rbSettingsUdp->isChecked()) {
        emit setUdp(ui->edIp->text(), ui->edPortSend->value(), ui->edPortReceive->value());
    } else if (ui->rbSettingsTcp->isChecked()) {
        if (ui->rbSettingsTcpClient->isChecked()) {
            emit setTcpClient(ui->edIp->text(), ui->edPortTcp->value());
        } else if (ui->rbSettingsTcpServer->isChecked()) {
            emit setTcpServer(ui->edIp->text(), ui->edPortTcp->value());
        }
    }
}
//------------------------------------------------------------------------------
