#include "../include/qssselector.h"

qss::Selector::Selector(const QString & str)
{
    parse(str);
}

qss::Selector& qss::Selector::operator=(const Selector &selector)
{
    m_fragments = selector.m_fragments;
    return *this;
}

qss::Selector& qss::Selector::addChild(const SelectorElement &fragment)
{
    return append(fragment, SelectorElement::CHILD);
}

qss::Selector& qss::Selector::addChild(const QString &fragment)
{
    return append(fragment, SelectorElement::CHILD);
}

qss::Selector& qss::Selector::addDescendant(const SelectorElement &fragment)
{
    return append(fragment, SelectorElement::DESCENDANT);
}

qss::Selector& qss::Selector::addDescendant(const QString &fragment)
{
    return append(fragment, SelectorElement::DESCENDANT);
}

qss::Selector& qss::Selector::addGeneralSibling(const SelectorElement &fragment)
{
    return append(fragment, SelectorElement::GENERAL_SIBLING);
}

qss::Selector& qss::Selector::addGeneralSibling(const QString &fragment)
{
    return append(fragment, SelectorElement::GENERAL_SIBLING);
}

qss::Selector& qss::Selector::addSibling(const SelectorElement &fragment)
{
    return append(fragment, SelectorElement::SIBLING);
}

qss::Selector& qss::Selector::addSibling(const QString &fragment)
{
    return append(fragment, SelectorElement::SIBLING);
}

qss::Selector& qss::Selector::append(const QString &fragment, SelectorElement::PositionType type)
{
    m_fragments.emplace_back(fragment);
    m_fragments.back().m_position = type;
    return *this;
}

qss::Selector& qss::Selector::append(const SelectorElement &fragment, SelectorElement::PositionType type)
{
    m_fragments.push_back(fragment);
    m_fragments.back().m_position = type;
    return *this;
}

void qss::Selector::parse(const QString &selector)
{
    auto addFragment = [this](const QStringList& list, int index, SelectorElement& fragment, SelectorElement::PositionType pos)
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
        SelectorElement select;
        addFragment(parts, 0, select, SelectorElement::PARENT);

        for (int i = 1; i < parts.size();)
        {
            SelectorElement select;

            if (Combinators.count(parts[i]) != 0)
            {
                if (parts.size() > (i + 1))
                {
                    addFragment(parts, i + 1, select, Combinators.at(parts[i]));
                    i += 2;
                }
                else
                {
                    throw Exception{ Exception::SELECTOR_INVALID, parts[i] };
                }
            }
            else if (Delimiters.at(QSS_BLOCK_START_DELIMITER) == parts[i].trimmed())
            {
                i++;
                continue;
            }
            else
            {
                addFragment(parts, i, select, SelectorElement::DESCENDANT);
                i++;
            }
        }
    }
}

QString qss::Selector::toString() const
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

bool qss::operator==(const Selector &lhs, const Selector &rhs)
{
    return lhs.toString() == rhs.toString();
}

void qss::Selector::preProcess(QString &str)
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

const std::unordered_map<QString, qss::SelectorElement::PositionType, qss::QStringHasher> qss::Selector::Combinators {
    { ">", SelectorElement::CHILD }, { "~", SelectorElement::SIBLING },
    { "+", SelectorElement::GENERAL_SIBLING }, { " ", SelectorElement::DESCENDANT },
    { ",", SelectorElement::ADJACENT }
};
