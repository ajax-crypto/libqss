#include "../include/qssfragment.h"

QSSFragment::QSSFragment(const QString & str)
{
    parse(str);
}

QSSFragment& QSSFragment::operator=(const QSSFragment &fragment)
{
    m_selector = fragment.m_selector;
    m_block = fragment.m_block;
    return *this;
}

QSSFragment& QSSFragment::select(const QSSSelector& selector)
{
    m_selector = selector;
    return *this;
}

QSSFragment& QSSFragment::select(const QString &selector)
{
    m_selector.parse(selector);
    return *this;
}

QSSFragment& QSSFragment::add(const QSSBlock &block)
{
    m_block += block;
    return *this;
}

QSSFragment& QSSFragment::add(const QString &block)
{
    m_block += block;
    return *this;
}

QSSFragment& QSSFragment::add(const QStringPairs &block)
{
    m_block.param(block);
    return *this;
}

QSSFragment &QSSFragment::add(const QStringPair &param)
{
    m_block.param(param.first, param.second);
    return *this;
}

QSSFragment &QSSFragment::add(const QString &key, const QString &val)
{
    m_block.param(key, val);
    return *this;
}

QSSFragment &QSSFragment::enableParam(const QString &key, bool enable)
{
    m_block.enableParam(key, enable);
    return *this;
}

QSSFragment &QSSFragment::remove(const QString &name)
{
    m_block.remove(name);
    return *this;
}

QSSFragment &QSSFragment::remove(const std::vector<QString> &names)
{
    m_block.remove(names);
    return *this;
}

void QSSFragment::parse(const QString &input)
{
    auto str = input.trimmed();

    if (str.size() > 0)
    {
        auto start = input.indexOf(QSSDelimiters.at(QSS_BLOCK_START_DELIMITER));
        auto end = input.indexOf(QSSDelimiters.at(QSS_BLOCK_END_DELIMITER));

        if ((end - start) > 0)
        {
            QString header = str, body = str;
            m_selector.parse(header.remove(start, header.size()).trimmed());
            body.remove(0, start + 1);
            body.remove(body.size() - 1, 1);
            m_block.parse(body.trimmed());
        }
        else
        {
            throw QSSException{ QSSException::BLOCK_BRACKETS_INVALID, str };
        }
    }
}

QString QSSFragment::toString() const
{
    QString result = m_selector.toString();
    result += " " + QSSDelimiters.at(QSS_BLOCK_START_DELIMITER) + "\n";
    result += m_block.toString() + QSSDelimiters.at(QSS_BLOCK_END_DELIMITER) + "\n";
    return result;
}

bool operator==(const QSSFragment &lhs, const QSSFragment &rhs)
{
    return lhs.toString() == rhs.toString();
}
