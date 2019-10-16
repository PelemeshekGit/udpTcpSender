#include "widgetdg.h"
#include "ui_widgetdg.h"
#include <QRegExp>
#include <QJsonDocument>
#include "indicators.h"

Q_DECLARE_METATYPE( STATUS_INDICATOR )

WidgetDg::WidgetDg(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::WidgetDg),
    mTablQuantitySays(0) {
    ui->setupUi(this);

    qRegisterMetaType<STATUS_INDICATOR>("STATUS_INDICATOR");

    mWidgetListSay16 = new QRect(0, 0,
                                 Consts::GEOMETRY_WIDTH_LISTSAY16,
                                 Consts::COUNTSAYS16 * Consts::GEOMETRY_HEIGHT_LINE);
    ui->widget->setGeometry(*mWidgetListSay16);

    ui->edDefaultValue->setValidator(new QRegExpValidator(QRegExp("[a-f,0-9,A-F]{0,4}"), 0));

    slotChangeSendData();

    slotUpdateQuantitySays();

    connect(ui->rbSendSay16, SIGNAL(toggled(bool)), SLOT(slotChangeSendData()));
    connect(ui->rbSendJson, SIGNAL(toggled(bool)), SLOT(slotChangeSendData()));
    connect(ui->rbSendString, SIGNAL(toggled(bool)), SLOT(slotChangeSendData()));
    connect(ui->edQuantitySays, SIGNAL(editingFinished()), SLOT(slotUpdateQuantitySays()));
    connect(ui->edDefaultValue, SIGNAL(editingFinished()), SLOT(slotCheckValueEdit()));
    connect(ui->btnSend, SIGNAL(clicked(bool)), SLOT(slotSendData()));
}
//------------------------------------------------------------------------------
WidgetDg::~WidgetDg() {
    for ( int row = 0; row < mTablQuantitySays; ++row ) {
        deleteSay(row);
    }

    // ну, спорный момент, ибо мы передали объект в QWidget, который сам объект удалит
    delete mWidgetListSay16;

    delete ui;
}
//------------------------------------------------------------------------------
void WidgetDg::slotChangeSendData() {
    ui->widgetSendJson->setVisible(ui->rbSendJson->isChecked());
    ui->widgetSendSay16->setVisible(ui->rbSendSay16->isChecked());
    ui->widgetSendString->setVisible(ui->rbSendString->isChecked());
}
//------------------------------------------------------------------------------
QLabel* WidgetDg::createNewLadel(int row) {
    QLabel* lb = new QLabel(ui->widget);
    lb->setText(QString::fromUtf8("сл.") + QString::number(row));
    lb->setGeometry(Consts::GEOMETRY_AX_LABEL, Consts::GEOMETRY_AY_LABEL * row,
                    Consts::GEOMETRY_WIDTH_LABEL, Consts::GEOMETRY_HEIGHT_LABEL);
    lb->setVisible(true);
    return lb;
}
//-----------------------------------------------------------------------------
QLineEdit* WidgetDg::createNewEdit( int row) {
    QLineEdit* ed = new QLineEdit(ui->widget);
    ed->setMaxLength( Consts::MAXLENGTH_SAY16 );
    ed->setText( ui->edDefaultValue->text() );
    ed->setGeometry(Consts::GEOMETRY_AX_LINEEDIT, Consts::GEOMETRY_AY_LINEEDIT * row,
                    Consts::GEOMETRY_WIDTH_LINEEDIT, Consts::GEOMETRY_HEIGHT_LINEEDIT);
    ed->setVisible(true);

    QRegExp regExp("[a-f,0-9,A-F]{0,4}");
    QRegExpValidator* regValidator = new QRegExpValidator(regExp, 0);
    ed->setValidator(regValidator);
    connect(ed, SIGNAL(editingFinished()), SLOT(slotCheckValueEdit()));
    return ed;
}
//------------------------------------------------------------------------------
void WidgetDg::slotCheckValueEdit() {
    QLineEdit* bufEd = (QLineEdit*)sender();
    bool isCorrectData;
    bufEd->text().toUShort(&isCorrectData, Consts::BASEDATAHEX);

    if (!isCorrectData) {
        bufEd->setText("0000");
        return;
    }

    bufEd->setText( bufEd->text().rightJustified(Consts::MAXLENGTH_SAY16, '0') );
}
//------------------------------------------------------------------------------
void WidgetDg::setResizeContents(int size) {
    ui->scrollAreaWidgetContents->setFixedSize(Consts::GEOMETRY_WIDTH_CONTENTS,
            Consts::GEOMETRY_AY_CONTENTS * size);
}
//------------------------------------------------------------------------------
void WidgetDg::createSay(int index) {
    mLabelsName.append( createNewLadel(index) );
    mEditsData.append( createNewEdit(index) );
}
//------------------------------------------------------------------------------
void WidgetDg::deleteSay(int index) {
    if (index >= mEditsData.size()) {
        return;
    }

    delete mEditsData[index];
    mEditsData.removeAt(index);

    delete mLabelsName[index];
    mLabelsName.removeAt(index);
}
//------------------------------------------------------------------------------
void WidgetDg::slotUpdateQuantitySays() {
    int newSize = ui->edQuantitySays->value();

    if (newSize == mTablQuantitySays) {
        return;
    }

    if ( (newSize < 1) || (newSize >= Consts::COUNTSAYS16) ) {
        return;
    }

    // если новый размер меньше старого
    if ( newSize < mTablQuantitySays ) {
        // удаляем лишние строчки
        for ( int row = (mTablQuantitySays - 1); row >= newSize; --row ) {
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
void WidgetDg::slotSendData() {
    mRealDg.clear();

    if (ui->rbSendSay16->isChecked()) { // слова 16 бит
        for (const QLineEdit* lineEdit : mEditsData) {
            WidgetDg::splitWordOnBytes( lineEdit->text().toUShort(0, 16), mRealDg );
        }
    } else if (ui->rbSendJson->isChecked()) { // json
        mRealDg.append( ui->textEditJson->toPlainText() );

        // проверка корректности
        if (!mRealDg.isEmpty()) {
            QJsonDocument jsonDoc = QJsonDocument::fromJson( mRealDg );

            if ( !jsonDoc.isObject() ) { // некорректный json код
                emit msg(QString("Uncorrect json-code"));
                setInicatorStatus(STATUS_INDICATOR::Error);
                return;
            }
        }
    } else if (ui->rbSendString->isChecked()) { // просто текст
        mRealDg.append( ui->textEditString->toPlainText() );
    }

    if (mRealDg.isEmpty()) { // данные не должны быть пустыми
        emit msg(QString("data is empty"));
        setInicatorStatus(STATUS_INDICATOR::Error);
        return;
    }

    setInicatorStatus(STATUS_INDICATOR::Current);
    emit sendData();
}
//------------------------------------------------------------------------------
void WidgetDg::splitWordOnBytes(ushort word, QByteArray& byteArray) {
    byteArray.append( static_cast<char>( word & 0xFF ) );
    byteArray.append( static_cast<char>( (word >> 8) & 0xFF ) );
}
//------------------------------------------------------------------------------
QByteArray WidgetDg::getDg() const {
    return mRealDg;
}
//------------------------------------------------------------------------------
void WidgetDg::setInicatorStatus(STATUS_INDICATOR status) {
    if (status == STATUS_INDICATOR::Ok) {
        ui->indicatorStatusSend->setPixmap( *INDICATOR.getOk() );
    } else if (status == STATUS_INDICATOR::Error) {
        ui->indicatorStatusSend->setPixmap( *INDICATOR.getError() );
    } else if (status == STATUS_INDICATOR::Current) {
        ui->indicatorStatusSend->setPixmap( *INDICATOR.getCurrent() );
    } else if (status == STATUS_INDICATOR::None) {
        ui->indicatorStatusSend->setPixmap( *INDICATOR.getNone());
    }
}
//------------------------------------------------------------------------------
