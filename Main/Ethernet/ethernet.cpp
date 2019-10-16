#include "ethernet.h"
namespace ethernet {

Ethernet::Ethernet(QObject* parent) : QObject(parent) {
}

Ethernet::~Ethernet() {
}

TypeDerivedClass Ethernet::getTypeClass() const {
    return mTypeClass;
}

void Ethernet::setTypeClass(const TypeDerivedClass& type) {
    mTypeClass = type;
}

}
