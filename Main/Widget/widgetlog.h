#ifndef WIDGETLOG_H
#define WIDGETLOG_H

#include <QWidget>
#include <QByteArray>

namespace Ui {
class WidgetLog;
}

/**
 * @brief Класс графического вывода отправленных/принятых команд
 */
class WidgetLog : public QWidget {
    Q_OBJECT

public:
    explicit WidgetLog(QWidget* parent = nullptr);
    ~WidgetLog();

    /// Добавить датаграмму в лог
    void writeLog(const QByteArray& dg);

    /// Добавить описание в лог
    void writeInfo(const QString& text);

public slots:
    void slotWriteInfo(QString text);

private:
    Ui::WidgetLog* ui;

    /// Разделитель для лога, чтобы отделять разные датаграммы
    void writeSeparatorToLog();

    /// Запись текущего времени в лог
    void writeCurrentTimeToLog();

private slots:
    void slotClearLog();
};

#endif // WIDGETLOG_H
