#include "widgetlog.h"
#include "ui_widgetlog.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QTime>

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
void WidgetLog::writeLog(const QByteArray& dg) {
    // запрет записи в лог
    if (!ui->checkEnableLog->isChecked()) {
        return;
    }

    writeCurrentTimeToLog();
    writeSeparatorToLog();

    if (ui->rbLogSay16->isChecked()) {  // записываем слова по 16 бит
        int countSays = dg.size() / 2;
        QString resultStr;

        for (int ind = 0, numSay = 0; numSay < countSays; ind += 2, numSay++) {
            resultStr += QString("[%1] = 0x%2%3\n")
                         .arg(numSay)
                         .arg(QString::number(static_cast<quint8>(dg[ind + 1]), 16).rightJustified(2, '0'))
                         .arg(QString::number(static_cast<quint8>(dg[ind]), 16).rightJustified(2, '0'));
        }

        ui->textLog->insertPlainText( std::move(resultStr) );
    } else if (ui->rbLogJson->isChecked()) {   // записываем json/текст
        ui->textLog->insertPlainText( dg.data() + QString("\n") );
    }

    writeSeparatorToLog();
    moveToEnd();
}
//------------------------------------------------------------------------------
void WidgetLog::writeInfo(const QString& text) {
    // запрет записи в лог
    if (!ui->checkEnableLog->isChecked()) {
        return;
    }

    writeCurrentTimeToLog();
    ui->textLog->insertPlainText( text + "\n" );
    moveToEnd();
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
    ui->textLog->insertPlainText("\n");
}
//------------------------------------------------------------------------------
void WidgetLog::writeCurrentTimeToLog() {
    ui->textLog->insertPlainText( QString("<%1> ")
                                  .arg(QTime::currentTime().toString("HH:mm:ss:zzz")) );
}
//------------------------------------------------------------------------------
void WidgetLog::moveToEnd(){
    ui->textLog->moveCursor(QTextCursor::End);
}
//------------------------------------------------------------------------------
