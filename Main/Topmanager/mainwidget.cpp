#include "mainwidget.h"
#include "ui_mainwidget.h"

#include "udp.h"
#include "tcpclient.h"
#include "tcpserver.h"

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


    // задать настройки соединения при запуске
    mWidgetSettings->updateSettings();
}
//------------------------------------------------------------------------------
MainWidget::~MainWidget() {
    delete ui;
}
//------------------------------------------------------------------------------
void MainWidget::slotSendData() {
    if (mEthernet.isNull()) {
        return;
    }

    QByteArray dg = mWidgetDg->getDg();
    mWidgetLog->writeInfo(QString("----send dg----"));
    mWidgetLog->writeLog(dg);

    // отправка датаграммы
    bool result = mEthernet.data()->sendData(dg);

    // результат отправки данных
    if (result) {
        mWidgetDg->setInicatorStatus(STATUS_INDICATOR::Ok);
    } else {
        mWidgetDg->setInicatorStatus(STATUS_INDICATOR::Error);
    }
}
//------------------------------------------------------------------------------
void MainWidget::slotReadData(int id) {
    mWidgetLog->writeInfo(QString("----responce dg----"));
    QByteArray dg = mEthernet.data()->getData(id);
    mWidgetLog->writeLog( dg );
}
//------------------------------------------------------------------------------
void MainWidget::slotCreateConnectTcpClient(QString ip, int portManage) {
    createConnect(ethernet::TypeDerivedClass::TcpClient, ip, portManage);
}
//------------------------------------------------------------------------------
void MainWidget::slotCreateConnectTcpServer(QString ip, int portManage) {
    createConnect(ethernet::TypeDerivedClass::TcpServer, ip, portManage);
}
//------------------------------------------------------------------------------
void MainWidget::slotCreateConnectUdp(QString ip, int portSend, int portReceive) {
    createConnect(ethernet::TypeDerivedClass::Udp, ip, portSend, portReceive);
}
//------------------------------------------------------------------------------
void MainWidget::createConnect(const ethernet::TypeDerivedClass& typeConnect,
                               QString ip, int portManage, int portReceive) {
    // если объект уже создан, то просто обновить настройки
    bool fCreateSocket = mEthernet.isNull();

    if (!fCreateSocket) {   // сокет создан, проверяем кому он принадлежит
        fCreateSocket = (mEthernet.data()->getTypeClass() != typeConnect);
    }

    if (fCreateSocket) {
        if (typeConnect == ethernet::TypeDerivedClass::Udp) {
            mEthernet.reset( new ethernet::Udp() );
        } else if (typeConnect == ethernet::TypeDerivedClass::TcpClient) {
            mEthernet.reset( new ethernet::TcpClient() );
        } else if (typeConnect == ethernet::TypeDerivedClass::TcpServer) {
            mEthernet.reset( new ethernet::TcpServer() );
        }

        // пришли данные
        connect(mEthernet.data(), SIGNAL(signalReadData(int)), SLOT(slotReadData(int)));
        // результат соединения
        connect(mEthernet.data(), SIGNAL(signalConnectStatus(bool)),
                mWidgetSettings, SLOT(slotSettingsIsAccept(bool)));
        connect(mEthernet.data(), SIGNAL(signalErrorMsg(QString)),
                mWidgetLog, SLOT(slotWriteInfo(QString)));
    }

    // настройки соединения
    mEthernet.data()->setSettings(QHostAddress(ip), portManage, portReceive);
}
