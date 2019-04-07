#ifndef CONTEXT_H
#define CONTEXT_H

//#include <coreplugin/core_global.h>
#include "id.h"

#include <QList>
#include <QObject>
#include <QPointer>
#include <QWidget>

#include <functional>

//namespace Core {

class Context
{
public:
    Context() = default;

    explicit Context(Id c1) { add(c1); }
    Context(Id c1, Id c2) { add(c1); add(c2); }
    Context(Id c1, Id c2, Id c3) { add(c1); add(c2); add(c3); }
    bool contains(Id c) const { return d.contains(c); }
    int size() const { return d.size(); }
    bool isEmpty() const { return d.isEmpty(); }
    Id at(int i) const { return d.at(i); }

    // FIXME: Make interface slimmer.
    using const_iterator = QList<Id>::const_iterator;
    const_iterator begin() const { return d.begin(); }
    const_iterator end() const { return d.end(); }
    int indexOf(Id c) const { return d.indexOf(c); }
    void removeAt(int i) { d.removeAt(i); }
    void prepend(Id c) { d.prepend(c); }
    void add(const Context &c) { d += c.d; }
    void add(Id c) { d.append(c); }
    bool operator==(const Context &c) const { return d == c.d; }

private:
    QList<Id> d;
};

class IContext : public QObject
{
    Q_OBJECT
public:
    IContext(QObject *parent = nullptr) : QObject(parent) {}

    virtual Context context() const { return m_context; }
    virtual QWidget *widget() const { return m_widget; }
    using HelpIdCallback = std::function<void(const QString &id)>;
    virtual void contextHelpId(const HelpIdCallback &callback) const { callback(m_contextHelpId); }

    virtual void setContext(const Context &context) { m_context = context; }
    virtual void setWidget(QWidget *widget) { m_widget = widget; }
    virtual void setContextHelpId(const QString &id) { m_contextHelpId = id; }

protected:
    Context m_context;
    QPointer<QWidget> m_widget;
    QString m_contextHelpId;
};

QDebug operator<<(QDebug debug, const Context &context);
#endif // CONTEXT_H
