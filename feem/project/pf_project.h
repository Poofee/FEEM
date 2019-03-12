#ifndef PF_PROJECT_H
#define PF_PROJECT_H

#include <QObject>

class PF_Project : public QObject
{
    Q_OBJECT
public:
    explicit PF_Project(QObject *parent = nullptr);

signals:

public slots:
};

#endif // PF_PROJECT_H