#include "partitionbar.h"
/**
 * This is a bit more difficult, a bar can contains an indeterminate number
 * of pulsation. It will consists into a @see QList of @see PartitionPulsation
 */
PartitionBar::PartitionBar(QObject *parent) : QObject(parent) {

}

PartitionBar::~PartitionBar() {
    delete pulseList;
}

void PartitionBar::addPulse(PartitionPulsation *note) {
    pulseList->push_back(note);
}

