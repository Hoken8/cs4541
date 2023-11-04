#include <stdio.h>

int main() {
   printf("Results:\n\n");

   // Problem 1:
   printf("Problem 1:\n");
   // - Declare a variable of type float
   // - Assign it the value 2.5
   float problem_01_var = 2.5;
   // - Print it to ten decimal places
   printf("x = 2.5\n"); 
   printf("Expected: 2.5000000000 \n");
   printf("  Result: %.10f\n", problem_01_var);
   printf("\n");

   // Problem 2:
   printf("Problem 2:\n");
   // - Declare a variable of type float
   // - Assign it the value -1.0/10.0 (exactly as shown here)
   float problem_02_var = -1.0/10.0;
   // - Print it to ten decimal places
   printf("x = -1.0/10.0\n"); 
   printf("Expected: -0.1000000000 \n");
   printf("  Result: %.10f\n\n", problem_02_var);
   printf("\n");

   // Problem 3:
   printf("Problem 3:\n");
   // - Declare a variable of type double
   // - Assign it the value 1/3 (exactly as shown here)
   double problem_03_var = 1/3;
   // - Print it
   printf("x = 1/3\n"); 
   printf("Expected: 0.333333 \n");
   printf("  Result: %g\n\n", problem_03_var);
   // - Assign it the value 1.0/3.0 (exactly as shown here)
   problem_03_var = 1.0/3.0;
   // Print it
   printf("x = 1.0/3.0\n"); 
   printf("Expected: 0.333333 \n");
   printf("  Result: %g\n\n", problem_03_var);
   printf("\n");

   // Problem 4:
   printf("Problem 4:\n");
   // Declare a variable of type double
   // Assign it the value 9999999.3399999999 (exactly as shown here)
   double problem_04_var = 9999999.3399999999;
   // Print it
   printf("x = 9999999.3399999999\n");
   printf("Expected: 9999999.3399999999 \n");
   printf("  Result: %g\n\n", problem_04_var);
   // Cast it to a float
   // Print the float
   printf("x = (float)9999999.3399999999\n");
   printf("Expected: 9999999.3399999999 \n");
   printf("  Result: %f\n", (float)problem_04_var);
   printf("\n\n");

   // Problem 5:
   printf("Problem 5:\n");
   // Declare a variable of type int
   // Assign it the value 30000*30000 (exactly as shown here)
   int problem_05_var = 30000*30000;
   // Print it
   printf("x = 30000*30000\n");
   printf("Expected: 900000000 \n");
   printf("  Result: %d\n\n", problem_05_var);
   // Assign it the value 40000*40000 (exactly as shown here)
   problem_05_var = 40000*40000;
   // Print it
   printf("x = 40000*40000\n");
   printf("Expected: 1600000000 \n");
   printf("  Result: %d\n\n", problem_05_var);
   // Assign it the value 50000*50000 (exactly as shown here)
   problem_05_var = 50000*50000;
   // Print it
   printf("x = 50000*0000\n");
   printf("Expected: 2500000000 \n");
   printf("  Result: %d\n\n", problem_05_var);
   // Assign it the value 60000*60000 (exactly as shown here)
   problem_05_var = 60000*60000;
   // Print it
   printf("x = 60000*60000\n");
   printf("Expected: 3600000000 \n");
   printf("  Result: %d\n\n", problem_05_var);
   // Assign it the value 70000*70000 (exactly as shown here)
   problem_05_var = 70000*70000;
   // Print it
   printf("x = 70000*70000\n");
   printf("Expected: 4900000000 \n");
   printf("  Result: %d\n\n", problem_05_var);
   printf("\n");

   // Problem 6:
   // - Use floats for this problem
   printf("Problem 6:\n");
   // - Assign the value 1e20 to a float and print it
   float problem_06_var = 1e20;
   printf("x = 1e20\n");
   printf("Expected: 100000000000000000000 \n");
   printf("  Result: %f\n\n", problem_06_var);
   // - Assign the value (1e20 + 3500000000) to a float and print it
   problem_06_var = (1e20 + 3500000000);
   printf("x = 1e20 + 3500000000\n");
   printf("Expected: 100000000003500000000 \n");
   printf("  Result: %f\n\n", problem_06_var);
   // - Assign the value (1e20 + (3500000000 * 1000000000)) to a float and print it
   problem_06_var = (1e20 + (3500000000 * 1000000000));
   printf("x = 1e20 + (3500000000 * 1000000000)\n");
   printf("Expected: 103500000000000000000 \n");
   printf("  Result: %f\n\n", problem_06_var);
   // - Declare a variable of type float
   // - Assign it the value of 1e20
   problem_06_var = 1e20;
   // - Use a loop to add 3500000000 to that variable 1000000000 times.
   // - Note: Don't just multiply then add for that last part above. Use a loop to add 3500000000 at each iteration.
   for(int i = 0; i < 1000000000; i++){
      problem_06_var = (problem_06_var + 3500000000);
   }
   // - Print the result after the loop
   printf("x = w/loop (1e20 + (3500000000 * 1000000000)\n");
   printf("Expected: 103500000000000000000 \n");
   printf("  Result: %f\n\n", problem_06_var);
   return 0;
}