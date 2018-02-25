#include "../include/qssblock.h"

QSSBlock::QSSBlock(const QString & str)
{
    parse(str);
}

QSSBlock &QSSBlock::operator=(const QSSBlock &block)
{
    m_params = block.m_params;
    return *this;
}

QSSBlock &QSSBlock::param(const QString &key, const QString &value)
{
    auto tkey = key.trimmed();
    m_params[tkey].first = value.trimmed();
    m_params[tkey].second = true;
    return *this;
}

QSSBlock &QSSBlock::param(const QStringPairs &params)
{
    for (const auto& param : params)
    {
        auto tkey = param.first.trimmed();
        m_params[tkey].first = param.second.trimmed();
        m_params[tkey].second = true;
    }

    return *this;
}

QSSBlock &QSSBlock::enableParam(const QString &key, bool enable)
{
    auto tkey = key.trimmed();
    auto itr = m_params.find(tkey);

    if (itr != m_params.cend())
    {
        m_params[tkey].second = enable;
    }

    return *this;
}

QSSBlock &QSSBlock::toggleParam(const QString &key)
{
    auto tkey = key.trimmed();
    auto itr = m_params.find(tkey);

    if (itr != m_params.cend())
    {
        m_params[tkey].second = !itr->second.second;
    }

    return *this;
}

QSSBlock &QSSBlock::remove(const QString &key)
{
    if (m_params.count(key))
    {
        m_params.erase(key);
    }

    return *this;
}

QSSBlock &QSSBlock::remove(const std::vector<QString> &keys)
{
    for (auto const& key : keys)
    {
        remove(key);
    }

    return *this;
}

QSSBlock& QSSBlock::operator+=(const QSSBlock & block)
{
    for (const auto& pair : block.m_params)
    {
        m_params[pair.first] = pair.second;
    }

    return *this;
}

QSSBlock & QSSBlock::operator+=(const QString & block)
{
    return this->operator+=(QSSBlock{ block });
}

void QSSBlock::parse(const QString &input)
{
    auto str = input.trimmed();

    if (str.size() > 0)
    {
        auto parts = str.split(QSSDelimiters.at(QSS_STATEMENT_END_DELIMITER), QString::SplitBehavior::SkipEmptyParts);

        for (auto const& part : parts)
        {
            auto line = part.split(QSSDelimiters.at(QSS_PSEUDO_CLASS_DELIMITER), QString::SplitBehavior::SkipEmptyParts);

            if (line.size() == 2)
            {
                auto key = line[0].trimmed();
                auto value = line[1].trimmed();
                m_params[key].first = value;
                m_params[key].second = true;
            }
            else
            {
                throw QSSException{ QSSException::BLOCK_PARAM_INVALID, part };
            }
        }
    }
}

QString QSSBlock::toString() const
{
    QString result;

    for (auto const& pair : m_params)
    {
        if (pair.second.second)
        {
            result += "\t" + pair.first + QSSDelimiters.at(QSS_PSEUDO_CLASS_DELIMITER);
            result += " " + pair.second.first + QSSDelimiters.at(QSS_STATEMENT_END_DELIMITER) + "\n";
        }
    }

    return result;
}

std::size_t QSSBlock::size() const noexcept
{
    return m_params.size();
}

bool operator==(const QSSBlock & lhs, const QSSBlock & rhs)
{
    return lhs.toString() == rhs.toString();
}

QSSBlock operator+(const QSSBlock & lhs, const QSSBlock & rhs)
{
    QSSBlock sum = lhs;
    sum += rhs;
    return sum;
}
