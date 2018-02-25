#include "../include/qssselectorfragment.h"

QSSSelectorFragment::QSSSelectorFragment(const QString & str)
{
    parse(str);
}

QSSSelectorFragment &QSSSelectorFragment::operator=(const QSSSelectorFragment &fragment)
{
    m_psuedoClass = fragment.m_psuedoClass;
    m_name = fragment.m_name;
    m_params = fragment.m_params;
    m_id = fragment.m_id;
    m_position = fragment.m_position;
    return *this;
}

QSSSelectorFragment &QSSSelectorFragment::select(const QString &sel)
{
    m_name = sel.trimmed();
    return *this;
}

QSSSelectorFragment &QSSSelectorFragment::on(const QString &key, const QString &value)
{
    m_params[key.trimmed()] = value;
    return *this;
}

QSSSelectorFragment &QSSSelectorFragment::on(const QStringPairs &params)
{
    for (const auto& param : params)
    {
        m_params[param.first.trimmed()] = param.second;
    }
    return *this;
}

QSSSelectorFragment &QSSSelectorFragment::sub(const QString &name)
{
    m_subControl = name;
    return *this;
}

QSSSelectorFragment &QSSSelectorFragment::when(const QString &pcl)
{
    m_psuedoClass = pcl.trimmed();
    return *this;
}

QSSSelectorFragment &QSSSelectorFragment::name(const QString &str)
{
    m_id = str.trimmed();
    return *this;
}

void QSSSelectorFragment::parse(const QString &str)
{
    auto selector = str.trimmed();

    if (selector.size() != 0)
    {
        auto remaining = extractSubControlAndPsuedoClass(selector);
        remaining = extractParams(remaining);
        extractNameAndSelector(remaining);
    }
}

QString QSSSelectorFragment::toString() const
{
    QString result;

    if (m_name.size() > 0)
    {
        result += m_position != PARENT ? Combinators.at(m_position) + " " + m_name : m_name;
    }

    if (m_id.size() > 0)
    {
        result += QSSDelimiters.at(QSS_NAME_DELIMITER) + m_id;
    }

    for (const auto& cl : m_classes)
    {
        result += QSSDelimiters.at(QSS_CLASS_DELIMITER) + cl;
    }

    for (auto const& pair : m_params)
    {
        result += QSSDelimiters.at(QSS_SELECT_PARAM_START_DELIMITER) +
                pair.first + QSSDelimiters.at(QSS_PARAM_DELIMITER) +
                QuotedString(pair.second) + QSSDelimiters.at(QSS_SELECT_PARAM_END_DELIMITER);
    }

    if (m_subControl.size() > 0)
    {
        result += QSSDelimiters.at(QSS_SUB_CONTROL_DELIMITER) + m_subControl;
    }

    if (m_psuedoClass.size() > 0)
    {
        result += QSSDelimiters.at(QSS_PSEUDO_CLASS_DELIMITER) + m_psuedoClass;
    }

    return result;
}

bool QSSSelectorFragment::isGeneralizedFrom(const QSSSelectorFragment &fragment) const
{
    // The params of this should all be contained in fragment, since
    // params is generalized from fragment i.e. fragment is more specific
    for (const auto& pair : m_params)
    {
        auto itr = fragment.m_params.find(pair.first);

        if (itr == fragment.m_params.cend())
        {
            return false;
        }
        else
        {
            if (itr->second != pair.second)
            {
                return false;
            }
        }
    }

    for (const auto& cl : m_classes)
    {
        auto itr = std::find(fragment.m_classes.cbegin(), fragment.m_classes.cend(), cl);

        if (itr == fragment.m_classes.cend())
        {
            return false;
        }
    }

    if (!m_id.isEmpty() && fragment.m_id != m_id)
    {
        return false;
    }

    if (!m_name.isEmpty() && fragment.m_name != m_name)
    {
        return false;
    }

    // this's pseudo class should be empty or same as fragment
    if (!m_psuedoClass.isEmpty() && fragment.m_psuedoClass != m_psuedoClass)
    {
        return false;
    }

    if (!m_subControl.isEmpty() && fragment.m_subControl != m_subControl)
    {
        return false;
    }

    return true;
}

bool QSSSelectorFragment::isSpecificThan(const QSSSelectorFragment &fragment) const
{
    return fragment.isGeneralizedFrom(*this);
}

QString QSSSelectorFragment::value(const QString & key) const
{
    auto itr = m_params.find(key);

    if (itr != m_params.cend())
    {
        return itr->second;
    }

    return QString{};
}

QString QSSSelectorFragment::extractSubControlAndPsuedoClass(const QString &str)
{
    auto parts = str.split(QSSDelimiters.at(QSS_PSEUDO_CLASS_DELIMITER));

    if (parts.size() > 1)
    {
        if (parts[1].size() == 0 && parts.size() > 2)
        {
            m_subControl = parts[2];
            if (parts.size() > 3)
            {
                m_psuedoClass = parts[3];
            }
        }
        else
        {
            m_psuedoClass = parts[1];
        }
    }

    return parts[0];
}

QString QSSSelectorFragment::extractParams(const QString &str)
{
    auto parts = str.split(QSSDelimiters.at(QSS_SELECT_PARAM_START_DELIMITER), QString::SkipEmptyParts);

    if (parts.size() == 1 && str.indexOf(QSSDelimiters.at(QSS_SELECT_PARAM_START_DELIMITER)) != -1)
    {
        throw QSSException{ QSSException::ILL_FORMED_HEADER_PARAM, str };
    }

    if (parts.size() > 1)
    {
        for (int i = 1; i < parts.size(); ++i)
        {
            auto params = parts[i].split(QSSDelimiters.at(QSS_PARAM_DELIMITER));
            if (params.size() != 2)
            {
                throw QSSException{ QSSException::HEADER_PARAM_INVALID, parts[i] };
            }
            else
            {
                auto value = params[1].remove(0, 1);
                auto size = value.size();
                value = value.remove(size - 2, 2);
                m_params[params[0]] = value;
            }
        }
    }

    return parts[0];
}

void QSSSelectorFragment::extractNameAndSelector(const QString &str)
{
    auto select = str.split(QSSDelimiters.at(QSS_NAME_DELIMITER));
    m_name = select[0];

    if (select.size() == 2)
    {
        auto parts = select[1].split(QSSDelimiters.at(QSS_CLASS_DELIMITER), QString::SkipEmptyParts);
        m_id = parts[0].trimmed();

        for (auto i = 1; i < parts.size(); ++i)
        {
            m_classes.push_back(parts[i]);
        }
    }
    else if (select.size() > 2)
    {
        throw QSSException{ QSSException::MULTIPLE_NAMES, str };
    }
}

bool operator==(const QSSSelectorFragment &lhs, const QSSSelectorFragment &rhs)
{
    return lhs.toString() == rhs.toString();
}

const std::unordered_map<int, QString> QSSSelectorFragment::Combinators{
    { QSSSelectorFragment::CHILD, ">" }, { QSSSelectorFragment::DESCENDANT, "" },
    { QSSSelectorFragment::SIBLING, "~" }, { QSSSelectorFragment::GENERAL_SIBLING, "+" },
    { QSSSelectorFragment::PARENT, "" }, { QSSSelectorFragment::ADJACENT, "," }
};

