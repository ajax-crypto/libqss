#include "../include/qsspropertyblock.h"

qss::PropertyBlock::PropertyBlock(const QString & str)
{
    parse(str);
}

qss::PropertyBlock& qss::PropertyBlock::operator=(const PropertyBlock &block)
{
    m_params = block.m_params;
    return *this;
}

qss::PropertyBlock& qss::PropertyBlock::addParam(const QString &key, const QString &value)
{
    auto tkey = key.trimmed();
    m_params[tkey].first = value.trimmed();
    m_params[tkey].second = true;
    return *this;
}

qss::PropertyBlock& qss::PropertyBlock::addParam(const QStringPairs &params)
{
    for (const auto& param : params)
    {
        auto tkey = param.first.trimmed();
        m_params[tkey].first = param.second.trimmed();
        m_params[tkey].second = true;
    }

    return *this;
}

qss::PropertyBlock& qss::PropertyBlock::enableParam(const QString &key, bool enable)
{
    auto tkey = key.trimmed();
    auto itr = m_params.find(tkey);

    if (itr != m_params.cend())
    {
        m_params[tkey].second = enable;
    }

    return *this;
}

qss::PropertyBlock& qss::PropertyBlock::toggleParam(const QString &key)
{
    auto tkey = key.trimmed();
    auto itr = m_params.find(tkey);

    if (itr != m_params.cend())
    {
        m_params[tkey].second = !itr->second.second;
    }

    return *this;
}

qss::PropertyBlock& qss::PropertyBlock::remove(const QString &key)
{
    if (m_params.count(key))
    {
        m_params.erase(key);
    }

    return *this;
}

qss::PropertyBlock& qss::PropertyBlock::remove(const std::vector<QString> &keys)
{
    for (auto const& key : keys)
    {
        remove(key);
    }

    return *this;
}

qss::PropertyBlock& qss::PropertyBlock::operator+=(const PropertyBlock & block)
{
    for (const auto& pair : block.m_params)
    {
        m_params[pair.first] = pair.second;
    }

    return *this;
}

qss::PropertyBlock& qss::PropertyBlock::operator+=(const QString & block)
{
    return this->operator+=(PropertyBlock{ block });
}

void qss::PropertyBlock::parse(const QString &input)
{
    auto str = input.trimmed();

    if (str.size() > 0)
    {
        auto parts = str.split(Delimiters.at(QSS_STATEMENT_END_DELIMITER), Qt::SkipEmptyParts);

        for (auto const& part : parts)
        {
            auto line = part.split(Delimiters.at(QSS_PSEUDO_CLASS_DELIMITER), Qt::SkipEmptyParts);

            if (line.size() == 2)
            {
                auto key = line[0].trimmed();
                auto value = line[1].trimmed();
                m_params[key].first = value;
                m_params[key].second = true;
            }
            else
            {
                throw Exception{ Exception::BLOCK_PARAM_INVALID, part };
            }
        }
    }
}

QString qss::PropertyBlock::toString() const
{
    QString result;

    for (auto const& pair : m_params)
    {
        if (pair.second.second)
        {
            result += "\t" + pair.first + Delimiters.at(QSS_PSEUDO_CLASS_DELIMITER);
            result += " " + pair.second.first + Delimiters.at(QSS_STATEMENT_END_DELIMITER) + "\n";
        }
    }

    return result;
}

std::size_t qss::PropertyBlock::size() const noexcept
{
    return m_params.size();
}

bool qss::operator==(const PropertyBlock & lhs, const PropertyBlock & rhs)
{
    return lhs.toString() == rhs.toString();
}

qss::PropertyBlock qss::operator+(const PropertyBlock & lhs, const PropertyBlock & rhs)
{
    PropertyBlock sum = lhs;
    sum += rhs;
    return sum;
}
