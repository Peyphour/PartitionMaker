#include "guitartab.h"
/**
 * This class is an implementation for a basic tablature,
 * It consists into a simple @see QList of @see PartitionBar
 */

GuitarTab::GuitarTab(QObject *parent) : QObject(parent) {
    barList = new QList<PartitionBar *>();
}

GuitarTab::~GuitarTab() {
    delete barList;
}

QList<PartitionBar *> *GuitarTab::getTab() {
    return barList;
}

void GuitarTab::addBar(PartitionBar *bar) {
    barList->push_back(bar);
}

void GuitarTab::writeToFile(const QString fileName) {
    Q_UNUSED(fileName);
}
