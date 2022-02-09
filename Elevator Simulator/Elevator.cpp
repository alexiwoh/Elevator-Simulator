#include "Elevator.h"
#include "Passenger.h"


Elevator::Elevator(ECS *ecs, int elevatorNum, int curFloor)   {
    this->ecs = ecs;
    this->elevNum = elevatorNum;
    int n = ecs->getNumFloors();
    curFloor = (curFloor > n-1) ? n-1 : curFloor;
    curWeight = lastAddedWeight = 0;
    direction = 0;
    waitTime = 0; idleTime = 0; waitTimeElapsed = 0;
    status = "Idle";
    moveTimer = new QTimer(this);
    moveTimer->start(elevMoveTime);
    connect(moveTimer,SIGNAL(timeout()),this,SLOT(move()));

    this->curFloor = curFloor;
    nButtons = new QList<bool>();
    for(int i=0; i<ecs->getNumFloors(); ++i)    { nButtons->append(false); }
    sButtons = new QHash<QString,bool>();
    sButtons->insert("help",false);
    sButtons->insert("doorOpen",false);
    sButtons->insert("doorClose",false);
    sButtons->insert("fire",false);
    sButtons->insert("overloaded",false);
    sButtons->insert("powerOut",false);
    sButtons->insert("doorObstacle",false);

    for(int i=0; i<ecs->getNumFloors(); ++i)    {
        passengers.append(QList<int>());
    }
}

Elevator::~Elevator()   {
    delete nButtons;
    delete sButtons;
    delete moveTimer;
}

void Elevator::delayMoveTime(int delayTime) {
    waitTime = qMin(delayTime + waitTime, maxWait);
}
void Elevator::setMoveTime(int moveTime)    {
    waitTime = qMax(qMin(maxWait,moveTime),0);
}

bool Elevator::addPassenger(Passenger& p, int floorNum)   {
    if(sButtons->value("fire") || sButtons->value("powerOut"))  return false;
    if(curFloor != floorNum)    return false;
    int wt = p.getWeight();

    if(curWeight + wt > MAX_WEIGHT)  {
        lastAddedWeight = wt;
        toggleButton("overloaded",true);
        return false;
    }
    curWeight += wt;
    passengers[p.getDestFloor()].push_back(wt);
    toggleButton(QString::number(floorNum),true);
    return true;
}
void Elevator::loadPassengers(int floorNum) {
    int pNum = QRandomGenerator::global()->generate() % qMax(2, ecs->getNumFloors());

    for(int i=0; i<pNum; ++i)   {
        Passenger p((*ecs->getFloors())[floorNum]);
        if(! addPassenger(p,floorNum))   break;
    }

    if(!ecs->hasRandomEvents()) return;
    if(pNum > 0)    {
        double p = QRandomGenerator::global()->generateDouble();
        if(p <= 0.010)          {

            ecs->toggleButton("fire",true);
        }
        else if(p <= 0.020)     {

            ecs->toggleButton("powerOut",true);
        }
        else if(p <= 0.030)     toggleButton("help",true);
        else if(p >= 0.95)      toggleButton("doorOpen",true);
        else if(p >= 0.90)      toggleButton("doorClose",true);
        else if(p >= 0.85)      toggleButton("doorObstacle",true);
    }
}
void Elevator::unloadPassengers(int floorNum, bool unloadAll)   {
    if(floorNum < 0 || floorNum >= ecs->getNumFloors())    return;
    if(unloadAll)   {
        for(int i=0; i<ecs->getNumFloors(); ++i)   passengers[i].clear();
        curWeight = lastAddedWeight = 0;
    }

    int sum = 0;
    for(int i=0; i<passengers.at(floorNum).size(); ++i) {
            sum += passengers.at(floorNum).at(i);
    }
    passengers[floorNum].clear();
    curWeight = qMax(0, qMin(MAX_WEIGHT, curWeight - sum));
    lastAddedWeight = 0;
}

