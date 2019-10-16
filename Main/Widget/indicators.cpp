#include "indicators.h"

namespace Widget {

Indicators::Indicators() {
    mPicOk.load("://icons/ok.xpm");
    mPicError.load("://icons/error.xpm");
    mPicCurrent.load("://icons/current.xpm");
    mPicNone.load("://icons/none.xpm");
}
//------------------------------------------------------------------------------
Indicators::~Indicators() {
}
//------------------------------------------------------------------------------
const QPixmap* Indicators::getOk() const {
    return &mPicOk;
}
//------------------------------------------------------------------------------
const QPixmap* Indicators::getError() const {
    return &mPicError;
}
//------------------------------------------------------------------------------
const QPixmap* Indicators::getCurrent() const {
    return &mPicCurrent;
}
//------------------------------------------------------------------------------
const QPixmap* Indicators::getNone() const {
    return &mPicNone;
}
//------------------------------------------------------------------------------

}
