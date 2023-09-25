#include "../include/qssfragment.h"

qss::Fragment::Fragment(const QString & input)
{
    parse(input);
}

qss::Fragment& qss::Fragment::operator=(const Fragment &fragment)
{
    m_selector = fragment.m_selector;
    m_block = fragment.m_block;
    return *this;
}

qss::Fragment& qss::Fragment::select(const Selector& selector)
{
    m_selector = selector;
    return *this;
}

qss::Fragment& qss::Fragment::select(const QString &selector)
{
    m_selector.parse(selector);
    return *this;
}

qss::Fragment& qss::Fragment::addBlock(const PropertyBlock& block)
{
    m_block += block;
    return *this;
}

qss::Fragment& qss::Fragment::addBlock(const QString &block)
{
    m_block += block;
    return *this;
}

qss::Fragment& qss::Fragment::addBlock(const QStringPairs &block)
{
    m_block.addParam(block);
    return *this;
}

qss::Fragment& qss::Fragment::addParam(const QStringPair &param)
{
    m_block.addParam(param.first, param.second);
    return *this;
}

qss::Fragment& qss::Fragment::addParam(const QString &key, const QString &val)
{
    m_block.addParam(key, val);
    return *this;
}

qss::Fragment& qss::Fragment::enableParam(const QString &key, bool enable)
{
    m_block.enableParam(key, enable);
    return *this;
}

qss::Fragment& qss::Fragment::remove(const QString &name)
{
    m_block.remove(name);
    return *this;
}

qss::Fragment& qss::Fragment::remove(const std::vector<QString> &names)
{
    m_block.remove(names);
    return *this;
}

void qss::Fragment::parse(const QString &input)
{
    auto str = input.trimmed();

    if (str.size() > 0)
    {
        auto start = str.indexOf(Delimiters.at(QSS_BLOCK_START_DELIMITER));
        auto end = str.indexOf(Delimiters.at(QSS_BLOCK_END_DELIMITER));

        if ((end - start) > 0)
        {
            QString header = str, body = str;
            m_selector.parse(header.remove(start, header.size()).trimmed());
            body = body.left(end);
            body.remove(0, start + 1);
            m_block.parse(body.trimmed());
        }
        else
        {
            throw Exception{ Exception::BLOCK_BRACKETS_INVALID, str };
        }
    }
}

QString qss::Fragment::toString() const
{
    QString result = m_selector.toString();
    result += " " + Delimiters.at(QSS_BLOCK_START_DELIMITER) + "\n";
    result += m_block.toString() + Delimiters.at(QSS_BLOCK_END_DELIMITER) + "\n";
    return result;
}

bool qss::operator==(const Fragment &lhs, const Fragment &rhs)
{
    return lhs.toString() == rhs.toString();
}
