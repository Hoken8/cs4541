#define MAP_ANONYMOUS 0x20
#define INITHEAPSIZE 1000
#define MAXHEAPSIZE 100000

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <getopt.h> 


//heap variables
int32_t *mem_heap = NULL;
intptr_t heap_size = 0;

//command line variables
char* output_path = NULL;
char* input_file = NULL;
char* free_list = NULL;
char* fit = NULL;
int verbose_mode = 0;


void *myalloc(intptr_t size){
    int32_t *ptr = NULL;
    printf("%d, 0x%08x \n", 1, mem_heap[1]);
    int index = 1;
    while((index < heap_size) && !(mem_heap[index]& 1)){
        printf("%d, 0x%08x \n", 1, mem_heap[1]);
        printf("index: %d\n", index);
        printf("max heap: %ld\n", heap_size);
        index += mem_heap[index]>>2;
    }
    return ptr;
}

void printHeap(int N, int32_t *ptr){
    // Print the elements of the array
    printf("The elements of the array => \n");
    for(int i = 0; i < N; i++)
        printf("%p: %d, 0x%08x \n",(void*)&ptr[i], i, ptr[i]);
    printf("\n");

    // printf("start: %p\n",(void*)&ptr[0]);
    // printf("mem_heap: %p\n",(void*)mem_heap);
    // printf("end: %p\n",(void*)(ptr+N-1));
}


void *initHeap(intptr_t size){
    int32_t *ptr = mmap ( NULL, size*sizeof(int32_t),
        PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0 );

    if(ptr == MAP_FAILED){
        printf("Mapping Failed\n");
        return (void *)-1;
    }

    heap_size = size;
    ptr[0] = 0x00000001;
    ptr[1] = (size-2)<<2;
    ptr[size-2] = (size-2)<<2;
    ptr[size-1] = 0x00000001;

    return ptr;
}

//opens the inputfile and reads each command line
void runInputFile(char* file_name) {
    printf("opening file: %s\n",file_name);
    FILE* fp = fopen(file_name, "r");
    char cmd;
    int v1;
    int v2;
    int v3;

    while (fscanf(fp, " %c, %d, %d, %d", &cmd, &v1, &v2, &v3) != -1) {
        switch(cmd) {
            case 'a':
                printf("a %d %d\n",v1,v2);
                myalloc(v1);
                break;
            case 'r': 
                printf("r %d %d %d\n",v1,v2,v3);
                break;
            case 'f':
                printf("f %d\n",v1);
                break;
            default:
                break;
        }
    }
    fclose(fp);
}

void saveHeap(char* path, char* name_part_1, char* name_part_2){

}

int freeHeap(){
    int err = munmap(mem_heap, 1000*sizeof(int32_t));
    if(err != 0){
        printf("UnMapping Failed\n");
        return -1;
    }
    return 0;
};

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

    mem_heap = initHeap(INITHEAPSIZE);
    runInputFile(input_file);
    //printHeap(heap_size, mem_heap);
    saveHeap(output_path, free_list, fit);
    freeHeap(mem_heap);
    return 0;
}