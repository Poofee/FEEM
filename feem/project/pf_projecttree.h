#ifndef PF_PROJECTTREE_H
#define PF_PROJECTTREE_H

#include <QObject>

class PF_ProjectTree : public QObject
{
    Q_OBJECT
public:
    explicit PF_ProjectTree(QObject *parent = nullptr);

signals:

public slots:
};

#endif // PF_PROJECTTREE_H