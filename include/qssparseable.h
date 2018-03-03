#ifndef QSSPARSEABLE_H
#define QSSPARSEABLE_H

#include "qssutils.h"

namespace qss
{
    class IParseable
    {
    public:

        IParseable() {}
        virtual ~IParseable() {}

        virtual QString toString() const = 0;
        virtual void    parse(const QString& str) = 0;
    };
}

#endif
