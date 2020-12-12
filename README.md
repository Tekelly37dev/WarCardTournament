# War Card Game Tournament

## Description 
- - -
The WarCardGame.c program demonstrates the use of Unix signals to simulate a War card game tournament by using IPC mechanisms to communicate between two children and a parent process to determine a winner.


## Installation
- - -
> gcc WarCardGame.c

> ./a.out 3

Note: 3 can be any positive integer and is the user determined number of rounds to play entered as a command-line argument.


## Example Output
- - - 
```C
./a.out 3        

This is a War Card Game
Child 1 PID: 67594 
Child 2 PID: 67595 

Let's Start Playing 3 Rounds Of War!

--------  Round: 1 ---------
Child One Draws 3
Child Two Draws 1

Child One Wins!

--------  Round: 2 ---------
Child One Draws 10 
Child Two Draws 8

Child One Wins!

--------  Round: 3 ---------
Child One Draws Diamonds
Child Two Draws Spades

Child One Wins!

Results
Child 1: 3
Child 2: 0
Child One Wins The Tournament!
```