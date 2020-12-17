# Traffic Light Simulator

This is an implementation of a Mealy finite state machine that simulates the behavior of a traffic light. The traffic lights can be thought of as a system of two traffic lights on two roads going east and north that meet at a junction.
The microcontroller is configured as:
- east facing red light connected to PB5
- east facing yellow light connected to PB4
- east facing green light connected to PB3
- north facing red light connected to PB2
- north facing yellow light connected to PB1
- north facing green light connected to PB0
- north facing car detector connected to PE1 (1=car present)
- east facing car detector connected to PE0 (1=car present)

### Sequence of Operations
- Initialize timer and direction registers
- Specify initial state
- Perform FSM controller
    - Output to traffic lights (depends on the state)
    - Delay (depends on the state)
    - Input from sensors
    - Change states (depends on the inputs and state)

### State Transition Table
| State # | Name | Lights (Port B) | Wait Time (10 ms)| In=0 | In=1 | In=2 | In=3 |
| --------|------|--------|-----------|------|------|------|------|
| 0       | goNorth | 100001   | 30   | goNorth | waitNow | goNorth | waitNow |
| 1       | waitNorth | 100010 | 5    | goEast  | goEast  | goEast  | goEast  |
| 2       | goEast | 001100    | 30   | goEast  | goEast  | waitEast| waitEast|
| 3       | waitEast | 010100  | 5    | goNorth | goNorth | goNorth | goNorth |

### State Transition Graph
![State Transition Graph](stateTransitionGraph.png)
***Note:** Image taken from edEx course website.*
