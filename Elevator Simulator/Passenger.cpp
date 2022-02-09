#include "Passenger.h"
#include "Elevator.h"

Passenger::Passenger(Floor *floor) : floor(floor) {
    weight = qAbs((QRandomGenerator::global()->generate() % (MAX_PASSENGER_WEIGHT - MIN_PASSENGER_WEIGHT)) + MIN_PASSENGER_WEIGHT);
    destFloor = qAbs(QRandomGenerator::global()->generate() % floor->getECS()->getNumFloors());
}


Floor *Passenger::getFloor()    {return floor;}
int Passenger::getDestFloor()   {return destFloor;}
int Passenger::getWeight()      {return weight;}

void Passenger::generateRandomRequest() {generateRandomRequest(floor);}
void Passenger::generateRandomRequest(Floor* f)  {
    if(f==NULL) return;

    double p = QRandomGenerator::global()->generateDouble();
    if(p < 0.45) {
        f->setUpButtonState(true);
    } else if(p > 0.55) {
        f->setDownButtonState(true);
    } else if(p < 0.5) {
        f->setUpButtonState(true);
        f->setDownButtonState(true);
    } else {
        return;
    }
    f->getECS()->makeRequest(f->getFloorNum());

}
