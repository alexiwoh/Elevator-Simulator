/*
 * Made by Alexander Iwoh
 * Represents an elevator in the simulation.
 */
#ifndef ELEVATOR_H
#define ELEVATOR_H
#define MAX_WEIGHT 1000 // Max weight capacity for elevator in kilograms.
#include "ECS.h"
class Passenger;

class Elevator : public QObject
{
        Q_OBJECT

public:
    Elevator(ECS*, int, int=0);
    ~Elevator();

    //Getters/Setters.
    QTimer *getTimer();
    int getCurFloor();
    int getElevNumber();
    QString getSymbol();
    QList<bool> *getNButtons();
    QHash<QString,bool> *getSButtons();
    int getMaxWeight();
    int getCurWeight();
    void setCurWeight(int=0);
    QString getStatus();
    char getDirection();
    void setDirection(int=0);
    int getElevMoveTime();
    bool atFloor(int=0); // Returns whether this elevator is at the specified floor.
    bool isIdle(bool=false); // Returns whether this elevator has no activated buttons.
    static const int elevMoveTime = 1000; // Time it takes elevator to move one floor in msec.
    void setMoveTime(int);

    void toggleButton(QString,bool=true);
    void delayMoveTime(int); // Extend the moveTimer's remaining time.
    bool addPassenger(Passenger&,int); // Adds a Passenger objects weight to the passengers list.
    void loadPassengers(int); // Loads passendgers onto the elevator.
    void unloadPassengers(int,bool=false); // Unloads passengers that requested a specified floor. Unloads all passengers if second argument is true.
    void clearButtons(QString=""); // Clears (deactivates) all buttons and turns on the button specified by the argument.

private:
    QTimer *moveTimer;
    int curFloor; // Elevator's current floor number.
    int elevNum; //Elevator's id number.
    int curWeight, lastAddedWeight; // Elevator weight in kg.
    char direction; // Current direction of movement.
    int waitTime, waitTimeElapsed; // Time for elevator to wait, and time that elevator has waited.
    int idleTime; // Time elevator has remained idle.
    int maxWait = elevMoveTime * 10; // Maximum waiting time for a stationary elevator.
    QString status; // Status message of thie Elevator.
    ECS *ecs; // ECS this Elevator object belongs to.

    QList<bool> *nButtons; // List of numbered button states.
    QHash<QString,bool> *sButtons; // Non-numbered button states.
    QList<QList<int>> passengers; // Stores passenger weights for each floor.


public slots:
    void move(); // Changes floorNum by +/- 1.

};

#endif // ELEVATOR_H
