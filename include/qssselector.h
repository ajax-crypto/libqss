#ifndef QSSSELECTOR_H
#define QSSSELECTOR_H

#include "qssselectorfragment.h"

class QSSSelector : public QSSBase
{
public:

    typedef typename std::deque<QSSSelectorFragment>::const_iterator ConstItr;
    typedef typename std::deque<QSSSelectorFragment>::iterator Itr;

    QSSSelector() {}
    QSSSelector(const QString& str);
    QSSSelector& operator=(const QSSSelector& selector);

    QSSSelector& child(const QSSSelectorFragment& fragment);
    QSSSelector& child(const QString& fragment);
    QSSSelector& descendant(const QSSSelectorFragment& fragment);
    QSSSelector& descendant(const QString& fragment);
    QSSSelector& generalSibling(const QSSSelectorFragment& fragment);
    QSSSelector& generalSibling(const QString& fragment);
    QSSSelector& sibling(const QSSSelectorFragment& fragment);
    QSSSelector& sibling(const QString& fragment);
    QSSSelector& append(const QString& fragment, QSSSelectorFragment::PositionType type);
    QSSSelector& append(const QSSSelectorFragment& fragment, QSSSelectorFragment::PositionType type);

    void    parse(const QString& input);
    QString toString() const;
    std::size_t fragmentCount() const  noexcept { return m_fragments.size(); }

    ConstItr cbegin() const noexcept { return m_fragments.cbegin(); }
    ConstItr cend() const noexcept { return m_fragments.cend(); }
    ConstItr begin() const noexcept { return m_fragments.cbegin(); }
    ConstItr end() const noexcept { return m_fragments.cend(); }
    Itr      begin() noexcept { return m_fragments.begin(); }
    Itr      end() noexcept { return m_fragments.end(); }

    const QSSSelectorFragment& front() const { return m_fragments.front(); }
    const QSSSelectorFragment& back() const { return m_fragments.back(); }
    
    QSSSelectorFragment& front() { return m_fragments.front(); }
    QSSSelectorFragment& back() { return m_fragments.back(); }

    QSSSelectorFragment& operator[](int index) { return m_fragments[index]; }
    const QSSSelectorFragment& operator[](int index) const { return m_fragments[index]; }

    friend bool operator==(const QSSSelector& lhs, const QSSSelector& rhs);

private:

    void preProcess(QString& str);

    const static char PreProcessChar = '`';
    const static std::unordered_map<QString, QSSSelectorFragment::PositionType, QStringHasher> Combinators;

    std::deque<QSSSelectorFragment> m_fragments;
};

#endif // QSSSELECTOR_H
