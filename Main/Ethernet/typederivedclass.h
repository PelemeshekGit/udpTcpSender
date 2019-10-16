#ifndef TYPEDERIVEDCLASS_H
#define TYPEDERIVEDCLASS_H

namespace ethernet {

/**
 * @brief Типы поризводных классов, используются для понимания какой это класс
 * P.S. выглядит как костыль, но пока так. По хорошему надо как-то сравнивать классы
 */
enum class TypeDerivedClass {
    Udp,
    TcpClient,
    TcpServer,
    Undefined
};

}

#endif // TYPEDERIVEDCLASS_H
