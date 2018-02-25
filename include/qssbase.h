#ifndef QSSBASE_H
#define QSSBASE_H

#include <QString>
#include <QStringList>

#include <unordered_map>
#include <vector>
#include <deque>
#include <iostream>
#include <string>
#include <type_traits>

inline std::string tab(int in)
{
    static std::string _tabs;
    auto local = _tabs;
    switch (in)
    {
    case 1: _tabs.append("\t"); break;
    case 2: break;
    case 0: _tabs.erase(0, 1); break;
    }
    return local;
}

#define LOG(X)  { std::cout << tab(2) << X << std::endl; }
#define START() { std::cout << tab(1) << "In [" __FUNCTION__ << "]" << std::endl; }
#define END() { tab(0); std::cout << tab(2) << "Out [" __FUNCTION__ << "]" << std::endl; }

struct QStringHasher
{
    std::size_t operator()(const QString& str) const
    {
        return std::hash<std::string>()(str.toStdString());
    }
};

template <typename T> using InvalidablePair = std::pair<T, bool>;
using QStringPair = std::pair<QString, QString>;
using QStringPairs = std::vector<QStringPair>;
using QStringMap = std::unordered_map<QString, QString, QStringHasher>;
using QssPropertyMap = std::unordered_map<QString, InvalidablePair<QString>, QStringHasher>;

enum QSSDelimiter
{
    QSS_NAME_DELIMITER,
    QSS_CLASS_DELIMITER,
    QSS_ADJACENT_SIBLING_DELIMITER,
    QSS_GENERAL_SIBLING_DELIMITER,
    QSS_DESCENDANT_DELIMITER,
    QSS_CHILD_DELIMITER,
    QSS_PSEUDO_CLASS_DELIMITER,
    QSS_STATEMENT_END_DELIMITER,
    QSS_PARAM_DELIMITER,
    QSS_BLOCK_START_DELIMITER,
    QSS_BLOCK_END_DELIMITER,
    QSS_SELECT_PARAM_START_DELIMITER,
    QSS_SELECT_PARAM_END_DELIMITER,
    QSS_SUB_CONTROL_DELIMITER
};

const std::unordered_map<QSSDelimiter, QString> QSSDelimiters {
    { QSS_NAME_DELIMITER, "#" },
    { QSS_CLASS_DELIMITER, "." },
    { QSS_ADJACENT_SIBLING_DELIMITER, "+" },
    { QSS_GENERAL_SIBLING_DELIMITER, "~" },
    { QSS_DESCENDANT_DELIMITER, " " },
    { QSS_CHILD_DELIMITER, ">" },
    { QSS_PSEUDO_CLASS_DELIMITER, ":" },
    { QSS_STATEMENT_END_DELIMITER, ";" },
    { QSS_PARAM_DELIMITER, "=" },
    { QSS_BLOCK_START_DELIMITER, "{" },
    { QSS_BLOCK_END_DELIMITER, "}" },
    { QSS_SELECT_PARAM_START_DELIMITER, "[" },
    { QSS_SELECT_PARAM_END_DELIMITER, "]" },
    { QSS_SUB_CONTROL_DELIMITER, "::" }
};

inline QString QuotedString(const QString& input)
{
    return QString{"\"%1\""}.arg(input);
}

inline std::ostream& operator<<(std::ostream& stream, const QString& str)
{
    stream << str.toStdString();
    return stream;
}

inline std::ostream& operator <<(std::ostream& stream, const QStringList& list)
{
    stream << "[ ";

    for (const auto& str : list)
    {
        stream << "(" << str << "), ";
    }

    stream << "]";
    return stream;
}

class QSSBase
{
public:

    QSSBase() {}
    virtual ~QSSBase() {}

    virtual QString toString() const = 0;
    virtual void    parse(const QString& str) = 0;
};

#endif // QSSBASE_H
