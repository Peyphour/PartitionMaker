#ifndef GUITARTAB_H
#define GUITARTAB_H

#include <QObject>
#include <QList>
#include <tab/partitionbar.h>

class GuitarTab : public QObject
{
    Q_OBJECT
public:
    explicit GuitarTab(QObject *parent = 0);
    ~GuitarTab();
    QList<PartitionBar *> *getTab();
    void addBar(PartitionBar *);
    void writeToFile(const QString fileName);


    static const qreal WHOLE_NOTE = 0.25;
    static const qreal HALF_NOTE = 0.5;
    static const qreal QUARTER_NOTE = 1.;
    static const qreal HEIGHTH_NOTE = 2;
    static const qreal SIXTEENTH_NOTE = 4;

signals:

public slots:

private:

    QList<PartitionBar *> *barList;

};

#endif // GUITARTAB_H
