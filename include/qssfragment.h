#ifndef QSSFRAGMENT_H
#define QSSFRAGMENT_H

#include "qssselector.h"
#include "qsspropertyblock.h"

namespace qss
{
    class Fragment : public IParseable
    {
    public:

        Fragment() {}
        Fragment(const QString& str);
        Fragment& operator=(const Fragment& fragment);

        Fragment& select(const Selector& selector);
        Fragment& select(const QString& selector);
        Fragment& addBlock(const PropertyBlock& block);
        Fragment& addBlock(const QString& block);
        Fragment& addBlock(const QStringPairs& block);
        Fragment& addParam(const QStringPair& param);
        Fragment& addParam(const QString& key, const QString& val);
        Fragment& enableParam(const QString& key, bool enable = false);
        Fragment& remove(const QString& name);
        Fragment& remove(const std::vector<QString>& names);

        const Selector& selector() const noexcept { return m_selector; }
        const PropertyBlock& block() const noexcept { return m_block; }

        Selector& selector() noexcept { return m_selector; }
        PropertyBlock& block() noexcept { return m_block; }

        void    parse(const QString& input);
        QString toString() const;

        friend bool operator==(const Fragment& lhs, const Fragment& rhs);

    private:

        Selector m_selector;
        PropertyBlock    m_block;
    };
}

#endif // QSSFRAGMENT_H
