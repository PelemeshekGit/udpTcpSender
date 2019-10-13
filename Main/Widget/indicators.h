#ifndef INDICATORS_H
#define INDICATORS_H

#include <QPixmap>
#include <QSharedPointer>

namespace Widget {

#define INDICATOR Indicators::Instance()

class Indicators {
public:
    static Indicators& Instance() {
        static Indicators instance;
        return instance;
    }
    ~Indicators();

    // передача значений по ссылке
    const QPixmap* getOk() const;
    const QPixmap* getError() const;
    const QPixmap* getCurrent() const;
    const QPixmap* getNone() const;

private:
    Indicators();

    /// Индикатор для команд - Ответ получен
    QPixmap mPicOk;
    /// Индикатор для команд - Ответ не получен
    QPixmap mPicError;
    /// Индикатор для команд - Ожидается ответ на команду
    QPixmap mPicCurrent;
    /// Индикатор для команд - По умолчанию
    QPixmap mPicNone;

};

}
#endif // INDICATORS_H
