#include "context.h"

#include <QDebug>

QDebug operator<<(QDebug debug, const Context &context)
{
    debug.nospace() << "Context(";
    Context::const_iterator it = context.begin();
    Context::const_iterator end = context.end();
    if (it != end) {
        debug << *it;
        ++it;
    }
    while (it != end) {
        debug << ", " << *it;
        ++it;
    }
    debug << ')';

    return debug;
}
