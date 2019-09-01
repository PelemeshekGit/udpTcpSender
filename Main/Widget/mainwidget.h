#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QRect>
#include <QLineEdit>
#include <QLabel>

namespace Ui {
class MainWidget;
}

/// Максимально число слов
static const int COUNTSAYS16 = 1024;

class MainWidget : public QWidget {
    Q_OBJECT

public:
    explicit MainWidget(QWidget* parent = 0);
    ~MainWidget();

private:
    Ui::MainWidget* ui;

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
    void createSay(int rowa);

    /** @brief Удалить выбранные слова команды
        @param row - удаляемое слово
    */
    void deleteSay(int row);

    /** @brief Установить размер таблицы
        @param size - Значение размера таблицы
    */
    void setResizeContents(int size);

    //------data------//
    /// Окно для списка команд
    QRect *mWidgetListSay16;
    /** @brief Текущий размер таблицы слов.
        Нужен при изменеии размера окна, в котором находится таблица
    */
    int mTablQuantitySays;

    // массивы указателей на элементы таблицы кодограммы
    /// Массив, хранящий код команды в HEX
    QLineEdit  *mEditsData[COUNTSAYS16];
    /// Массивы, хранящие имя номера слова
    QLabel     *mLabelsName[COUNTSAYS16];

private slots:
    void slotChangeSendData();

    /// Проверка, что введенные значения пользователем корректные
    void slotCheckValueEdit();

    void slotUpdateQuantitySays();

    void slotUpdateSettings();

    void slotChangeSetting();

    void slotClearLog();

};

#endif // MAINWIDGET_H
