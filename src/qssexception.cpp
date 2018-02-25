#include "../include/qssexception.h"

const std::unordered_map<int, QString> QSSException::Messages {
    { QSSException::HEADER_PARAM_INVALID, "Header param is invalid" },
    { QSSException::BLOCK_PARAM_INVALID, "Block param is invalid" },
    { QSSException::SELECTOR_INVALID, "Selector is invalid" },
    { QSSException::BLOCK_BRACKETS_INVALID, "Block brackets invalid" },
    { QSSException::MULTIPLE_NAMES, "More than one name encountered" },
    { QSSException::ILL_FORMED_HEADER_PARAM, "Header param is incomplete" }
};

QString QSSException::what() const
{
    return QString{ Messages.at(m_code) + ": < %1 >" }.arg(m_details);
}
