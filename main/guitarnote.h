#ifndef GUITARNOTE_H
#define GUITARNOTE_H

#include <QObject>

class GuitarNote : public QObject
{
    Q_OBJECT
public:
    explicit GuitarNote(QObject *parent = 0);
    explicit GuitarNote(QString name, QString pos, qreal minFreq, qreal maxFreq, QObject *parent = 0);
    ~GuitarNote();
    QString name() const;
    QString pos() const;
    qreal minFreq() const;
    qreal maxFreq() const;
    QString toString();
    GuitarNote *getNextOctave();
    bool isOctaveOf(GuitarNote*);
private :
    QString m_name, m_pos;
    qreal m_minFreq, m_maxFreq;
signals:

public slots:
};

#endif // GUITARNOTE_H
