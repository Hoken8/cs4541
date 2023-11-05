Date: 11/04/2023
Class: CS4541
Assignment: Cache Simulator
Author(s): Daniel Wilkins

gcc -std=c99 -Wall -o csim csim.c

Run: ./csim [-hv] -s <s> -E <E> -b <b> -t <tracefile>

Note: verbose mode not implemented due to instructions.

Note: I used the create code provided by the instructor Hilil Ibrhim Dursunoglu. see below for the code snippet. 

Code snippet:
```c
typedef struct cache_line {
    char valid;
    mem_addr_t tag;
    int counter;
    struct cache_line * next;
} cache_line_t;

void initCache()
{
    // Compute S = 2^s and B = 2^b
    S = ( 1 << s);
    B = ( 1 << b);

    // Initialize the cache
    cache = malloc(S * sizeof(cache_set_t));        //allocate space for S number of sets
    for (int i = 0; i < S; i++) {
        
        cache[i] = malloc(E * sizeof(cache_line_t));    //allocate space for E number of lines in each set
        for (int j = 0; j < E; j++) {
            cache[i][j].valid = 0;
            cache[i][j].tag = 0;
            cache[i][j].counter = 0;
            cache[i][j].next = NULL;

            //set next of previous line to current line
            if (i > 0 && j==0) { //if it is the beginning of a new set and there is a previous set
                cache[i-1][E-1].next = &cache[i][j];
            }
            else if (j>0) { //as long as not at the first line at index i=0 j=0, set next of previous line
                cache[i][j-1].next = &cache[i][j];
            }
        }
    }

}

void accessData(mem_addr_t addr)
{
 

    // Get the address of the first s bit, and the set number of addr
    mem_addr_t sBit = addr >> b;
    mem_addr_t set = sBit & ((int)pow(2,s) - 1);

    // Get the address of the first t bit, and the tag of addr
    mem_addr_t tBit = addr >> (b + s);
    mem_addr_t tag = tBit & ((int)pow(2, 64 - (b+s)) -1);

........ }

```
End of Code snippet:

References: I did this on my own with no research