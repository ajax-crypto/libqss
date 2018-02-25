#ifndef QSSTEXT_H
#define QSSTEXT_H

#include "qssfragment.h"

#include <algorithm>

class QSSText : public QSSBase
{
public:

    typedef InvalidablePair<QSSFragment> QSSFragmentPair;
    typedef typename std::deque<QSSFragmentPair>::const_iterator ConstItr;
    typedef typename std::deque<QSSFragmentPair>::iterator Itr;

    QSSText(const QString& qss);
    virtual ~QSSText() {}

    QSSText& addFragment(const QSSFragment& fragment, bool enabled = true);
    QSSText& addFragment(const QString& fragment, bool enabled = true);
    QSSText& enableFragment(int index, bool enable = true);
    QSSText& toggleFragment(int index);
    QSSText& removeFragment(const QString& fragment);
    QSSText& removeFragment(int index);
    QSSText& operator+=(const QString& fragment);
    QSSText& operator+=(const QSSText& qss);

    void parse(const QString&);
    QString toString() const;

    const QSSFragment& operator[](int index) const { return m_fragments[index].first; }
    std::size_t totalFragments() const noexcept { return m_fragments.size(); }
    std::size_t totalActiveFragments() const;

    ConstItr cbegin() const noexcept { return m_fragments.cbegin(); }
    ConstItr cend() const noexcept { return m_fragments.cend(); }
    ConstItr begin() const noexcept { return m_fragments.cbegin(); }
    ConstItr end() const noexcept { return m_fragments.cend(); }
    Itr      begin() noexcept { return m_fragments.begin(); }
    Itr      end() noexcept { return m_fragments.end(); }

    const QSSFragment& front() const noexcept { return m_fragments.front().first; }
    const QSSFragment& back() const noexcept { return m_fragments.back().first; }

    QSSFragment& front() noexcept { return m_fragments.front().first; }
    QSSFragment& back() noexcept { return m_fragments.front().first; }

    friend QSSText operator+(const QSSText& lhs, const QSSText& rhs);

private:

    std::deque<QSSFragmentPair> m_fragments;
};

#endif // QSSTEXT_H
