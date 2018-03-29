#include "guitarfilter.h"

GuitarFilter::GuitarFilter(QObject *parent) : QObject(parent) {
    initFrequencies();
}

GuitarFilter::~GuitarFilter() {
    delete noteList;
}

QList<GuitarNote*> *GuitarFilter::getNoteList() {
    return this->noteList;
}


void GuitarFilter::initFrequencies() {

    this->noteList = new QList<GuitarNote*>;

    GuitarNote *tmp;

    tmp = new GuitarNote("E2", "0.6", 81.0, 83.0);
    noteList->push_back(tmp);

    tmp = new GuitarNote("F2", "1.6", 87.0, 88.0);
    noteList->push_back(tmp);

    tmp = new GuitarNote("F#2", "2.6", 91.0, 94.0);
    noteList->push_back(tmp);

    tmp = new GuitarNote("G2", "3.6", 97.0, 99.0);
    noteList->push_back(tmp);

    tmp = new GuitarNote("G#2", "4.6", 102.0, 105.0);
    noteList->push_back(tmp);

    tmp = new GuitarNote("A2", "0.5", 108.0, 110.0);
    noteList->push_back(tmp);

    tmp = new GuitarNote("A#2", "1.5", 115.0, 118.0);
    noteList->push_back(tmp);

    tmp = new GuitarNote("B2", "2.5", 121.0, 125.0);
    noteList->push_back(tmp);

    tmp = new GuitarNote("C3", "3.5", 128.0, 133.0);
    noteList->push_back(tmp);

    tmp = new GuitarNote("C#3", "4.5", 137.0, 141.0);
    noteList->push_back(tmp);

    tmp = new GuitarNote("D3", "0.4", 145.0, 150.0);
    noteList->push_back(tmp);

    tmp = new GuitarNote("D#3", "1.4", 152.0, 160.0);
    noteList->push_back(tmp);

    tmp = new GuitarNote("E3", "2.4", 162.0, 170.0);
    noteList->push_back(tmp);

    tmp = new GuitarNote("F3", "3.4", 172.0, 180.0);
    noteList->push_back(tmp);

    tmp = new GuitarNote("F#3", "4.4", 182.0, 190.0);
    noteList->push_back(tmp);

    tmp = new GuitarNote("G3", "0.3", 194.0, 198.0);
    noteList->push_back(tmp);

    tmp = new GuitarNote("G#3", "1.3", 205.0, 212.0);
    noteList->push_back(tmp);

    tmp = new GuitarNote("A3", "2.3", 217.0, 225.0);
    noteList->push_back(tmp);

    tmp = new GuitarNote("A#3", "3.3", 230.0, 240.0);
    noteList->push_back(tmp);

    tmp = new GuitarNote("B3", "0.2", 244.0, 255.0);
    noteList->push_back(tmp);

    tmp = new GuitarNote("C4", "1.2", 259.0, 270.0);
    noteList->push_back(tmp);

    tmp = new GuitarNote("C#4", "2.2", 275.0, 287.0);
    noteList->push_back(tmp);

    tmp = new GuitarNote("D4", "3.2", 290.0, 305.0);
    noteList->push_back(tmp);

    tmp = new GuitarNote("D#4", "4.2", 308.0, 320.0);
    noteList->push_back(tmp);

    tmp = new GuitarNote("E4", "0.1", 325.0, 340.0);
    noteList->push_back(tmp);

    tmp = new GuitarNote("F4", "1.1", 345.0, 360.0);
    noteList->push_back(tmp);

    tmp = new GuitarNote("F#4", "2.1", 365.0, 382.0);
    noteList->push_back(tmp);

    tmp = new GuitarNote("G4", "3.1", 387.0, 400.0);
    noteList->push_back(tmp);

}


