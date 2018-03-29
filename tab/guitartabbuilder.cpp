#include "guitartabbuilder.h"

GuitarTabBuilder::GuitarTabBuilder(QObject *parent) : QObject(parent) {

}

GuitarTabBuilder::~GuitarTabBuilder() {
    delete m_notes;
    delete m_pulsations;
}

void GuitarTabBuilder::setData(QList<QList<PartitionNote *> *> *notes, qreal notes_interval) {
    m_notes = notes;
    this->m_notes_interval = notes_interval;
}

void pulseString(PartitionPulsation *p) {
    for(int i = 0; i < p->getList()->size(); i++) {
        qDebug() << p->getList()->at(i)->descriptor()->toString() << p->getList()->at(i)->power();
    }
}

void GuitarTabBuilder::computePulsations() {
    // First of all we need to get the index of the last pulse
    int lastPulse = 0;
    m_pulsations = new QList<PartitionPulsation *>();

    for(int i = 0; i < m_notes->size(); i++) {
        for(int j = 0; j < m_notes->at(i)->size(); j++) {
            if(m_notes->at(i)->at(j)->tempo() > lastPulse) {
                lastPulse = m_notes->at(i)->at(j)->tempo();
            }
        }
    }
    // Now initializing pulseList;
    for(int i = 0; i <= lastPulse; i++) {
        m_pulsations->append(new PartitionPulsation);
    }
    // And now putting each note into the pulsations list
    for(int i = 0; i < m_notes->size(); i++) {
        for(int j = 0; j < m_notes->at(i)->size(); j++) {
            m_pulsations->at(m_notes->at(i)->at(j)->tempo())->addNote(m_notes->at(i)->at(j));
        }
    }
    for(int i = 0; i < m_pulsations->size(); i++) {
        m_pulsations->at(i)->normalize();
        qDebug() << "Pulsation " << i;
        pulseString(m_pulsations->at(i));
        qDebug() << "End pulsation " << i;
    }
}

GuitarTab *GuitarTabBuilder::build() {
    GuitarTab *tab = new GuitarTab;
    computePulsations();
    emit progressChanged(100);
    return tab;
}

