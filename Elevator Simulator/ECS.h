/*
 * Made by Alexander Iwoh
 * This class controls the Elevator and Floor objects.
 * It simulates an Elevator Control System (ECS).
 */

#ifndef ECS_H
#define ECS_H
#include <QObject>
#include <QThread>
#include <QDateTime>
#include <QLabel>
#include <QRect>
#include <QTimer>
#include <QString>
#include <string>
#include <QDebug>
#include <QPushButton>
#include <QList>
#include <QMap>
#include <QHash>
#include <QSet>
#include <QtMath>
#include <QException>
#include <QRandomGenerator>
#define MAX_FLOORS 10 // Max number of floors in building.
#define MAX_ELEVATORS 10 //Max number of elevators in building.
#define MIN_FLOOR -10 // Minimum possible floor number in building.
#define MAX_FLOOR 20 // Maximum possible floor number in building.
class Elevator;
class Floor;
class Passenger;

class ECS : public QObject
{
    Q_OBJECT

public:

    ECS(int=3, int=3, int=0, bool=false);
    ~ECS();

    // Getters/Setters.
    int getNumElevators(), getMaxElevators();
    int getNumFloors(), getMaxFloors();
    int getMinFloor(), getCurFloor(), getCurElevator();
    void setMinFloor(int=0), setCurFloor(int=0),setCurElevator(int=0);
    QList<Elevator*> *getElevators();
    QList<Floor*> *getFloors();
    QSet<int> *getFloorRequests();
    int getSafeFloor();
    bool hasRandomEvents();
    void toggleRandomEvents(); // Toggles the random events on/off.
    void setSafeFloor(int);
    bool setDimensions(int=3,int=3); // Sets number of elevators and floors, respectively.
    bool movingToSafeFloor(); // Returns whether the elevators are moving to a safe floor (ie. emergency mode).

    bool addPassenger(Passenger&, int); // Tells the elevator with specified number to add a Passenger.
    void newFloor(int,int); // Alerts ECS that specific elevator has reached new floor.
    void newFloor(Elevator*); // Helper function.
    void toggleButton(QString,int); // Tells a specified elevator to toggle one of its floor or special buttons.
    void toggleButton(int,int); // Same as above except used only for numbered elevator buttons.
    void makeRequest(int=0); // Stores new floor request


private:
    int m; // Number of elevators.
    int n; // Number of floors.
    int curElev, curFloor, minFloor, safeFloor; // Currently selected elevator and floor, minimum floor in building, and safe floor in the case of emergencies, respectively.
    QTimer *requestTimer; // Timer.
    int requestInterval; // Interval for timer.
    QList<Elevator*> *elevators; // List of Elevator object pointers.
    QList<Floor*> *floors; // List of Floor object pointers.
    QSet<int> *floorRequests; // Stores requests for each floor (int).
    bool generateRandomEvents; // Tracks whether random events are allowed for the simulation.

public slots:
    void makeDecision(); // Checks for idle elevators and assigns them to a floor.
};


#endif // ECS_H
