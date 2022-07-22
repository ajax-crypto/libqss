#ifndef QSSUTILS_H
#define QSSUTILS_H

#include <QString>
#include <QStringList>

#include <unordered_map>
#include <vector>
#include <deque>
#include <iostream>
#include <string>
#include <type_traits>

#ifdef _WIN32
#ifdef qss_EXPORTS
#define QSS_API __declspec(dllexport)
#else
#ifndef QSS_STATIC
#define QSS_API __declspec(dllimport)
#else
#define QSS_API
#endif
#endif
#else // _WIN32
#define QSS_API
#endif // _WIN32


#define LOG(X)  { std::cout << qss::tab(2) << X << std::endl; }
#define START() { std::cout << qss::tab(1) << "In [" __FUNCTION__ << "]" << std::endl; }
#define END() { qss::tab(0); std::cout << qss::tab(2) << "Out [" __FUNCTION__ << "]" << std::endl; }

namespace qss
{
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
    using PropertyMap = std::unordered_map<QString, InvalidablePair<QString>, QStringHasher>;

    enum Delimiter
    {
        QSS_ID_DELIMITER,
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

    const std::unordered_map<Delimiter, QString> Delimiters{
        { QSS_ID_DELIMITER, "#" },
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
        return QString{ "\"%1\"" }.arg(input);
    }

    QSS_API std::ostream& operator<<(std::ostream& stream, const QString& str);
    
    QSS_API std::ostream& operator<<(std::ostream& stream, const QStringList& list);
}

#endif
