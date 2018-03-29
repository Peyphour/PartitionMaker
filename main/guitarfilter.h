#ifndef GUITARFILTER_H
#define GUITARFILTER_H

#include <QObject>
#include <QList>
#include <math.h>
#include "guitarnote.h"

class GuitarFilter : public QObject
{
    Q_OBJECT
public:
    explicit GuitarFilter(QObject *parent = 0);
    ~GuitarFilter();

    QList<GuitarNote *> *getNoteList();

private:

    void initFrequencies();

    QList<GuitarNote*> *noteList;

signals:

public slots:
};

#endif // GUITARFILTER_H
