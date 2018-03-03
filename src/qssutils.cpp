#include "../include/qssutils.h"

std::ostream & qss::operator<<(std::ostream & stream, const QString& str)
{
    stream << str.toStdString();
    return stream;
}

std::ostream & qss::operator<<(std::ostream & stream, const QStringList& list)
{
    stream << "[ ";

    for (const auto& str : list)
    {
        stream << "(" << str << "), ";
    }

    stream << "]";
    return stream;
}
