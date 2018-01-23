#ifndef PF_ACTIONFACTORY_H
#define PF_ACTIONFACTORY_H

#include <QObject>
#include <QMap>

class QActionGroup;
class QAction;
class PF_ActionGroupManager;

class PF_ActionFactory : public QObject
{
    Q_OBJECT
public:
    PF_ActionFactory(QObject *parent, QObject* a_handler);
    void fillActionContainer(QMap<QString, QAction*>& a_map, PF_ActionGroupManager* agm);

signals:

public slots:

private:
    QObject* main_window;
    QObject* action_handler;
};

#endif // PF_ACTIONFACTORY_H
