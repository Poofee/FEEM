#ifndef PF_ACTIONGROUPMANAGER_H
#define PF_ACTIONGROUPMANAGER_H

#include <QObject>
#include <QList>
#include <QMap>

class QActionGroup;
class QAction;

class PF_ActionGroupManager : public QObject
{
    Q_OBJECT
public:
    PF_ActionGroupManager(QObject *parent = nullptr);

    QActionGroup* file;
    QActionGroup* edit;

signals:

public slots:
};

#endif // PF_ACTIONGROUPMANAGER_H
