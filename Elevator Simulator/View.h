/*
 * Made by Alexander Iwoh
 * This class is a view in the MVC paradigm that displays the states of the ECS as a Graphical User Interface.
 *
 */
#ifndef VIEW_H
#define VIEW_H
#include <QObject>
#include <QThread>
#include <QLabel>
#include <QRect>
#include <QFrame>
#include <QPainter>
#include <QTimer>
#include <QString>
#include <string>
#include <QDebug>
#include <QPushButton>
#include <QSignalMapper>
#include <QLineEdit>
#include <QSlider>
#include <QList>
#include <QMap>
#include <QHash>
#include <QColor>
#include "mainwindow.h"
#include "ECS.h"
#include "Elevator.h"
#include "Floor.h"

class View : public QWidget {
    Q_OBJECT
public:
    View(MainWindow*,ECS* = NULL);
    ~View();

    // Getters/Setters.
    MainWindow *getMainWindow();
    void setMainWindow(MainWindow*);
    ECS *getModel();
    void setModel(ECS *);

private:
    QList<QLabel*> labels, labelsGUI; // GUI labels.
    MainWindow *mainWindow; // Pointer to Main Window.
    ECS *model; // The ECS to display.
    QTimer *updateTimer; // Timer to draw grid.
    QList<QWidget*> widgets; // Stores GUI widgets.

    const int gridStartX = 25, gridstartY = 25, boxSize = 45, boxSizeGrid = 30, maxLabelLength = 125; // Constants used for GUI element placement.
    const int refreshTime = 200; // Timer interval for updating GUI.
    int curFloor, curElev; // Current floor and elevator number respectively.
    QLabel *curFloorNumLabel, *curElevNumLabel; // GUI labels.

    QSlider *curFloorSlider; // Slider to select GUI floor number.
    QSlider *curElevSlider; // Slider to select GUi elevator number.
    QLineEdit *weightBox, *floorsBox, *elevBox, *minFloorsBox; // GUI textboxes for user input.
    QPushButton *dimensionsButton; // Button to trigger ECS floor/elevator numbers change.
    QList<QPushButton*> *nButtons; // Numbered GUI buttons.
    QPushButton *upButton, *downButton; // Up/down GUI floor buttons.
    QPushButton *randomButton; // Button to toggle on/off random events.
    QHash<QString,QPushButton*> *sButtons; // Special GUI buttons (e.g. "Help" button).
    QLabel *floorLabel, *elevLabel; // GUI labels.

    void toggleButton(QString); // Tells the ECS to alter the Elevators' or Floors' buttons states.

private slots:
    void drawGrid(); // Draws the grid representing the floor/elevator states and movements.
    void update(); // Updates GUI elements.
    void createGUI(); // Create GUI elements.
    void createGUIButtons(); // Create GUI elements.
    void restartNewDimensions(); // Facilitates the ECS's floor/elevator numbers change.

};

#endif // VIEW_H
