/*
 * Made by Alexander Iwoh
 * Represents a floor in the simulation.
 */
#ifndef FLOOR_H
#define FLOOR_H
#define MAX_PASSENGER_WAIT_TIME 90000 // Controls how often a passenger requests an elevator at this floor. A request will be made at most every MAX_PASSENGER_WAIT_TIME milliseconds.
class Passenger;
#include "ECS.h"

class Floor : public QObject {

        Q_OBJECT

public:
    Floor(ECS*, int);
    ~Floor();

    // Getters/Setters.
    bool getUpButtonState(), getDownButtonState(), isIdle(); // Get button states and floor activity state (isIdle()).
    QString getSymbol(); // Get drawing symbol for GUI representing this floor.
    ECS *getECS(); // Get the ECS.
    int getFloorNum(); // Get this floor's number.
    void setUpButtonState(bool), setDownButtonState(bool); // Sets button states.

    void clear(); // Clear(deactivate) buttons.

public slots:
    void inform(); // Informs ECS to modify requests list.
    void generatePassenger(); // Generates a Passenger object.

private:
    ECS *ecs;
    int floorNum; // This floor's number.
    bool up, down; // Floor button states.
    QTimer *informTimer; // Informs the ECS of requests on a time interval.
    const int informInterval = 100; // Interval in msec for the timer.

};

#endif // FLOOR_H
