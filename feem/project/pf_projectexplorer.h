#ifndef PF_PROJECTEXPLORER_H
#define PF_PROJECTEXPLORER_H

#include <QObject>

class PF_ProjectExplorer : public QObject
{
    Q_OBJECT
public:
    explicit PF_ProjectExplorer(QObject *parent = nullptr);

signals:

public slots:
};

#endif // PF_PROJECTEXPLORER_H