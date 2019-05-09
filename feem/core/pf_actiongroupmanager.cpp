#include "pf_actiongroupmanager.h"

#include <QActionGroup>

PF_ActionGroupManager::PF_ActionGroupManager(QObject *parent)
    : QObject(parent)
    , file(new QActionGroup(this))
    , edit(new QActionGroup(this))
{

}
