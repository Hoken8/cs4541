#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h> 

#define pow(two, shift) (1 << (shift))

typedef unsigned long long int mem_addr_t;  //shorthand, memory address size

typedef struct cache_line {     //Block structure
    char valid;                 //validation bit
    mem_addr_t tag;             //tag bits
    int counter;                //counter for when caches was accessed
    struct cache_line * next;   //next address in cache
} cache_line_t;

typedef cache_line_t* cache_set_t;  //array of blocks
typedef cache_set_t* cache_t;       //array of sets
cache_t cache;                      //cache

int S;                  // number of Sets
int B;                  // Block size

int verbose_mode = 0;   // verbose mode: line by line details of trace file actions
int s = 0;              // number of set index bits
int E = 0;              // number of line per set
int b = 0;              // numober of block bits
char* trace_file = NULL;// valgrind trace file name

int misses = 0;     // accumulator for misses
int hits = 0;       // accumulator for hits
int evictions = 0;  // accumulator for evictions

// initialize a cache of size S X E
void initCache()
{
    // Compute S = 2^s and B = 2^b
    S = (unsigned int)( 1 << s);
    B = (unsigned int)( 1 << b);

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

// reads cache and writes to cache when needed
void accessData(mem_addr_t addr)
{
    // Get the address of the first s bit, and the set number of addr
    mem_addr_t sBit = addr >> b;
    mem_addr_t set = sBit & ((int)pow(2,s) - 1);

    // Get the address of the first t bit, and the tag of addr
    mem_addr_t tBit = addr >> (b + s);
    mem_addr_t tag = tBit & ((int)pow(2, 64 - (b+s)) -1);

    // sets the current set to search
    cache_set_t cache_set = cache[set];

    //check for hit
    for (int i = 0; i < E; ++i) {
        if (cache_set[i].valid) {
            if (cache_set[i].tag == tag) {
                hits++;
                if(verbose_mode){
                    printf(" hit");
                }
                return;
            }
        }
    }

    misses++;

    //find the next cache place to write
    for (int i = 0; i < E; ++i) {
        if (cache_set[i].counter == 0) {
            cache_set[i].valid = 1;
            cache_set[i].tag = tag;
            cache_set[i].counter = 1;
            if(verbose_mode){
                    printf(" miss");
            }
            break;
        }else if(cache_set[i].counter == E) {
            cache_set[i].valid = 1;
            cache_set[i].tag = tag;
            cache_set[i].counter = 1;
            evictions++;
            if(verbose_mode){
                    printf(" miss eviction");
            }
        }else{
            cache_set[i].counter++;
        }
    }
}

// frees cache
void freeCache(){
    for(int i = 0; i < S; i++){
        free(cache[i]);
    }
    free(cache);
}

// opens the trace file and reads each command line
// does nothing for line starting with 'I' aka instruction lines
void runTraceFile(char* file_name) {
    FILE* fp = fopen(file_name, "r");
    char cmd;
    mem_addr_t address;
    int size;

    while (fscanf(fp, " %c %llx,%d", &cmd, &address, &size) == 3) {
        if(verbose_mode && (cmd =='L'||cmd =='S'||cmd =='M')){
            printf("%c %llx,%d", cmd, address, size);
        } 
        switch(cmd) {
            case 'L':
                accessData(address);
                break;
            case 'S': 
                accessData(address); 
                break;
            case 'M': 
                accessData(address); 
                accessData(address); 
                break;
            default:
                break;
        }
        if(verbose_mode && (cmd =='L'||cmd =='S'||cmd =='M')){
            printf("\n");
        }
    }
    fclose(fp);
}

// prints out instructions for using application
void printInstructions(){
    printf("\n");
    printf("Usage: ./cism [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n");
    printf("  -h:             Optional help flag that prints usage info\n");
    printf("  -v:             Optional verbose flag that displays trace info\n");
    printf("  -s <s>:         Number of set index bits (S = 2 s is the number of sets)\n");
    printf("  -E <E>:         Associativity (number of lines per set)\n");
    printf("  -b <b>:         Number of block bits (B = 2 b is the block size)\n");
    printf("  -t <tracefile>: Name of the valgrind trace to replay\n\n\n");
    exit(0);
}

// prints out the global totals of hits, misses, and evictions.
void printTotals(int hits, int misses, int evictions){
    printf("hits:%d misses:%d evictions:%d\n", hits, misses, evictions);
}

int main(int argc, char* argv[]){
    char arg; // argument from command line

    // parse over all the command line arguments 
    while((arg =getopt(argc, argv, "s:E:b:t:vh")) != -1){
        switch (arg)
        {
            case 's':
                s = atoi(optarg);
                break;
            case 'E':
                E = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
                break;
            case 't':
                trace_file = optarg;
                break;
            case 'v':
                verbose_mode = 1;
                break;
            case 'h':
                printInstructions();
                exit(0);
            default:
                printInstructions();
                exit(1);
        }
    }

    // check for missing arguments
    if (s == 0 || E == 0 || b == 0 || trace_file == NULL) {
        printf("Missing command line argument\n");
        printInstructions();
        exit(1);
    }

    initCache();
    runTraceFile(trace_file);
    freeCache();
    printTotals(hits, misses, evictions);
    return 0;
}