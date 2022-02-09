#include "Floor.h"
#include "Passenger.h"

Floor::Floor(ECS *ecs, int floorNum) : ecs(ecs), floorNum(floorNum)   {
    up = false;
    down = false;
    informTimer = new QTimer(this);
    informTimer->start(informInterval);
    connect(informTimer,SIGNAL(timeout()),this,SLOT(inform()));

    QTimer::singleShot(QRandomGenerator::global()->generate() % MAX_PASSENGER_WAIT_TIME, this, SLOT(generatePassenger()));
}

Floor::~Floor() {
    delete informTimer;
}

ECS *Floor::getECS()    {return ecs;}
void Floor::setDownButtonState(bool state)    {
    if(!ecs->movingToSafeFloor() && floorNum!=0)    down = state;
}
void Floor::setUpButtonState(bool state)      {
    if(!ecs->movingToSafeFloor() && floorNum!=ecs->getNumFloors()-1)    up = state;
}
bool Floor::getDownButtonState()    {return down;}
bool Floor::getUpButtonState()      {return up;}
int Floor::getFloorNum()    {return floorNum;}
QString Floor::getSymbol()          {
    if(up && down)  return "[^][v]";
    if(up)          return "[^]   ";
    if(down)        return "[v]   ";
    return "     ";

}
void Floor::inform()    {
    if(ecs->movingToSafeFloor())    clear();
    if(!up && !down)  {
        ecs->getFloorRequests()->remove(floorNum);
    }
}

void Floor::generatePassenger() {
    if(ecs->hasRandomEvents())    Passenger::generateRandomRequest(this);
    QTimer::singleShot(QRandomGenerator::global()->generate() % MAX_PASSENGER_WAIT_TIME, this, SLOT(generatePassenger()));
}

bool Floor::isIdle()    {return !(up || down);}
void Floor::clear()     {up = down = false;}
