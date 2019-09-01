#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <QRegExp>

static const int GEOMETRY_AX_LABEL = 2;
static const int GEOMETRY_AY_LABEL = 20;
static const int GEOMETRY_WIDTH_LABEL = 50;
static const int GEOMETRY_HEIGHT_LABEL = 18;
static const int GEOMETRY_AX_LINEEDIT = 70;
static const int GEOMETRY_AY_LINEEDIT = 20;
static const int GEOMETRY_WIDTH_LINEEDIT = 40;
static const int GEOMETRY_HEIGHT_LINEEDIT = 18;
static const int MAXLENGTH_SAY16 = 4;
static const int BASEDATAHEX = 16;
static const int GEOMETRY_WIDTH_CONTENTS = 120;
static const int GEOMETRY_AY_CONTENTS = 20;
static const int GEOMETRY_HEIGHT_LINE = 20;
static const int GEOMETRY_WIDTH_LISTSAY16 = 450;

MainWidget::MainWidget(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::MainWidget) {
    ui->setupUi(this);

    ui->tabWidget->setCurrentIndex(0);

    mWidgetListSay16 = new QRect(0, 0,
                                 GEOMETRY_WIDTH_LISTSAY16,
                                 COUNTSAYS16 * GEOMETRY_HEIGHT_LINE);
    ui->widget->setGeometry(*mWidgetListSay16);

    slotChangeSendData();

    ui->edDefaultValue->setValidator(new QRegExpValidator(QRegExp("[a-f,0-9,A-F]{0,4}"), 0));
    // настройки для ввода IP адреса
    ui->edIp->setValidator(new QRegExpValidator(QRegExp("\\d{1,3}[.]\\d{1,3}[.]\\d{1,3}[.]\\d{1,3}"),
                           0));

    for (int i = 0; i < COUNTSAYS16; ++i) {
        mEditsData[i] = nullptr;
        mLabelsName[i] = nullptr;
    }

    slotUpdateQuantitySays();
    slotUpdateSettings();

    connect(ui->rbSendSay16, SIGNAL(toggled(bool)), SLOT(slotChangeSendData()));
    connect(ui->rbSendJson, SIGNAL(toggled(bool)), SLOT(slotChangeSendData()));
    connect(ui->edQuantitySays, SIGNAL(editingFinished()), SLOT(slotUpdateQuantitySays()));
    connect(ui->edDefaultValue, SIGNAL(editingFinished()), SLOT(slotCheckValueEdit()));

    connect(ui->rbSettingsUdp, SIGNAL(clicked(bool)), SLOT(slotChangeSetting()));
    connect(ui->rbSettingsTcp, SIGNAL(clicked(bool)), SLOT(slotChangeSetting()));
    connect(ui->rbSettingsTcpClient, SIGNAL(clicked(bool)), SLOT(slotChangeSetting()));
    connect(ui->rbSettingsTcpServer, SIGNAL(clicked(bool)), SLOT(slotChangeSetting()));

    connect(ui->edIp, SIGNAL(textChanged(QString)), SLOT(slotUpdateSettings()));
    connect(ui->edPortTcp, SIGNAL(valueChanged(int)), SLOT(slotUpdateSettings()));
    connect(ui->edPortSend, SIGNAL(valueChanged(int)), SLOT(slotUpdateSettings()));
    connect(ui->edPortReceive, SIGNAL(valueChanged(int)), SLOT(slotUpdateSettings()));

    connect(ui->btnLogClear, SIGNAL(clicked(bool)), SLOT(slotClearLog()));
}
//------------------------------------------------------------------------------
MainWidget::~MainWidget() {
    for ( int row = 0; row < mTablQuantitySays; ++row ) {
        deleteSay(row);
    }

    // ну, спорный момент, ибо мы передали объект в QWidget, который сам объект удалит
    delete mWidgetListSay16;

    delete ui;
}
//------------------------------------------------------------------------------
void MainWidget::slotChangeSendData() {
    ui->widgetSendJson->setVisible(ui->rbSendJson->isChecked());
    ui->widgetSendSay16->setVisible(ui->rbSendSay16->isChecked());
}
//------------------------------------------------------------------------------
QLabel* MainWidget::createNewLadel(int row) {
    QLabel* lb = new QLabel(ui->widget);
    lb->setText(QString::fromUtf8("сл.") + QString::number(row));
    lb->setGeometry(GEOMETRY_AX_LABEL, GEOMETRY_AY_LABEL * row,
                    GEOMETRY_WIDTH_LABEL, GEOMETRY_HEIGHT_LABEL);
    lb->setVisible(true);
    return lb;
}
//------------------------------------------------------------------------------
QLineEdit* MainWidget::createNewEdit( int row) {
    QLineEdit* ed = new QLineEdit(ui->widget);
    ed->setMaxLength( MAXLENGTH_SAY16 );
    ed->setText( ui->edDefaultValue->text() );
    ed->setGeometry(GEOMETRY_AX_LINEEDIT, GEOMETRY_AY_LINEEDIT * row,
                    GEOMETRY_WIDTH_LINEEDIT, GEOMETRY_HEIGHT_LINEEDIT);
    ed->setVisible(true);

    QRegExp regExp("[a-f,0-9,A-F]{0,4}");
    QRegExpValidator* regValidator = new QRegExpValidator(regExp, 0);
    ed->setValidator(regValidator);
    connect(ed, SIGNAL(editingFinished()), SLOT(slotCheckValueEdit()));
    return ed;
}
//------------------------------------------------------------------------------
void MainWidget::slotCheckValueEdit() {
    QLineEdit* bufEd = (QLineEdit*)sender();
    bool isCorrectData;
    bufEd->text().toUShort(&isCorrectData, BASEDATAHEX);

    if (!isCorrectData) {
        bufEd->setText("0000");
        return;
    }

    bufEd->setText( bufEd->text().rightJustified(MAXLENGTH_SAY16, '0') );
}
//------------------------------------------------------------------------------
void MainWidget::setResizeContents(int size) {
    ui->scrollAreaWidgetContents->setFixedSize(GEOMETRY_WIDTH_CONTENTS,
            GEOMETRY_AY_CONTENTS * size);
}
//------------------------------------------------------------------------------
void MainWidget::createSay(int row) {
    if (!mLabelsName[row]) {
        mLabelsName[row] = createNewLadel(row);
    }

    if (!mEditsData[row]) {
        mEditsData[row] = createNewEdit(row);
    }
}
//------------------------------------------------------------------------------
void MainWidget::deleteSay(int row) {
    if (mEditsData[row]) {
        delete mEditsData[row];
        mEditsData[row] = nullptr;
    }

    if (mLabelsName[row]) {
        delete mLabelsName[row];
        mLabelsName[row] = nullptr;
    }
}
//------------------------------------------------------------------------------
void MainWidget::slotUpdateQuantitySays() {
    int newSize = ui->edQuantitySays->value();

    if (newSize == mTablQuantitySays) {
        return;
    }

    if ( (newSize < 1) || (newSize >= COUNTSAYS16) ) {
        return;
    }

    // если новый размер меньше старого
    if ( newSize < mTablQuantitySays ) {
        // удаляем лишние строчки
        for ( int row = newSize; row < mTablQuantitySays; ++row ) {
            deleteSay(row);
        }
    } else {
        // иначе добавляем недостающие строки
        for ( int row = mTablQuantitySays; row < newSize; ++row ) {
            createSay(row);
        }
    }

    mTablQuantitySays = newSize; // запомнить новый размер таблицы
    setResizeContents(mTablQuantitySays);
}
//------------------------------------------------------------------------------
void MainWidget::slotChangeSetting() {
    ui->widgetTcpManager->setVisible(ui->rbSettingsTcp->isChecked());
    ui->framPortTcp->setVisible(ui->rbSettingsTcp->isChecked());
    ui->framePortUdpSend->setVisible(ui->rbSettingsUdp->isChecked());
    ui->framePortUdpReceive->setVisible(ui->rbSettingsUdp->isChecked());

    slotUpdateSettings();
}
//------------------------------------------------------------------------------
void MainWidget::slotUpdateSettings() {
    // TODO
}
//------------------------------------------------------------------------------
void MainWidget::slotClearLog() {
    ui->textLog->clear();
}
//------------------------------------------------------------------------------
