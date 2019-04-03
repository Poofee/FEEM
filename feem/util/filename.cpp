#include "filename.h"


#include <QFileInfo>
#include <QDir>

/*! \class Utils::FileName

    \brief The FileName class is a light-weight convenience class for filenames.

    On windows filenames are compared case insensitively.
*/

FileName::FileName()
    : QString()
{

}

/// Constructs a FileName from \a info
FileName::FileName(const QFileInfo &info)
    : QString(info.absoluteFilePath())
{
}

/// \returns a QFileInfo
QFileInfo FileName::toFileInfo() const
{
    return QFileInfo(*this);
}

/// \returns a QString for passing on to QString based APIs
const QString &FileName::toString() const
{
    return *this;
}

/// \returns a QString to display to the user
/// Converts the separators to the native format
QString FileName::toUserOutput() const
{
    return QDir::toNativeSeparators(toString());
}

QString FileName::fileName(int pathComponents) const
{
    if (pathComponents < 0)
        return *this;
    const QChar slash = QLatin1Char('/');
    int i = lastIndexOf(slash);
    if (pathComponents == 0 || i == -1)
        return mid(i + 1);
    int component = i + 1;
    // skip adjacent slashes
    while (i > 0 && at(--i) == slash);
    while (i >= 0 && --pathComponents >= 0) {
        i = lastIndexOf(slash, i);
        component = i + 1;
        while (i > 0 && at(--i) == slash);
    }

    // If there are no more slashes before the found one, return the entire string
    if (i > 0 && lastIndexOf(slash, i) != -1)
        return mid(component);
    return *this;
}

/// \returns a bool indicating whether a file with this
/// FileName exists.
bool FileName::exists() const
{
    return !isEmpty() && QFileInfo::exists(*this);
}

/// Find the parent directory of a given directory.

/// Returns an empty FileName if the current directory is already
/// a root level directory.

/// \returns \a FileName with the last segment removed.
FileName FileName::parentDir() const
{
    const QString basePath = toString();
    if (basePath.isEmpty())
        return FileName();

    const QDir base(basePath);
    if (base.isRoot())
        return FileName();

    const QString path = basePath + QLatin1String("/..");
    const QString parent = QDir::cleanPath(path);
    if(parent == path) return FileName();

    return FileName::fromString(parent);
}

/// Constructs a FileName from \a filename
/// \a filename is not checked for validity.
FileName FileName::fromString(const QString &filename)
{
    return FileName(filename);
}

/// Constructs a FileName from \a fileName. The \a defaultExtension is appended
/// to \a filename if that does not have an extension already.
/// \a fileName is not checked for validity.
FileName FileName::fromString(const QString &filename, const QString &defaultExtension)
{
    if (filename.isEmpty() || defaultExtension.isEmpty())
        return filename;

    QString rc = filename;
    QFileInfo fi(filename);
    // Add extension unless user specified something else
    const QChar dot = QLatin1Char('.');
    if (!fi.fileName().contains(dot)) {
        if (!defaultExtension.startsWith(dot))
            rc += dot;
        rc += defaultExtension;
    }
    return rc;
}

/// Constructs a FileName from \a fileName
/// \a fileName is not checked for validity.
FileName FileName::fromLatin1(const QByteArray &filename)
{
    return FileName(QString::fromLatin1(filename));
}

/// Constructs a FileName from \a fileName
/// \a fileName is only passed through QDir::cleanPath
FileName FileName::fromUserInput(const QString &filename)
{
    QString clean = QDir::cleanPath(filename);
    if (clean.startsWith(QLatin1String("~/")))
        clean = QDir::homePath() + clean.mid(1);
    return FileName(clean);
}

/// Constructs a FileName from \a fileName, which is encoded as UTF-8.
/// \a fileName is not checked for validity.
FileName FileName::fromUtf8(const char *filename, int filenameSize)
{
    return FileName(QString::fromUtf8(filename, filenameSize));
}

FileName::FileName(const QString &string)
    : QString(string)
{

}

bool FileName::operator==(const FileName &other) const
{
    return QString::compare(*this, other, Qt::CaseInsensitive) == 0;
}

bool FileName::operator!=(const FileName &other) const
{
    return !(*this == other);
}

bool FileName::operator<(const FileName &other) const
{
    return QString::compare(*this, other, Qt::CaseInsensitive) < 0;
}

bool FileName::operator<=(const FileName &other) const
{
    return QString::compare(*this, other, Qt::CaseInsensitive) <= 0;
}

bool FileName::operator>(const FileName &other) const
{
    return other < *this;
}

bool FileName::operator>=(const FileName &other) const
{
    return other <= *this;
}

FileName FileName::operator+(const QString &s) const
{
    FileName result(*this);
    result.appendString(s);
    return result;
}

/// \returns whether FileName is a child of \a s
bool FileName::isChildOf(const FileName &s) const
{
    if (s.isEmpty())
        return false;
    if (!QString::startsWith(s, Qt::CaseInsensitive))
        return false;
    if (size() <= s.size())
        return false;
    // s is root, '/' was already tested in startsWith
    if (s.QString::endsWith(QLatin1Char('/')))
        return true;
    // s is a directory, next character should be '/' (/tmpdir is NOT a child of /tmp)
    return at(s.size()) == QLatin1Char('/');
}

/// \overload
bool FileName::isChildOf(const QDir &dir) const
{
    return isChildOf(FileName::fromString(dir.absolutePath()));
}

/// \returns whether FileName endsWith \a s
bool FileName::endsWith(const QString &s) const
{
    return QString::endsWith(s, Qt::CaseInsensitive);
}

/// \returns the relativeChildPath of FileName to parent if FileName is a child of parent
/// \note returns a empty FileName if FileName is not a child of parent
/// That is, this never returns a path starting with "../"
FileName FileName::relativeChildPath(const FileName &parent) const
{
    if (!isChildOf(parent))
        return FileName();
    return FileName(QString::mid(parent.size() + 1, -1));
}

/// Appends \a s, ensuring a / between the parts
FileName &FileName::appendPath(const QString &s)
{
    if (s.isEmpty())
        return *this;
    if (!isEmpty() && !QString::endsWith(QLatin1Char('/')))
        appendString(QLatin1Char('/'));
    appendString(s);
    return *this;
}

FileName &FileName::appendString(const QString &str)
{
    QString::append(str);
    return *this;
}

FileName &FileName::appendString(QChar str)
{
    QString::append(str);
    return *this;
}
