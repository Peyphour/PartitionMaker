#ifndef GUITARTABBUILDER_H
#define GUITARTABBUILDER_H

#include <QObject>
#include <QList>
#include "main/guitarnote.h"
#include "tab/guitartab.h"
#include "tab/partitionpulsation.h"

class GuitarTabBuilder : public QObject
{
    Q_OBJECT
public:
    explicit GuitarTabBuilder(QObject *parent = 0);
    ~GuitarTabBuilder();
    void setData(QList<QList<PartitionNote *> *> *, qreal notes_interval);
    GuitarTab *build();
private:

    void computePulsations();

    QList<QList<PartitionNote *> *> *m_notes;
    QList<PartitionPulsation *> *m_pulsations;

    qreal m_notes_interval;
signals:
    void progressChanged(int);
public slots:
};

#endif // GUITARTABBUILDER_H
