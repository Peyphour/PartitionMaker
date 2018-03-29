#include "partitionpulsation.h"

/**
 * A pulsation can contains a maximum of 6 notes, one for each string
 *
 */

PartitionPulsation::PartitionPulsation(QObject *parent) : QObject(parent) {
    this->notesList = new QList<PartitionNote *>();
}

PartitionPulsation::~PartitionPulsation() {
    delete notesList;
}

QList<PartitionNote *> *PartitionPulsation::getList() {
    return this->notesList;
}

void PartitionPulsation::addNote(PartitionNote *note) {
    this->notesList->push_back(note);
}

PartitionNote *PartitionPulsation::getNextOctave(PartitionNote *note) {
    for(int i = 0; i < notesList->size(); i++) {
        if(notesList->at(i)->descriptor()->isOctaveOf(note->descriptor())) {
            return notesList->at(i);
        }
    }
    return Q_NULLPTR;
}

/**
 * This function will normalize the pulsation :
 * We cannot have two notes at the same moment on the same string
 * To determinate which one is the most significent we check the power
 * on the next octave for each note
 */
void PartitionPulsation::normalize() {
    for(int i = 0; i < notesList->size(); i++) {
        PartitionNote *cur = notesList->at(i);
        for(int j = 0; j < notesList->size(); j++) {
            if(j != i) { // cannot compare the same note
                PartitionNote *toCompare = notesList->at(j);
                int curStringNB = cur->descriptor()->pos().at(2).digitValue();
                int toCompareStringNB = toCompare->descriptor()->pos().at(2).digitValue();
                if(curStringNB == toCompareStringNB) {
                    qDebug() << "same string detected " << cur->descriptor()->name()
                             << " " << toCompare->descriptor()->name();
                    PartitionNote *curOctave = getNextOctave(cur);
                    if(curOctave == Q_NULLPTR) {
                        // Then toCompare is the master note
                        cur = toCompare;
                        notesList->removeAt(j);
                        break;
                    }
                    PartitionNote *toCompareOctave = getNextOctave(toCompare);
                    if(toCompareOctave == Q_NULLPTR) {
                        // Then cur is the master note
                        notesList->removeAt(j);
                        break;
                    }
                    if(curOctave->power() > toCompareOctave->power()) {
                        // then cur is the master note
                        notesList->removeAt(j);
                    } else {
                        // then toCompare is the master note
                        cur = toCompare;
                        notesList->removeAt(j);
                    }
                }
            }
        }
    }
}


