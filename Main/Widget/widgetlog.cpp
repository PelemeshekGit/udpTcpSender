#include "widgetlog.h"
#include "ui_widgetlog.h"
#include <QJsonDocument>
#include <QJsonObject>

WidgetLog::WidgetLog(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::WidgetLog) {
    ui->setupUi(this);

    connect(ui->btnLogClear, SIGNAL(clicked(bool)), SLOT(slotClearLog()));
}
//------------------------------------------------------------------------------
WidgetLog::~WidgetLog() {
    delete ui;
}
//------------------------------------------------------------------------------
void WidgetLog::writeLog(QByteArray& dg) {
    // запрет записи в лог
    if (!ui->checkEnableLog->isChecked()) {
        return;
    }

    if (ui->rbLogSay16->isChecked()) {  // записываем слова по 16 бит
        quint16* datagrammSay = reinterpret_cast<quint16*>(dg.data());
        int countSays = dg.size() / 2;
        QString resultStr;

        for (int i = 0; i < countSays; ++i) {
            resultStr += QString("[%1] = 0x%2 \n")
                         .arg(i)
                         .arg(QString::number(datagrammSay[i], 16).rightJustified(4, '0'));
        }

        ui->textLog->insertPlainText( resultStr );
    } else {    // записываем json
        ui->textLog->insertPlainText( dg.data() );
    }

    writeSeparatorToLog();
}
//------------------------------------------------------------------------------
void WidgetLog::writeInfo(const QString& text) {
    ui->textLog->insertPlainText( text + "\n" );
}
//------------------------------------------------------------------------------
void WidgetLog::slotWriteInfo(QString text) {
    writeInfo( text );
}
//------------------------------------------------------------------------------
void WidgetLog::slotClearLog() {
    ui->textLog->clear();
}
//------------------------------------------------------------------------------
void WidgetLog::writeSeparatorToLog() {
    ui->textLog->insertPlainText("---------\n");
}
//------------------------------------------------------------------------------

