#ifndef PARTITIONNOTE_H
#define PARTITIONNOTE_H

#include <QObject>
#include "main/guitarnote.h"

class PartitionNote : public QObject
{
    Q_OBJECT
public:
    explicit PartitionNote(QObject *parent = 0);
    ~PartitionNote();
    void setDescriptor(GuitarNote *);
    void setTempo(int);
    int tempo();
    GuitarNote *descriptor();
    void setPower(double);
    double power();
signals:

public slots:

private:
    GuitarNote *m_noteDescriptor;
    int m_tempo;
    double m_power;
};

#endif // PARTITIONNOTE_H
