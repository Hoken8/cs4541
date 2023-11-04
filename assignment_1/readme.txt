Date: 09/25/2023
Class: CS4541
Assignment: Number_Demonstrations
Author(s): Daniel Wilkins

gcc -std=c99 -Wall -o test test.c

Run: ./number_demonstration

Warnings: I had a couple of warnings but I was supposed to!
```
number_demonstration.c:77:26: warning: integer overflow in expression of type ‘int’ results in ‘-1794967296’ [-Woverflow]
   77 |    problem_05_var = 50000*50000;
      |                          ^
number_demonstration.c:83:26: warning: integer overflow in expression of type ‘int’ results in ‘-694967296’ [-Woverflow]
   83 |    problem_05_var = 60000*60000;
      |                          ^
number_demonstration.c:89:26: warning: integer overflow in expression of type ‘int’ results in ‘605032704’ [-Woverflow]
   89 |    problem_05_var = 70000*70000;
      |
```
All errors due to overflow.        

References: I did this on my own with no research