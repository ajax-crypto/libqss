#ifndef QSSSELECTOR_H
#define QSSSELECTOR_H

#include "qssselectorelement.h"

namespace qss
{
    class Selector : public IParseable
    {
    public:

        typedef typename std::deque<SelectorElement>::const_iterator ConstItr;
        typedef typename std::deque<SelectorElement>::iterator Itr;

        Selector() {}
        Selector(const QString& str);
        Selector& operator=(const Selector& selector);

        Selector& child(const SelectorElement& fragment);
        Selector& child(const QString& fragment);
        Selector& descendant(const SelectorElement& fragment);
        Selector& descendant(const QString& fragment);
        Selector& generalSibling(const SelectorElement& fragment);
        Selector& generalSibling(const QString& fragment);
        Selector& sibling(const SelectorElement& fragment);
        Selector& sibling(const QString& fragment);
        Selector& append(const QString& fragment, SelectorElement::PositionType type);
        Selector& append(const SelectorElement& fragment, SelectorElement::PositionType type);

        void    parse(const QString& input);
        QString toString() const;
        std::size_t fragmentCount() const  noexcept { return m_fragments.size(); }

        ConstItr cbegin() const noexcept { return m_fragments.cbegin(); }
        ConstItr cend() const noexcept { return m_fragments.cend(); }
        ConstItr begin() const noexcept { return m_fragments.cbegin(); }
        ConstItr end() const noexcept { return m_fragments.cend(); }
        Itr      begin() noexcept { return m_fragments.begin(); }
        Itr      end() noexcept { return m_fragments.end(); }

        const SelectorElement& front() const { return m_fragments.front(); }
        const SelectorElement& back() const { return m_fragments.back(); }

        SelectorElement& front() { return m_fragments.front(); }
        SelectorElement& back() { return m_fragments.back(); }

        SelectorElement& operator[](int index) { return m_fragments[index]; }
        const SelectorElement& operator[](int index) const { return m_fragments[index]; }

        friend bool operator==(const Selector& lhs, const Selector& rhs);

    private:

        void preProcess(QString& str);

        const static char PreProcessChar = '`';
        const static std::unordered_map<QString, SelectorElement::PositionType, QStringHasher> Combinators;

        std::deque<SelectorElement> m_fragments;
    };
}

#endif // QSSSELECTOR_H
