#ifndef PARTITIONPULSATION_H
#define PARTITIONPULSATION_H

#include <QObject>
#include <QList>
#include <QDebug>
#include <tab/partitionnote.h>

class PartitionPulsation : public QObject
{
    Q_OBJECT
public:
    explicit PartitionPulsation(QObject *parent = 0);
    ~PartitionPulsation();
    QList<PartitionNote *> *getList();
    void addNote(PartitionNote *);
    PartitionNote *getNextOctave(PartitionNote*);
    void normalize();
private:
    QList<PartitionNote *> *notesList;
signals:

public slots:
};

#endif // PARTITIONPULSATION_H
