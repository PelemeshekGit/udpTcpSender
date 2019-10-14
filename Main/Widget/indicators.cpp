#include "indicators.h"

namespace Widget {

Indicators::Indicators() {
    mPicOk.save("://icons/ok.xpm");
    mPicError.save("://icons/error.xpm");
    mPicCurrent.save("://icons/current.xpm");
    mPicNone.save("://icons/none.xpm");
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
