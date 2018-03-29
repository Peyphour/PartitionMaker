#ifndef PARTITIONBAR_H
#define PARTITIONBAR_H

#include <QObject>
#include <QList>
#include "tab/partitionpulsation.h"

class PartitionBar : public QObject
{
    Q_OBJECT
public:
    explicit PartitionBar(QObject *parent = 0);
    ~PartitionBar();
    QList <PartitionPulsation *> *getBar();
    void addPulse(PartitionPulsation *);
signals:

public slots:

private:
    QList<PartitionPulsation *> *pulseList;
};

#endif // PARTITIONBAR_H
