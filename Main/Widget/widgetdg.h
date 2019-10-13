#ifndef WIDGETDG_H
#define WIDGETDG_H

#include <QWidget>
#include <QRect>
#include <QLineEdit>
#include <QLabel>
#include <QList>
#include <QByteArray>

#include "consts.h"
using namespace Widget;

namespace Ui {
class WidgetDg;
}

/**
 * @brief Класс графического формирования датаграмм
 */
class WidgetDg : public QWidget {
    Q_OBJECT

public:
    explicit WidgetDg(QWidget* parent = nullptr);
    ~WidgetDg();

    /// Получить сформированную датаграмму
    QByteArray getDg() const;

public slots:
    /// Вывести статус отправляемой датаграммы
    void slotSetStatusSendingDg(STATUS_INDICATOR status);

signals:
    /// Датаграмма сформирована, отправить её методом getDg()
    void sendData();

    /// Отправка сообщение о состоянии
    void msg(QString);

private:
    Ui::WidgetDg* ui;

    /// перевод из 16-битного слова в qbytearray
    static void splitWordOnBytes(ushort word, QByteArray& byteArray);

    /** @brief Изменить количество строк в таблице кодограммы
        @param newsize - колличество строк
    */
    void resizeTable(const int newsize);

    /** @brief Добавить Лейбл, в котором отображается код в HEX и который можно менять
        @param row - Номер слова, которое добавляется на панель
    */
    QLabel* createNewLadel(int row);

    /** @brief Добавить текстовое окно, в котором отображается код в HEX
        и который можно менять
        @param row - Номер строки, которая добавляется на панель
    */
    QLineEdit* createNewEdit(int row);

    /** @brief Создать панель с заданным числом слов команды
        @param rowa - число слов
    */
    void createSay(int index);

    /** @brief Удалить выбранные слова команды
        @param row - удаляемое слово
    */
    void deleteSay(int index);

    /** @brief Установить размер таблицы
        @param size - Значение размера таблицы
    */
    void setResizeContents(int size);

    /// Установить значение индикатора отправки данных
    void setInicatorStatus(STATUS_INDICATOR status);

    //------data------//
    /// Окно для списка команд
    QRect* mWidgetListSay16;
    /** @brief Текущий размер таблицы слов.
        Нужен при изменеии размера окна, в котором находится таблица
    */
    int mTablQuantitySays;

    // массивы указателей на элементы таблицы кодограммы
    /// Массив, хранящий код команды в HEX
    QList<QLineEdit*>  mEditsData;
    /// Массивы, хранящие имя номера слова
    QList<QLabel*>     mLabelsName;

    /// Сформированная датаграмма
    QByteArray mRealDg;

private slots:
    /// Изменение типа отправляемых данных (слова/json)
    void slotChangeSendData();

    /// Проверка, что введенные значения пользователем корректные
    void slotCheckValueEdit();

    /// Изменено число слов для датаграммы 16 бит
    void slotUpdateQuantitySays();

    /// Нажание на кнопку отправки данных
    void slotSendData();
};

#endif // WIDGETDG_H
