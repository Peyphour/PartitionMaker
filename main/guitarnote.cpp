#include "guitarnote.h"

GuitarNote::GuitarNote(QObject *parent) : QObject(parent) {

}

GuitarNote::GuitarNote(QString name, QString pos, qreal minFreq, qreal maxFreq, QObject *parent) :
    QObject(parent), m_name(name), m_pos(pos), m_minFreq(minFreq), m_maxFreq(maxFreq) {}

GuitarNote::~GuitarNote() {
}

QString GuitarNote::name() const{
    return m_name;
}

QString GuitarNote::pos() const{
    return m_pos;
}

qreal GuitarNote::maxFreq() const {
    return m_maxFreq;
}

qreal GuitarNote::minFreq() const{
    return m_minFreq;
}

bool GuitarNote::isOctaveOf(GuitarNote *note) {

    if(m_name.at(0) != note->name().at(0)) // if not the same name
        return false;

    if((m_name.at(1) == '#' && note->name().size() == 2) // Example : F3 is not an octave of F#2
            || (note->name().at(1) == '#' && m_name.size() == 2))
        return false;
    // The only remaining possibilities are if the note's names are of same size and same first letter
    // In other words, if note is an octave
    return true;
}

QString GuitarNote::toString() {
    QString toReturn;
    toReturn.append("Note : ");
    toReturn.append(name());
    toReturn.append("<br />Neck position : ");
    toReturn.append(pos());
    toReturn.append("<br />Freq range : ");
    toReturn.append(QString::number(minFreq()));
    toReturn.append("-");
    toReturn.append(QString::number(maxFreq()));

    return toReturn;
}

