#include "../include/qssselector.h"

QSSSelector::QSSSelector(const QString & str)
{
    parse(str);
}

QSSSelector &QSSSelector::operator=(const QSSSelector &selector)
{
    m_fragments = selector.m_fragments;
    return *this;
}

QSSSelector &QSSSelector::child(const QSSSelectorFragment &fragment)
{
    return append(fragment, QSSSelectorFragment::CHILD);
}

QSSSelector &QSSSelector::child(const QString &fragment)
{
    return append(fragment, QSSSelectorFragment::CHILD);
}

QSSSelector &QSSSelector::descendant(const QSSSelectorFragment &fragment)
{
    return append(fragment, QSSSelectorFragment::DESCENDANT);
}

QSSSelector &QSSSelector::descendant(const QString &fragment)
{
    return append(fragment, QSSSelectorFragment::DESCENDANT);
}

QSSSelector &QSSSelector::generalSibling(const QSSSelectorFragment &fragment)
{
    return append(fragment, QSSSelectorFragment::GENERAL_SIBLING);
}

QSSSelector &QSSSelector::generalSibling(const QString &fragment)
{
    return append(fragment, QSSSelectorFragment::GENERAL_SIBLING);
}

QSSSelector &QSSSelector::sibling(const QSSSelectorFragment &fragment)
{
    return append(fragment, QSSSelectorFragment::SIBLING);
}

QSSSelector &QSSSelector::sibling(const QString &fragment)
{
    return append(fragment, QSSSelectorFragment::SIBLING);
}

QSSSelector &QSSSelector::append(const QString &fragment, QSSSelectorFragment::PositionType type)
{
    m_fragments.emplace_back(fragment);
    m_fragments.back().m_position = type;
    return *this;
}

QSSSelector &QSSSelector::append(const QSSSelectorFragment &fragment, QSSSelectorFragment::PositionType type)
{
    m_fragments.push_back(fragment);
    m_fragments.back().m_position = type;
    return *this;
}

void QSSSelector::parse(const QString &selector)
{
    auto addFragment = [this](const QStringList& list, int index, QSSSelectorFragment& fragment, QSSSelectorFragment::PositionType pos)
    {
        fragment.parse(list[index]);
        fragment.m_position = pos;
        m_fragments.push_back(fragment);
    };

    auto str = selector.trimmed();

    if (str.size() > 0)
    {
        preProcess(str);

        QRegExp regex("`+");
        auto parts = str.split(regex, QString::SkipEmptyParts);
        QSSSelectorFragment select;
        addFragment(parts, 0, select, QSSSelectorFragment::PARENT);

        for (int i = 1; i < parts.size();)
        {
            QSSSelectorFragment select;

            if (Combinators.count(parts[i]) != 0)
            {
                if (parts.size() > (i + 1))
                {
                    addFragment(parts, i + 1, select, Combinators.at(parts[i]));
                    i += 2;
                }
                else
                {
                    throw QSSException{ QSSException::SELECTOR_INVALID, parts[i] };
                }
            }
            else if (QSSDelimiters.at(QSS_BLOCK_START_DELIMITER) == parts[i].trimmed())
            {
                i++;
                continue;
            }
            else
            {
                addFragment(parts, i, select, QSSSelectorFragment::DESCENDANT);
                i++;
            }
        }
    }
}

QString QSSSelector::toString() const
{
    QString result;

    if (m_fragments.size() > 0)
    {
        result = m_fragments[0].toString();

        for (int i = 1; i < m_fragments.size(); ++i)
        {
            result += " " + m_fragments[i].toString();
        }
    }

    return result;
}

bool operator==(const QSSSelector &lhs, const QSSSelector &rhs)
{
    return lhs.toString() == rhs.toString();
}

void QSSSelector::preProcess(QString &str)
{
    int quotes = str[0] == '"';

    for (int i = 1; i < str.size(); ++i)
    {
        if (str[i].isSpace() && (quotes % 2 == 0))
        {
            str[i] = PreProcessChar;
        }

        quotes += (str[i] == '"' && str[i - 1] != '\\');
    }
}

const std::unordered_map<QString, QSSSelectorFragment::PositionType, QStringHasher> QSSSelector::Combinators {
    { ">", QSSSelectorFragment::CHILD }, { "~", QSSSelectorFragment::SIBLING },
    { "+", QSSSelectorFragment::GENERAL_SIBLING }, { " ", QSSSelectorFragment::DESCENDANT },
    { ",", QSSSelectorFragment::ADJACENT }
};
