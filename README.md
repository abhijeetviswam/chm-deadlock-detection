# Chandy-Misra-Haas Deadlock Detection (OR Model)

## Source File
cmh_or_model.cpp

---
## Build command
    g++ -g -pthread -o cmh-or-model cmh_or_model.cpp
_Compiled in Linux using g++ (Ubuntu 9.3.0-10ubuntu2) 9.3.0_

---
## Execution Example
```
user@linux:~/projects/CMH$ ./cmh-or-model
Enter the number of processes : 6
Input the wait graph :

Is process P0 waiting for P1 : y
Is process P0 waiting for P2 : n
Is process P0 waiting for P3 : n
Is process P0 waiting for P4 : n
Is process P0 waiting for P5 : n
Is process P1 waiting for P0 : n
Is process P1 waiting for P2 : y
Is process P1 waiting for P3 : n
Is process P1 waiting for P4 : n
Is process P1 waiting for P5 : y
Is process P2 waiting for P0 : n
Is process P2 waiting for P1 : n
Is process P2 waiting for P3 : y
Is process P2 waiting for P4 : y
Is process P2 waiting for P5 : n
Is process P3 waiting for P0 : n
Is process P3 waiting for P1 : n
Is process P3 waiting for P2 : n
Is process P3 waiting for P4 : y
Is process P3 waiting for P5 : n
Is process P4 waiting for P0 : n
Is process P4 waiting for P1 : n
Is process P4 waiting for P2 : n
Is process P4 waiting for P3 : n
Is process P4 waiting for P5 : y
Is process P5 waiting for P0 : y
Is process P5 waiting for P1 : n
Is process P5 waiting for P2 : n
Is process P5 waiting for P3 : n
Is process P5 waiting for P4 : n


Input deadlock detection trigger process : P0

====================================================

Wait-for Graph Process Dependencies :
P0      -> P1
P1      -> P2 P5
P2      -> P3 P4
P3      -> P4
P4      -> P5
P5      -> P0


Starting Deadlock Detection...

QUERY : (P0,P0,P1)
QUERY : (P0,P1,P2)
QUERY : (P0,P1,P5)
QUERY : (P0,P5,P0)
QUERY : (P0,P2,P4)
QUERY : (P0,P2,P3)
REPLY : (P0,P0,P5) - Remaining replies at P5:0
REPLY : (P0,P5,P1) - Remaining replies at P1:1
QUERY : (P0,P4,P5)
REPLY : (P0,P5,P4) - Remaining replies at P4:0
REPLY : (P0,P4,P2) - Remaining replies at P2:1
QUERY : (P0,P3,P4)
REPLY : (P0,P4,P3) - Remaining replies at P3:0
REPLY : (P0,P3,P2) - Remaining replies at P2:0
REPLY : (P0,P2,P1) - Remaining replies at P1:0
REPLY : (P0,P1,P0) - Remaining replies at P0:0
*************************
*   DEADLOCK DETECTED   *
*************************
```