void Elevator::move()    {
    ecs->newFloor(this);
    if(sButtons->value("overload")) return;

    waitTime = qMax(0,waitTime - elevMoveTime);
    if (waitTime > 0)   {
        waitTimeElapsed += elevMoveTime;
        status = "Waiting";
        return;
    }


    bool moveToSafeFloor = sButtons->value("fire") || sButtons->value("powerOut");

    if(sButtons->value("help")) {
        if(moveToSafeFloor) sButtons->insert("help",false);
        else                clearButtons("");
        unloadPassengers(curFloor,true);
        if(waitTimeElapsed >= elevMoveTime*5)    qInfo() << "911 Contacted for Elevator" << elevNum << "."; // Debug.
    }
    waitTimeElapsed = 0;
    sButtons->insert("doorOpen",false);
    sButtons->insert("doorClose",false);
    sButtons->insert("overloaded",false);
    sButtons->insert("doorObstacle",false);
    lastAddedWeight = 0;

    if(moveToSafeFloor) {
        int safeFloor = ecs->getSafeFloor() - ecs->getMinFloor();
        if(curFloor == safeFloor) {
            direction = 0;
            sButtons->insert("fire",false);
            sButtons->insert("powerOut",false);
            unloadPassengers(curFloor, true);
            waitTime += elevMoveTime * 10;
        } else if(curFloor < safeFloor) {
            direction = 1;
            curFloor += 1;
        } else {
            direction = -1;
            curFloor -= 1;
        }
        return;
    }

    if(isIdle())    {
        waitTime = 0;
        direction = 0;
        status = "Idle";
        idleTime++;
        if(idleTime > elevMoveTime * 3)   {
            unloadPassengers(curFloor,true);
            idleTime = 0;
        }

        for(int i=0; i<passengers.size(); ++i)    {
            if(!passengers.at(i).isEmpty())    {
                toggleButton(QString::number(i),true);
            }
        }

        return;
    }
    idleTime = 0;

    status = "Moving";
    bool goUp = false, goDown = false;
    if(curFloor <= ecs->getNumFloors()-1)    {
        for(int i=curFloor+1; i<=ecs->getNumFloors()-1; ++i) {
            if(nButtons->at(i)) {
                goUp = true;
                break;
            }
        }
    }
    if(curFloor >= 0) {
        for(int i=0; i<=curFloor-1; ++i) {
            if(nButtons->at(i)) {
                goDown = true;
                break;
            }
        }
    }
    if(direction == 0)  {
        if(goUp)    direction = 1;
        if(goDown)  direction = -1;

    }
    if(direction > 0)    {
        if(goUp)    curFloor += 1;
        else if(goDown)       direction = -1;
    }
    if(direction < 0)  {
        if(goDown)  curFloor -= 1;
        else if(goUp)   direction = 1;
    }
    ecs->newFloor(this);
}

bool Elevator::isIdle(bool numberedButtonsOnly) {
    if(waitTime > 0)    return false;

    for(int i=0; i<nButtons->size(); ++i)   {
        if(nButtons->at(i)) return false;
    }
    if(numberedButtonsOnly) return true;

    for(QHash<QString,bool>::iterator i=sButtons->begin(); i != sButtons->end(); ++i) {
        if(i.value())   return false;
    }

    return true;
}


QString Elevator::getSymbol()   {
    if(sButtons->value("fire")) return "Fire";
    if(sButtons->value("powerOut")) return " X ";
    if(sButtons->value("help")) return "Help";
    if(sButtons->value("doorOpen") || sButtons->value("doorClose") ||
            sButtons->value("doorObstacle")) return "Door";
    if(status.toLower() == "waiting")   return "Wait";
    if(direction > 0)   return "^^^";
    if(direction == 0)  return "---";
    return "vvv";
}

