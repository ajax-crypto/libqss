#ifndef QSSSELECTORFRAGMENT_H
#define QSSSELECTORFRAGMENT_H

#include "qssbase.h"
#include "qssexception.h"

class QSSSelectorFragment : public QSSBase
{
public:

    enum PositionType
    {
        ADJACENT, PARENT, CHILD, DESCENDANT, SIBLING, GENERAL_SIBLING
    };

    QSSSelectorFragment() {}
    QSSSelectorFragment(const QString& str);
    QSSSelectorFragment& operator=(const QSSSelectorFragment& fragment);

    QSSSelectorFragment& select(const QString& sel);
    QSSSelectorFragment& on(const QString& key, const QString& value);
    QSSSelectorFragment& on(const QStringPairs& params);
    QSSSelectorFragment& sub(const QString& name);
    QSSSelectorFragment& when(const QString& pcl);
    QSSSelectorFragment& name(const QString& str);

    void    parse(const QString& input);
    QString toString() const;
    bool    isGeneralizedFrom(const QSSSelectorFragment& fragment) const;
    bool    isSpecificThan(const QSSSelectorFragment& fragment) const;
    QString id() const { return m_id; }
    QString psuedoClass() const { return m_psuedoClass; }
    QString subControl() const { return m_subControl; }
    QString name() const { return m_name; }
    QString value(const QString& key) const;

    PositionType position() const noexcept { return m_position; }

    const QStringList& classes() const noexcept { return m_classes; }
    const QStringMap& params() const noexcept { return m_params; }

    std::size_t classCount() const noexcept { return m_classes.size(); }
    std::size_t paramCount() const noexcept { return m_params.size(); }
    
    friend bool operator==(const QSSSelectorFragment& lhs, const QSSSelectorFragment& rhs);

private:

    static const std::unordered_map<int, QString> Combinators;

    friend class QSSSelector;

    QString extractSubControlAndPsuedoClass(const QString& str);
    QString extractParams(const QString& str);
    void    extractNameAndSelector(const QString& str);

    QString      m_name;
    QString      m_id;
    QString      m_subControl;
    QString      m_psuedoClass;
    QStringMap   m_params;
    PositionType m_position = PARENT;
    QStringList  m_classes;
};

#endif // QSSSELECTORFRAGMENT_H
