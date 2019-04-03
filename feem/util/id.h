#ifndef ID_H
#define ID_H


//#include "core_global.h"

#include <QMetaType>
#include <QString>

QT_BEGIN_NAMESPACE
class QDataStream;
class QVariant;
QT_END_NAMESPACE

//namespace Core {

class Id
{
public:
    Id() = default;
    Id(const char *name); // Good to use.
    Id(const QLatin1String &) = delete;

    Id withSuffix(int suffix) const;
    Id withSuffix(const char *suffix) const;
    Id withSuffix(const QString &suffix) const;
    Id withPrefix(const char *prefix) const;

    QByteArray name() const;
    QString toString() const; // Avoid.
    QVariant toSetting() const; // Good to use.
    QString suffixAfter(Id baseId) const;
    bool isValid() const { return m_id; }
    bool operator==(Id id) const { return m_id == id.m_id; }
    bool operator==(const char *name) const;
    bool operator!=(Id id) const { return m_id != id.m_id; }
    bool operator!=(const char *name) const { return !operator==(name); }
    bool operator<(Id id) const { return m_id < id.m_id; }
    bool operator>(Id id) const { return m_id > id.m_id; }
    bool alphabeticallyBefore(Id other) const;

    quintptr uniqueIdentifier() const { return m_id; } // Avoid.
    static Id fromString(const QString &str); // FIXME: avoid.
    static Id fromName(const QByteArray &ba); // FIXME: avoid.
    static Id fromSetting(const QVariant &variant); // Good to use.

    static Id versionedId(const QByteArray &prefix, int major, int minor = -1);

//    static QSet<Id> fromStringList(const QStringList &list);
//    static QStringList toStringList(const QSet<Id> &ids);

private:
    explicit Id(quintptr uid) : m_id(uid) {}

    quintptr m_id = 0;
};

inline uint qHash(Id id) { return static_cast<uint>(id.uniqueIdentifier()); }

//} // namespace Core

Q_DECLARE_METATYPE(Id)
Q_DECLARE_METATYPE(QList<Id>)

QT_BEGIN_NAMESPACE
QDataStream &operator<<(QDataStream &ds, Id id);
QDataStream &operator>>(QDataStream &ds, Id &id);
QDebug operator<<(QDebug dbg, const Id &id);
QT_END_NAMESPACE

#endif // ID_H
