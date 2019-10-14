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

    // связать UDP/TCP и WidgetSettings по настройкам сети
    connect(mWidgetSettings, SIGNAL(setTcpClient(QString, int)),
            SLOT(slotCreateConnectTcpClient(QString, int)));
    connect(mWidgetSettings, SIGNAL(setTcpServer(QString, int)),
            SLOT(slotCreateConnectTcpServer(QString, int)));
    connect(mWidgetSettings, SIGNAL(setUdp(QString, int, int)),
            SLOT(slotCreateConnectUdp(QString, int, int)));

    // связать WidgetDg и mWidgetLog для вывода ошибок
    connect(mWidgetDg, SIGNAL(msg(QString)), mWidgetLog, SLOT(slotWriteInfo(QString)));

    // отправка данных
    connect(mWidgetDg, SIGNAL(sendData()), SLOT(slotSendData()));



    // TODO результат соединения

    // задать настройки соединения при запуске
    mWidgetSettings->updateSettings();
}
//------------------------------------------------------------------------------
MainWidget::~MainWidget() {
    delete ui;
}
//------------------------------------------------------------------------------
void MainWidget::slotSendData() {
    if(mEthernet.isNull()){
        return;
    }
    QByteArray dg = mWidgetDg->getDg();
    mWidgetLog->writeInfo(QString("----send dg----"));
    mWidgetLog->writeLog(dg);

    // отправка датаграммы
    mEthernet.data()->sendDataFast(dg);
}
//------------------------------------------------------------------------------
void MainWidget::slotReadData(int id){
    mWidgetLog->writeInfo(QString("----responce dg----"));
    QByteArray dg = mEthernet.data()->getData(id);
    mWidgetLog->writeLog( dg );
}
//------------------------------------------------------------------------------
void MainWidget::slotCreateConnectTcpClient(QString ip, int portManage) {
//    mEthernet.reset( new Tcp());
    // TODO
}
//------------------------------------------------------------------------------
void MainWidget::slotCreateConnectTcpServer(QString ip, int portManage) {
    // TODO
}
//------------------------------------------------------------------------------
void MainWidget::slotCreateConnectUdp(QString ip, int portSend, int portReceive) {
    mEthernet.reset( new Udp() );
    // пришли данные
    connect(mEthernet.data(), SIGNAL(signalReadData(int)), SLOT(slotReadData(int)));
    connect(mEthernet.data(), SIGNAL(signalConnectStatus(bool)),
            mWidgetSettings, SLOT(slotSettingsIsAccept(bool)));
    mEthernet.data()->setSender(ip, portSend);
    mEthernet.data()->setReceive(portReceive);
}
//------------------------------------------------------------------------------
