#ifndef QSSEXCEPTION_H
#define QSSEXCEPTION_H

#include "qssparseable.h"

namespace qss
{
    class QSS_API Exception
    {
    public:

        enum Codes
        {
            HEADER_PARAM_INVALID,
            BLOCK_PARAM_INVALID,
            SELECTOR_INVALID,
            BLOCK_BRACKETS_INVALID,
            MULTIPLE_IDS,
            ILL_FORMED_HEADER_PARAM
        };

        Exception(int code, const QString& details = "")
            : m_code{ code }, m_details{ details } {}

        QString what() const;

    private:

        static const std::unordered_map<int, QString> Messages;

        int     m_code;
        QString m_details;
    };
}


#endif // QSSEXCEPTION_H
