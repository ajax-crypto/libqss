#ifndef QSSFRAGMENT_H
#define QSSFRAGMENT_H

#include "qssselector.h"
#include "qssblock.h"

class QSSFragment : public QSSBase
{
public:

    QSSFragment() {}
    QSSFragment(const QString& str);
    QSSFragment& operator=(const QSSFragment& fragment);

    QSSFragment& select(const QSSSelector& selector);
    QSSFragment& select(const QString& selector);
    QSSFragment& add(const QSSBlock& block);
    QSSFragment& add(const QString& block);
    QSSFragment& add(const QStringPairs& block);
    QSSFragment& add(const QStringPair& param);
    QSSFragment& add(const QString& key, const QString& val);
    QSSFragment& enableParam(const QString& key, bool enable = false);
    QSSFragment& remove(const QString& name);
    QSSFragment& remove(const std::vector<QString>& names);

    const QSSSelector& selector() const noexcept { return m_selector; }
    const QSSBlock& block() const noexcept { return m_block; }

    QSSSelector& selector() noexcept { return m_selector; }
    QSSBlock& block() noexcept { return m_block; }

    void    parse(const QString& input);
    QString toString() const;

    friend bool operator==(const QSSFragment& lhs, const QSSFragment& rhs);

private:

    QSSSelector m_selector;
    QSSBlock    m_block;
};

#endif // QSSFRAGMENT_H
