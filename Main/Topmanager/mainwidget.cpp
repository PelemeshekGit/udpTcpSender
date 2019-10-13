#include "mainwidget.h"
#include "ui_mainwidget.h"

#include "udp.h"
#include "tcp.h"

MainWidget::MainWidget(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::MainWidget) {
    ui->setupUi(this);

    mWidgetLog = new WidgetLog;
    ui->layoutLog->addWidget(mWidgetLog);

    mWidgetDg = new WidgetDg;
    ui->layoutDg->addWidget(mWidgetDg);

    mWidgetSettings = new WidgetSettings;
    ui->layoutSettings->addWidget(mWidgetSettings);

    ui->tabWidget->setCurrentIndex(0);

    // TODO связать UDP/TCP и WidgetSettings по настройкам сети
    connect(mWidgetSettings, SIGNAL(setTcpClient(QString, int)),
            SLOT(slotCreateConnectTcpClient(QString, int)));
    connect(mWidgetSettings, SIGNAL(setTcpServer(QString, int)),
            SLOT(slotCreateConnectTcpServer(QString, int)));
    connect(mWidgetSettings, SIGNAL(setUdp(QString, int, int)),
            SLOT(slotCreateConnectUdp(QString, int, int)));

    // связать WidgetDg и mWidgetLog для вывода в лог
    connect(mWidgetDg, SIGNAL(msg(QString)), mWidgetLog, SLOT(slotWriteInfo(QString)));

    // TODO добавить классы отправки и приема данных и связать их с mWidgetLog
    connect(mWidgetDg, SIGNAL(sendData()), SLOT(slotSendData()));

}
//------------------------------------------------------------------------------
MainWidget::~MainWidget() {
    delete ui;
}
//------------------------------------------------------------------------------
void MainWidget::slotSendData() {
    QByteArray dg = mWidgetDg->getDg();
    mWidgetLog->writeInfo(QString("----send dg----"));
    mWidgetLog->writeLog(dg);

    // TODO отправка датаграммы
}
//------------------------------------------------------------------------------
void MainWidget::slotCreateConnectTcpClient(QString ip, int portManage) {
//    mEthernet.reset( new Tcp());
}
//------------------------------------------------------------------------------
void MainWidget::slotCreateConnectTcpServer(QString ip, int portManage) {

}
//------------------------------------------------------------------------------
void MainWidget::slotCreateConnectUdp(QString ip, int portSend, int portReceive) {
    mEthernet.reset( new Udp() );
    mEthernet.data()->setSender(ip, portSend);
    mEthernet.data()->setReceive(portReceive);
}
//------------------------------------------------------------------------------
