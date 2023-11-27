#define INITHEAPSIZE 1000                                                   //intial heap size
#define MAXHEAPSIZE 100000                                                  //maximum heap size

#define PACK(size, alloc) ((size<<2)|(alloc))                               //set allocation bit and size into a word
#define GET(p) (*(p))                                                       //reads a word at address p
#define PUT(p, val) (*(p) = val)                                            //writes a word at address p
#define GET_SIZE(p) (GET(p)>>2)                                             //reads the size from address p
#define GET_ALLOC(p) (GET(p) & 0x1)                                         //reads the allocated bit from the address p
#define HDRP(bp) ((uint32_t *)(bp) - 1)                                     //finds the of the header of a block from the block pointer
#define FTRP(bp) ((uint32_t *)(bp) + GET_SIZE(HDRP(bp))-2)                  //finds the footer of a block from the block pointer
#define NEXT_BLKP(bp) ((uint32_t *)(bp) + GET_SIZE((uint32_t *)(bp)-1))     //calcutes address of next block
#define PREV_BLKP(bp) ((uint32_t *)(bp) - GET_SIZE((uint32_t *)(bp)-2))     //calcutes address of previous block


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>


//heap variables
static uint32_t *mem_heap = NULL;           //address of the start of the heap
static uint32_t *heap_listp = NULL;         //first block address in heap
static uint32_t *mem_brk = NULL;            //last address in heap
static uint32_t heap_size = 0;              //number of words in heap

//command line variables
char* output_path = NULL;                   //user input: output path
char* input_file = NULL;                    //user input: command file 
char* free_list = NULL;                     //user input: heap list type
char* fit = NULL;                           //user input: fit type   
int verbose_mode = 0;                       //user input: verbose mode flag

//pointer address storage
struct pointers
{
   int key;                                 //pointer index 
   uint32_t* addr;                          //pointer address
};

size_t total_pointers = 0;                  //number of active pointers

//sets the boundry headers and footer for fresh heap
int setMemory(){
    for(int i = 0; i > heap_size; i++){
        mem_heap[i] = 0;
    }
    PUT(mem_heap, PACK(0,1));
    PUT(mem_heap+1, PACK((heap_size-2),0));
    PUT(mem_brk-2, PACK((heap_size-2),0));
    PUT(mem_brk-1, PACK(0,1));
    return 0;
}

// grows or shrinks the size of the heap by a number of words specified by the input parameter "size"
// returns pointer to start of the heap
void *mysbrk(uint32_t size){

    //does nothing if called with 0
    // return the old pointer to the start of the heap
    if(size == 0){
        return (void *)mem_heap;
    }

    // errors if more than maximum memory is called
    // returns null
    if((size + heap_size) > MAXHEAPSIZE){
        printf("Error: Heap size exceeded MAXIMUM ALLOWABLE SIZE of %d.\n\n", MAXHEAPSIZE);
        return NULL; 
    }

    // allocs virtual heap memory in your heap memory
    // sets all pointers
    // returns pointer to the start of the heap
    if(size > 0){
        uint32_t *new_mem_heap = (uint32_t *)realloc(mem_heap, heap_size+(size)*sizeof(uint32_t));
        heap_listp  = &new_mem_heap[2];
        heap_size += size;
        mem_brk = &new_mem_heap[heap_size];
        return (void *)new_mem_heap;
    }
    return (void *)mem_heap;
}

// find the first unallocated block
// returns address that block
// returns null if nothing found
static void *find_fit(uint32_t asize){
    void *bp;
    for(bp = heap_listp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp)){
        if(!GET_ALLOC(HDRP(bp)) && (asize <= GET_SIZE(HDRP(bp)))){
            printf("<FF>found: %p\n",bp);
            return bp;
        }
    }
    return NULL;
}

// places a block into memory
static void place(void *bp, uint32_t asize){
    uint32_t csize = GET_SIZE(HDRP(bp));
    
    //checks for smallest block with data alinement 
    if((csize - asize) >= 4){
        PUT(HDRP(bp), PACK((asize),1));
        PUT(FTRP(bp), PACK((asize),1));
        bp = NEXT_BLKP(bp);
        PUT(HDRP(bp), PACK((csize-asize),0));
        PUT(FTRP(bp), PACK((csize-asize),0));
    }else{
        PUT(HDRP(bp), PACK((csize),1));
        PUT(FTRP(bp), PACK((csize),1));
    }
}

