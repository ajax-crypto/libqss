#ifndef QSSEXCEPTION_H
#define QSSEXCEPTION_H

#include "qssbase.h"

class QSSException
{
public:

    enum Codes
    {
        HEADER_PARAM_INVALID,
        BLOCK_PARAM_INVALID,
        SELECTOR_INVALID,
        BLOCK_BRACKETS_INVALID,
        MULTIPLE_NAMES,
        ILL_FORMED_HEADER_PARAM
    };

    QSSException(int code, const QString& details = "")
        : m_code{code}, m_details{details} {}

    QString what() const;

private:

    static const std::unordered_map<int, QString> Messages;

    int     m_code;
    QString m_details;
};


#endif // QSSEXCEPTION_H
