#include "../include/qsstext.h"

QSSText::QSSText(const QString &qss)
{
    parse(qss);
}

QSSText& QSSText::addFragment(const QSSFragment& fragment, bool enabled)
{
    auto selectorExists = false;

    for (auto& frags : m_fragments)
    {
        if (frags.first.selector() == fragment.selector())
        {
            frags.first.add(fragment.block());
            selectorExists = true;
        }
    }

    if (!selectorExists)
    {
        m_fragments.push_back(std::make_pair(fragment, enabled));
    }

    return *this;
}

QSSText &QSSText::toggleFragment(int index)
{
    m_fragments[index].second = !m_fragments[index].second;
    return *this;
}

QSSText& QSSText::addFragment(const QString& fragment, bool enabled)
{
    return addFragment(QSSFragment{ fragment }, enabled);
}

QSSText &QSSText::enableFragment(int index, bool enable)
{
    m_fragments[index].second = enable;
    return *this;
}

QSSText &QSSText::removeFragment(const QString& fragment)
{
    std::remove_if(m_fragments.begin(), m_fragments.end(), [fragment](const QSSFragmentPair& existing){
        return QString::compare(existing.first.toString(), QSSFragment{ fragment }.toString()) == 0;
    });
    return *this;
}

QSSText &QSSText::removeFragment(int index)
{
    m_fragments.erase(m_fragments.begin() + index);
    return *this;
}

QSSText& QSSText::operator+=(const QString& fragment)
{
    addFragment(fragment, true);
    return *this;
}

QSSText & QSSText::operator+=(const QSSText & qss)
{
    for (const auto& fragment : qss)
    {
        addFragment(fragment.first, true);
    }
    
    return *this;
}

void QSSText::parse(const QString& input)
{
    auto insideStr = false;
    QString fragment;

    for (auto i = 0; i < input.size(); ++i)
    {
        fragment += input.at(i);

        if (input.at(i) == '"')
        {
            insideStr = !insideStr;
        }

        if (insideStr)
            continue;
        else
        {
            if (input.at(i) == QSSDelimiters.at(QSS_BLOCK_END_DELIMITER))
            {
                m_fragments.emplace_back(fragment, true);
                fragment.clear();
            }
        }
    }
}

QString QSSText::toString() const
{
    QString result;

    for (const auto& pair : m_fragments)
    {
        if (pair.second)
        {
            result += pair.first.toString() + "\n";
        }
    }

    return result;
}

std::size_t QSSText::totalActiveFragments() const
{
    return std::count_if(m_fragments.cbegin(), m_fragments.cend(), [](const QSSFragmentPair& pair){
        return pair.second;
    });
}

QSSText operator+(const QSSText & lhs, const QSSText & rhs)
{
    QSSText sum = lhs;
    sum += rhs;
    return sum;
}