// takes an integer value indicating the number of bytes to allocate for the payload of the block
// returns a "pointer" to the starting address of the payload of the allocated block
void *myalloc(size_t size){
    size_t asize;
    uint32_t *bp;

    // return null is size is 0
    if(size == 0){
        return NULL;
    }

    // calculates total block size
    asize = ((int)(size>>2) +((int)(size%4))+2);
    if(asize%2 != 0){
        asize++;
    }
    
    // find the a smaller free block to write to
    if((bp = find_fit(asize)) != NULL){
        place(bp, asize);
        return bp;
    };

    // increases the heap to hold block that will not fit
    // sets the new heaps headers / footers
    PUT(mem_brk, PACK(asize,0));
    mem_heap = mysbrk(asize);
    bp = find_fit(asize-1);
    PUT(mem_brk-2, PACK((asize),0));
    PUT(mem_brk-1, PACK(0,1));
    place(bp, asize);
    return bp;
}

// coalesce blocks
// case 1: alloceted prev and next
// case 2: alloceted prev only
// case 3: alloceted next only
// case 4: both unalloceted

static void *coalesce(uint32_t *bp){
    uint32_t prev_alloc = *(uint32_t *)(HDRP(bp)-1)& 0x1;
    uint32_t next_alloc = *(uint32_t *)(FTRP(bp)+1)& 0x1;
    uint32_t size = GET_SIZE(HDRP(bp));

    // case 1 [1,1]
    if(prev_alloc && next_alloc){
        return bp;
    }

    // case 2 [1,0]
    else if(prev_alloc && !next_alloc){
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        PUT(HDRP(bp), PACK((size),0));
        PUT(FTRP(bp), PACK((size),0));
    }

    // case 3 [0,1]
    else if(!prev_alloc && next_alloc){
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        PUT(FTRP(bp), PACK((size),0));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size,0));
        bp = PREV_BLKP(bp);
    }

     // case 4 [0,0]
    else{
        size += GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(FTRP(NEXT_BLKP(bp)));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size,0));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(size,0));
        bp = PREV_BLKP(bp);
    }

    return bp;
}

// frees block from heap
void myfree(int32_t *bp){
    printf("\n<myFree>: %p\n", bp);
    size_t size = GET_SIZE(HDRP(bp));

    PUT(HDRP(bp), PACK(size,0));
    //PUT(FTRP(bp), PACK(size,0));

    coalesce((void*)bp);
}

// takes a pointer to an allocated block and an integer value to resize the block
void *myrealloc(int32_t *bp, size_t size){
    size_t asize = GET_SIZE(HDRP(bp));
    uint32_t *new_bp;


    new_bp = myalloc(size);
    memcpy(new_bp, bp, sizeof(int32_t)*(asize-2));
    size_t fsize = GET_SIZE(HDRP(bp));
    PUT(HDRP(bp), PACK(fsize,0));
    PUT(FTRP(bp), PACK(fsize,0));
    return new_bp;
}

void printHeap(int N, uint32_t *ptr){
    // Print the elements of the array
    printf("\nThe elements of the array => \n");
    for(int i = 0; i < heap_size; i++)
        printf("%p: %d, 0x%08X \n",(void*)&ptr[i], i, ptr[i]);
    printf("\n");
}

int saveHeap(int N, uint32_t *ptr){
    char filename[32];
    char *filenumber = strtok(input_file, "/");
    filenumber = strtok(NULL, ".");
    snprintf(filename, sizeof(char) * 32, "output/%s.%s.%s.out", filenumber, free_list, fit);

    FILE* fp = fopen(filename, "w+");
    if (fp == NULL) { 
        printf("Failed to create the file.\n");
        return -1;
    }
    for(int i = 0 ; i < heap_size; i++){
        if(ptr[i] != 0){
            fprintf(fp, "%d, 0x%08X\n", i, ptr[i]);
        } else {
            fprintf(fp, "%d, \n", i);
        }
    }
    fclose(fp);
    return 0;
}

