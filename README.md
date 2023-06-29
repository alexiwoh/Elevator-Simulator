# Elevator-Simulator
 A simple elevator simulator

The Elevator Simulator folder files can be downloaded (as a zip and extracted) and then opened and run in QtCreator. 

Alternatively the A2 file can be run in a Linux terminal by typing "./A2" for example.
For MacOS users, navigate to the "Elevator Simulator MacOS" folder and type "./A2.app/Contents/MacOS/A2" to run the program.

![image](https://github.com/alexiwoh/Elevator-Simulator/assets/71348624/5b876d44-1e73-4c21-8286-d68a0309b2d9)

    Upon running you will see a text-based grid on the top left, a set of
buttons and input boxes on the bottom and a set of "floor" and "elevator"
buttons on the right. The grid represents the floor of an elevator versus
its number. For example, "F1" refers to Floor 1 and "E2" refers to Elevator 2.
So a "---" symbol at the intersection of "F1" and "E2" represents an idle
Elevator 2 at Floor 1. The Elevator symbols will change according to the
state of the Elevator. Note, "B1" represents "Basement Floor 1"; this happens when
the floor number is negative (This happens when the minFloor text box is negative).
    You can use the Floor and Elevator sliders to change which Floor or Elevator
buttons will be in focus (ie. the currently selected elevator or floor)
on the right hand panel. Below, you can type in values for the
number of elevators, floors, and the minimum floor number for the building, then
press the "Change Dimensions" button to change the dimensions of the building.
The number of floors (and min floor) can range from 2-10 and the number of elevators from 1-10
(these limits were hard coded). The minFloor can range from -10 up to 10 minus the number of
floors. Try playing around with the values and the program should automatically adjust the numbers
to make the building structure wtay within limits.
    On the right panel, the "Floor Buttons" allow you to request an elevator to the currently
selected floor (the one selected using the sliders) in a chosen direction by pressing the "Up"/"Down" buttons.
Underneath, the text shows the status of the currently selected elevator (e.g. "moving"), that is,
the text that would be displayed to users. Underneath that are the floor buttons and special
buttons for the elevator. Click a numbered button to request the elevator go to that floor.
Clicking a special button (e.g. "Help" or "Fire") will trigger an appopriate event corresponding
to that request. For example, "Fire" will simulate a fire emergency on the building and will
send the elevators to the safe floor (which is the floor closest to 0 in this simulation).
    Underneath all this is the weight box that shows the current weight of passengers on the elevator.
When this weight exceeds 1000 (the hard coded limit) the elevator will become "overloaded" and will
have its movement delayed. You can also simulate this by directly pressing the "Overload" button.
    The speed of the simulation is set such that the elevator move time is 1000ms (1 second).
You can speed this up by decreasing the "elevMoveTime" variable in the Elevator.h file. There are
also randomly generated passengers in the simulation. These "passengers" will generate floor requests
and activate elevator buttons, as well as contribute to the elevator weight. In the Floor.h file you
can decrease the MAX_PASSENGER_WAIT_TIME variable to increase the frequency of Passenger requests.
You can turn on/off the random events by pressing the "Random Events" button. It starts in the off state
by default.

Note: when an Elevator moves to a Floor in response to a floor request, Passengers will increase the
weight of the elevator and trigger numbered buttons for the Elevator to go to. This is true whether
Random events are on or off.