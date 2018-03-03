#ifndef QSSSELECTORFRAGMENT_H
#define QSSSELECTORFRAGMENT_H

#include "qssparseable.h"
#include "qssexception.h"

namespace qss
{
    class SelectorElement : public IParseable
    {
    public:

        enum PositionType
        {
            ADJACENT, PARENT, CHILD, DESCENDANT, SIBLING, GENERAL_SIBLING
        };

        SelectorElement() {}
        SelectorElement(const QString& str);
        SelectorElement& operator=(const SelectorElement& fragment);

        SelectorElement& select(const QString& sel);
        SelectorElement& on(const QString& key, const QString& value);
        SelectorElement& on(const QStringPairs& params);
        SelectorElement& sub(const QString& name);
        SelectorElement& when(const QString& pcl);
        SelectorElement& name(const QString& str);

        void    parse(const QString& input);
        QString toString() const;
        bool    isGeneralizedFrom(const SelectorElement& fragment) const;
        bool    isSpecificThan(const SelectorElement& fragment) const;
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

        friend bool operator==(const SelectorElement& lhs, const SelectorElement& rhs);

    private:

        static const std::unordered_map<int, QString> Combinators;

        friend class Selector;

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
}

#endif // QSSSELECTORFRAGMENT_H
