# Number Demonstrations

Points: 60

## Write some demonstrations in C

## IMPORTANT: You can ignore warnings for this assignment. You will get some due to the nature of the code we're writing, so please include them in your README.txt, but you don't have to fix them for this one.

For this assignment, you will be writing several little bits of code in C to demonstrate how some types of numbers are dealt with. The results of some of these exercises will be just as you expect them to be, and some will be a little odd.

Please use the normal Homework Guidelines for your submission, with the following additions:

Create a typescript for this assignment.

- If you write this as multiple programs, please include a line for each gcc command line command or makefile command that you use to compile in your README.txt
- If you write this as multiple programs, your typescript should show you compiling and running each one. You can use makefile commands when making the typescript.
- In your README.txt, add a Results section where for each problem you indicate the problem #, the expected mathematical result, and the result you got.
- Include a makefile with 3 targets. If you write this as multiple programs include a makefile for each program in separate directories.
  - all: compile your program
  - run: run your program
  - clean: remove object files and executable from the directory
- Make sure that your makefile compiles your program using gcc -std=c99 -Wall
- Do not include object files or executables in your submission.
- Clearly print the problem number and result for each problem in your program.
- Do not use any gcc flags to suppress errors or warnings

### Before you submit:

1. Make sure that all your project files are in a folder named: FirstNameLastName_assignment_1
2. Then put the project folder into a zip archive.
   - Be sure to name your zip archive according to the guidelines in "Homework Guidelines"
3. When I unzip your submission I should get a single folder containing all your project files.
   - I have attached an example submission to help

README should be something like this:

Date: 01/19/2020
Class: CS5541
Assignment: Number_Demonstrations
Author(s): Lin Cheng

gcc -std=c99 -Wall -o test test.c

Warnings: I had a couple of warnings but I was supposed to!

References: I did this on my own with no research

## Results:

### Problem 1:

Expected: 15000
Result: 523408275492

### Problem 2:

Expected: 500
Result: 500
etc...
Problem 1:
Declare a variable of type float
Assign it the value 2.5
Print it to ten decimal places

### Problem 2:

Declare a variable of type float
Assign it the value -1.0/10.0 (exactly as shown here)
Print it to ten decimal places

### Problem 3:

Declare a variable of type double
Assign it the value 1/3 (exactly as shown here)
Print it
Assign it the value 1.0/3.0 (exactly as shown here)
Print it

### Problem 4:

Declare a variable of type double
Assign it the value 9999999.3399999999 (exactly as shown here)
Print it
Cast it to a float
Print the float

### Problem 5:

Declare a variable of type int
Assign it the value 30000*30000 (exactly as shown here)
Print it
Assign it the value 40000*40000 (exactly as shown here)
Print it
Assign it the value 50000*50000 (exactly as shown here)
Print it
Assign it the value 60000*60000 (exactly as shown here)
Print it
Assign it the value 70000\*70000 (exactly as shown here)
Print it

### Problem 6:

Use floats for this problem
Assign the value 1e20 to a float and print it
Assign the value (1e20 + 3500000000) to a float and print it
Assign the value (1e20 + (3500000000 \* 1000000000)) to a float and print it
Declare a variable of type float
Assign it the value of 1e20
Use a loop to add 3500000000 to that variable 1000000000 times.
Print the result after the loop
Note: Don't just multiply then add for that last part above. Use a loop to add 3500000000 at each iteration.

### Grading:

I will be grading based on the following criteria.

Each problem is worth 2 points (12 total)
readme = 3 points
Correct gcc flags= 3 points
makefile = 9 points
Compiles and runs without errors = 33 points