QString Elevator::getStatus()   {
    QString s = status.toLower();
    if(s == "waiting") {
        if(sButtons->value("fire"))    return "Fire! Disembark on safe floor!";
        if(sButtons->value("powerOut"))    return "Power out! Disembark on safe floor!";
        if(sButtons->value("help"))    {
            if(waitTimeElapsed >= elevMoveTime*5)  return "Calling 911!";
            return "Recieving help!";
        }
        if(sButtons->value("overloaded"))    return "Overloaded!";
        if(sButtons->value("doorObstacle"))    return "Door Obstacle! Please clear doors!";
        if(sButtons->value("doorClose"))    return "Closing Door!";
        if(sButtons->value("doorOpen"))    {
            return "Door Open!";
        }

        return "Waiting (Bell rings! Door Open!)";
    }
    else
        return status;

}

int Elevator::getCurFloor() {return curFloor;}
int Elevator::getCurWeight() {return curWeight + lastAddedWeight;}
void Elevator::setCurWeight(int weight) {
    curWeight = (weight <= MAX_WEIGHT) ? weight : MAX_WEIGHT;
    curWeight = (weight >= 0) ? weight : 0;
}
int Elevator::getMaxWeight()    {return MAX_WEIGHT;}
int Elevator::getElevNumber()   {return elevNum;}
char Elevator::getDirection() {return direction;}
void Elevator::setDirection(int dir) {
    if(dir==0)  {
        direction = 0;
    }
    else if(dir<0)   {
        direction = -1;
    } else {
        direction = 1;
    }
}
int Elevator::getElevMoveTime() {return elevMoveTime;}
bool Elevator::atFloor(int floorNum)    {return curFloor == floorNum;}

QList<bool> *Elevator::getNButtons()    {return nButtons;}
QHash<QString,bool> *Elevator::getSButtons()    {return sButtons;}

void Elevator::toggleButton(QString buttonName, bool state)   {
    bool ok;
    int num = buttonName.toInt(&ok);
    if(ok == true && num >= 0 && num < nButtons->size())  {
        (*nButtons)[num] = state;
        return;
    }

    if(buttonName=="doorOpen" || buttonName=="doorObstacle")  {
        if(status != "Moving")  {
            delayMoveTime(elevMoveTime*5);
            qInfo() << ((buttonName=="doorOpen") ? "Door Open Pressed" : "Door Obstacle")
                << " on Elevator" << elevNum << "."; // Debug.
        }
    }
    if(buttonName=="doorClose" || buttonName=="fire" || buttonName=="powerOut")  {
        if(status != "Moving")  {
            setMoveTime(elevMoveTime*2);
            qInfo() << ((buttonName=="doorClose") ? "Door Closed" :
                      ((buttonName=="fire") ? "Fire" : "Power Out"))
                           << " on Elevator" << elevNum << "!"; // Debug.
        }
    }
    if(buttonName=="overloaded")  {
        delayMoveTime(elevMoveTime*5);
        qInfo() << "Overloaded on Elevator" << elevNum << ". Please clear!"; // Debug.
    }
    if(buttonName=="help")  {
        delayMoveTime(elevMoveTime*5);
        qInfo() << "Help needed on Elevator" << elevNum; // Debug.
        if(waitTimeElapsed >= elevMoveTime*5)    qInfo() << "Calling 911 for Elevator" << elevNum << "."; // Debug.
    }

    if(sButtons->contains(buttonName))  {
        sButtons->insert(buttonName,state);
        return;
    }
}

void Elevator::clearButtons(QString toggle) {

    for(int i=0; i<nButtons->size(); ++i)    {(*nButtons)[i] = false;}
    sButtons->insert("help",false);
    sButtons->insert("doorOpen",false);
    sButtons->insert("doorClose",false);
    sButtons->insert("fire",false);
    sButtons->insert("overloaded",false);
    sButtons->insert("powerOut",false);
    sButtons->insert("doorObstacle",false);

    direction = 0;
    waitTime = 0;
    toggleButton(toggle,true);
}
