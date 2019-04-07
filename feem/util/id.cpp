#include "id.h"

//#include <utils/algorithm.h>
//#include <utils/qtcassert.h>

#include <QByteArray>
#include <QDataStream>
#include <QDebug>
#include <QHash>
#include <QVariant>

#include <string.h>

//namespace Core {

/*!
    \class Core::Id

    \brief The Id class encapsulates an identifier that is unique
    within a specific running \QC process.

    \c{Core::Id} is used as facility to identify objects of interest
    in a more typesafe and faster manner than a plain \c QString or
    \c QByteArray would provide.

    An id is associated with a plain 7-bit-clean ASCII name used
    for display and persistency.

*/

class StringHolder
{
public:
    StringHolder() = default;

    StringHolder(const char *s, int length)
        : n(length), str(s)
    {
        if (!n)
            length = n = static_cast<int>(strlen(s));
        h = 0;
        while (length--) {
            h = (h << 4) + *s++;
            h ^= (h & 0xf0000000) >> 23;
            h &= 0x0fffffff;
        }
    }
    int n = 0;
    const char *str = nullptr;
    quintptr h;
};

static bool operator==(const StringHolder &sh1, const StringHolder &sh2)
{
    // sh.n is unlikely to discriminate better than the hash.
    return sh1.h == sh2.h && sh1.str && sh2.str && strcmp(sh1.str, sh2.str) == 0;
}


static uint qHash(const StringHolder &sh)
{
    return QT_PREPEND_NAMESPACE(qHash)(sh.h, 0);
}

struct IdCache : public QHash<StringHolder, quintptr>
{
#ifndef QTC_ALLOW_STATIC_LEAKS
    ~IdCache()
    {
        for (IdCache::iterator it = begin(); it != end(); ++it)
            delete[](const_cast<char *>(it.key().str));
    }
#endif
};


static QHash<quintptr, StringHolder> stringFromId;
static IdCache idFromString;

static quintptr theId(const char *str, int n = 0)
{
    static quintptr firstUnusedId = 10 * 1000 * 1000;
    if(str && *str)
        ;
    else
        return 0;
    StringHolder sh(str, n);
    int res = idFromString.value(sh, 0);
    if (res == 0) {
        res = firstUnusedId++;
        sh.str = qstrdup(sh.str);
        idFromString[sh] = res;
        stringFromId[res] = sh;
    }
    return res;
}

static quintptr theId(const QByteArray &ba)
{
    return theId(ba.constData(), ba.size());
}

/*!
    \fn Core::Id::Id(quintptr uid)
    \internal

    构造一个给定\ UID的id。

    UID是一个整数值，在运行\ QC过程中是唯一的。

*/

/*!
根据其关联的名称构造一个id。
内部表示将是未指定的，但在\ QC过程中是一致的。

*/
Id::Id(const char *name)
    : m_id(theId(name, 0))
{}

/*!
  返回id的内部表示。
*/

QByteArray Id::name() const
{
    return stringFromId.value(m_id).str;
}

/*!
  返回适合UI显示的id的字符串表示形式。

  这不应该用于创建Id的持久版本，而是使用\ c {toSetting（）}。

  \sa fromString(), toSetting()
*/

QString Id::toString() const
{
    return QString::fromUtf8(stringFromId.value(m_id).str);
}

/*!
  从字符串表示创建id。

  这不应该用于处理Id的持久版本，而是使用\ c {fromSetting（）}。

  \deprecated

  \sa toString(), fromSetting()
*/

Id Id::fromString(const QString &name)
{
    if (name.isEmpty())
        return Id();
    return Id(theId(name.toUtf8()));
}

/*!
  从字符串表示创建id。

  这不应该用于处理Id的持久版本，而是使用\ c {fromSetting（）}。

  \deprecated

  \sa toString(), fromSetting()
*/

Id Id::fromName(const QByteArray &name)
{
    return Id(theId(name));
}

/*!
  返回表示适合存储在QSettings中的id的持久值。

  \sa fromSetting()
*/

QVariant Id::toSetting() const
{
    return QVariant(QString::fromUtf8(stringFromId.value(m_id).str));
}

/*!
  从持久值重构id。

  \sa toSetting()
*/

Id Id::fromSetting(const QVariant &variant)
{
    const QByteArray ba = variant.toString().toUtf8();
    if (ba.isEmpty())
        return Id();
    return Id(theId(ba));
}

Id Id::versionedId(const QByteArray &prefix, int major, int minor)
{
    if(major < 0) return fromName(prefix);

    QByteArray result = prefix + '.';
    result += QString::number(major).toLatin1();

    if (minor < 0)
        return fromName(result);
    return fromName(result + '.' + QString::number(minor).toLatin1());
}

//QSet<Id> Id::fromStringList(const QStringList &list)
//{
//    return QSet<Id>::fromList(Utils::transform(list, &Id::fromString));
//}

//QStringList Id::toStringList(const QSet<Id> &ids)
//{
//    QList<Id> idList = ids.toList();
//    Utils::sort(idList);
//    return Utils::transform(idList, &Id::toString);
//}

/*!
  构造派生id。

这可以用于构建逻辑上属于一起的id组。
将通过附加\ a后缀生成关联的内部名称。
*/

Id Id::withSuffix(int suffix) const
{
    const QByteArray ba = name() + QByteArray::number(suffix);
    return Id(ba.constData());
}

/*!
  \overload
*/

Id Id::withSuffix(const char *suffix) const
{
    const QByteArray ba = name() + suffix;
    return Id(ba.constData());
}

/*!
  \overload

  \sa stringSuffix()
*/

Id Id::withSuffix(const QString &suffix) const
{
    const QByteArray ba = name() + suffix.toUtf8();
    return Id(ba.constData());
}

/*!
  构造派生id。

这可以用于构建逻辑上属于一起的id组。
相关的内部名称将通过前缀\前缀生成。
*/

Id Id::withPrefix(const char *prefix) const
{
    const QByteArray ba = prefix + name();
    return Id(ba.constData());
}


bool Id::operator==(const char *name) const
{
    const char *string = stringFromId.value(m_id).str;
    if (string && name)
        return strcmp(string, name) == 0;
    else
        return false;
}

// For debugging purposes
//CORE_EXPORT const char *nameForId(quintptr id)
//{
//    return stringFromId.value(id).str;
//}

bool Id::alphabeticallyBefore(Id other) const
{
    return toString().compare(other.toString(), Qt::CaseInsensitive) < 0;
}


/*!
提取id字符串表示的一部分。
此函数可用于拆分由\ a {baseSuffix（）}生成id时使用的\ baseId指定的基本部分。

  \sa withSuffix()
*/

QString Id::suffixAfter(Id baseId) const
{
    const QByteArray b = baseId.name();
    const QByteArray n = name();
    return n.startsWith(b) ? QString::fromUtf8(n.mid(b.size())) : QString();
}

//} // namespace Core

QT_BEGIN_NAMESPACE

QDataStream &operator<<(QDataStream &ds, Id id)
{
    return ds << id.name();
}

QDataStream &operator>>(QDataStream &ds, Id &id)
{
    QByteArray ba;
    ds >> ba;
    id = Id::fromName(ba);
    return ds;
}

QDebug operator<<(QDebug dbg, const Id &id)
{
    return dbg << id.name();
}

QT_END_NAMESPACE
