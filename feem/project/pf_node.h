#ifndef PF_NODE_H
#define PF_NODE_H

#include <QObject>

class PF_Node : public QObject
{
    Q_OBJECT
public:
    explicit PF_Node(QObject *parent = nullptr);

signals:

public slots:
};

#endif // PF_NODE_H