#ifndef PF_ACTIONHANDLER_H
#define PF_ACTIONHANDLER_H

#include <QObject>

class PF_ActionHandler : public QObject
{
    Q_OBJECT
public:
    PF_ActionHandler(QObject *parent = nullptr);

signals:

public slots:
    void slotFileNew();
    void slotFileOpen();
};

#endif // PF_ACTIONHANDLER_H
