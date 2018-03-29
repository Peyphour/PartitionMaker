#include "partitionnote.h"

/**
 * This is a basic implementation for a note. This will contains every effects
 * that a note can have (let ring, palm mute, slide, hammer-pull etc ...) and
 * a note descriptor : @see GuitarNote
 */

PartitionNote::PartitionNote(QObject *parent) : QObject(parent) {

}

PartitionNote::~PartitionNote() {
    delete m_noteDescriptor;
}

void PartitionNote::setDescriptor(GuitarNote *desc) {
    this->m_noteDescriptor = desc;
}

void PartitionNote::setTempo(int tempo) {
    this->m_tempo = tempo;
}

int PartitionNote::tempo() {
    return this->m_tempo;
}

void PartitionNote::setPower(double power) {
    this->m_power = power;
}

double PartitionNote::power() {
    return this->m_power;
}

GuitarNote *PartitionNote::descriptor() {
    return this->m_noteDescriptor;
}