// prints out instructions for using application
void printInstructions(){
    printf("\n");
    printf("Usage: ./hsim: [-hv] [-o <output-path>] [-l {implicit or explicit}] [-f {first or best}] [-i <input file>]\n");
    printf("  -h:                       Optional help flag that prints usage info\n");
    printf("  -v:                       Optional verbose flag that displays input info\n");
    printf("  -o <path of output file>: Path of output file\n");
    printf("  -l <free-list type>:      Free-list type, {implicit or explicit}\n");
    printf("  -f <fit type>:            Fit type, {first or best}\n");
    printf("  -i <input file>:          Name of file containing input commands\n\n\n");
    printf("Example: ./hsim -o outputfile -l implicit -f fit -i 1.in\n\n\n");
    exit(0);
}



//opens the inputfile and reads each command line
void runInputFile(char* file_name) {
    FILE* fp = fopen(file_name, "r");
    char cmd;
    int v1, v2, v3;
    uint32_t *bp;
    
    struct pointers *pts = NULL;

    while (fscanf(fp, " %c, %d, %d, %d", &cmd, &v1, &v2, &v3) != -1) {
        switch(cmd) {
            case 'a':
                //printf("\nRun Command: a %d %d\n",v1,v2);
                bp = myalloc(v1);
                pts = realloc(pts, (sizeof(struct pointers) * (total_pointers+1)));
                pts[total_pointers].key = v2;
                pts[total_pointers].addr= bp - *mem_heap;
                total_pointers++;
                break;
            case 'r': 
                //printf("\nRun Command: r %d %d %d\n",v1,v2,v3);
                for(int i = 0; i < total_pointers; i++){
                    if(pts[i].key == v2 && pts[i].addr != NULL){
                        bp = pts[i].addr + *mem_heap;
                        pts[i].addr = myrealloc((void*)bp, v1);
                        pts[i].key = v3;
                    }
                }
                break;
            case 'f':
                //printf("\nRun Command: f %d\n",v1);
                int pos = 0;
                for(int i = 0; i < total_pointers; i++){
                    if(pts[i].key == v1 && pts[i].addr != NULL){
                        bp = pts[i].addr + *mem_heap;
                        pos = i;
                        myfree((void*)bp);
                    }
                }
                {
                    int j = 0;
                    for(int i = 0; i < total_pointers; i++)
                    {
                        if(pos==i){
                            continue;
                        }
                        pts[j].addr = pts[i].addr;
                        pts[j].key = pts[i].key;
                        j++;
                    }
                    //Decrement size
                    total_pointers--;
                    pts = realloc(pts, (sizeof(struct pointers) * (total_pointers)));
}
                break;
            default:
                break;
        }
    }
    free(pts);
    fclose(fp);
}


int main(int argc, char* argv[]){
    char arg; // argument from command line

    // parse over all the command line arguments 
    while((arg =getopt(argc, argv, "o:l:f:i:vh")) != -1){
        switch (arg)
        {
            case 'o':
                output_path = optarg;
                break;
            case 'l':
                free_list = optarg;
                break;
            case 'f':
                fit = optarg;
                break;
            case 'v':
                verbose_mode = 1;
                break;
            case 'i':
                input_file = optarg;
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
    if (output_path == NULL || free_list == NULL || fit == NULL || input_file == NULL) {
        printf("Missing command line argument\n");
        printf("output_path: %s\n", output_path);
        printf("free_list: %s\n", free_list);
        printf("fit: %s\n", fit);
        printf("input_file: %s\n", input_file);
        printInstructions();
        exit(1);
    }

    // sets up heap
    mem_heap = mysbrk(INITHEAPSIZE);
    if(mem_heap == NULL){
        exit(1);
    }

    // sets boundry headers and footers
    setMemory();

    // reads command file
    runInputFile(input_file);

    // outputs heap
    if(verbose_mode){
        printHeap(heap_size, mem_heap);
    }
    saveHeap(heap_size, mem_heap);
    
    free(mem_heap);
    return 0;
}