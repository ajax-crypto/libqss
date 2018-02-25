#ifndef QSSBLOCK_H
#define QSSBLOCK_H

#include "qssbase.h"
#include "qssexception.h"

class QSSBlock : public QSSBase
{
public:

    typedef typename QssPropertyMap::const_iterator ConstItr;
    typedef typename QssPropertyMap::iterator Itr;

    QSSBlock() {}
    QSSBlock(const QString& str);
    QSSBlock& operator=(const QSSBlock& block);

    QSSBlock& param(const QString& key, const QString& value);
    QSSBlock& param(const QStringPairs& params);
    QSSBlock& enableParam(const QString& key, bool enable = true);
    QSSBlock& toggleParam(const QString& key);
    QSSBlock& remove(const QString& key);
    QSSBlock& remove(const std::vector<QString>& keys);

    QSSBlock& operator+=(const QSSBlock& block);
    QSSBlock& operator+=(const QString& block);

    void    parse(const QString& input);
    QString toString() const;
    std::size_t size() const noexcept;

    ConstItr cbegin() const noexcept { return m_params.cbegin(); }
    ConstItr cend() const noexcept { return m_params.cend(); }
    Itr      begin() noexcept { return m_params.begin(); }
    Itr      end() noexcept { return m_params.end(); }

    friend bool operator==(const QSSBlock& lhs, const QSSBlock& rhs);
    friend QSSBlock operator+(const QSSBlock& lhs, const QSSBlock& rhs);

private:

    QssPropertyMap m_params;
};

#endif // QSSBLOCK_H
