#ifndef IOUTPUTPANE_H
#define IOUTPUTPANE_H

#include <QObject>
#include <QString>
#include <QList>

class IOutputPane : public QObject
{
    Q_OBJECT
public:
    enum Flag { NoModeSwitch = 0, ModeSwitch = 1, WithFocus = 2, EnsureSizeHint = 4};
    Q_DECLARE_FLAGS(Flags, Flag)
public:
    IOutputPane(QObject* parent = nullptr);
    ~IOutputPane() override;

    virtual QString displayName() const = 0;

    virtual void clearContents() = 0;
public slots:

signals:


};

#endif // IOUTPUTPANE_H
