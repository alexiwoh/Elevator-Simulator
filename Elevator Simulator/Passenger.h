/*
 * Made by Alexander Iwoh
 * Represents a passenger in the simulation.
 */
#ifndef PASSENGER_H
#define PASSENGER_H
#define MIN_PASSENGER_WEIGHT 20 // Minimum passenger weight in kilograms.
#define MAX_PASSENGER_WEIGHT 100 // Maximum passenger weight in kilograms.
#include "Floor.h"
#include "Elevator.h"
class Elevator;


class Passenger : public QObject {

        Q_OBJECT

public:
    Passenger(Floor*); // Creates passenger knowing the Floor.

    // Getters.
    int getWeight();
    int getDestFloor();
    Floor *getFloor();

    static void generateRandomRequest(Floor*); // Toggles one of the Floor buttons at random.
    void generateRandomRequest(); // Toggles one of the Floor buttons at random.
private:
    int weight; // Passenger's weight.
    int destFloor; // Floor passenger is going to.
    Floor *floor; // Floor this Passenger is at.
};



#endif // PASSENGER_H
