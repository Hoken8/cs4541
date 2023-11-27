Assignment
Introduction
In this assignment, you will create a memory allocation simulator. You will be evaluated only on the correctness of your simulated heap, so you don't have to worry about throughput. You may use any programming language you choose from the following options:

C
C++
Java
Ruby
Python
You will not actually make any memory calls in your program. You will simply operate on a simulated heap. You may make all the decisions about how you represent your simulated heap, but you will need to accept input and produce output according to the specifications that follow in the "Specifications" section.

You read a series of allocation, reallocation, and free calls from an input text file, detailed in the "Specifications" section below, and process them using both implicit and explicit free list approaches. You must use both headers and footers for blocks in both approaches. You will also allow the user to choose whether to use a First-fit or Best-fit approach to allocating memory blocks. For all cases, you will use immediate coalescing of free blocks.

All work should be your own. You are prohibited from using any code that is not your own and from working in groups. If you find examples of HOW to do something on the internet or elsewhere you must reference the source and what you used it for in your report. For example, you may look for examples on how to write output to a file, as long as you give credit to the source in your references and specify the reason you referenced that resource. Any use of code that is not your own will be considered to be academic dishonesty. You MUST write your own code (no use of ANY code from the internet or elsewhere) for the four primary functions (myalloc, myrealloc, myfree, mysbrk) in the assignment.

Specifications
Your Heap
Start your heap at address 0
Assume a 32-bit system (so each word is 4 bytes)
Assume double-word alignment of all allocated blocks
Calls to myalloc and myrealloc take an input parameter indicating the size of the allocation in bytes
Invalid calls to any of your primary functions will give an error but not crash your simulator (though they may have no effect on the heap)
Your initial heap size will be 1000 words, and you may expand your heap to 100,000 words maximum
As an example, your heap would start at word 0. If your first call is myalloc(5), then you would start the header at word 1, your payload at word 2, and your footer at word 4 to meet alignment requirements. This is because the payload would have to start at an address divisible by 8, and take up two words, 5 bytes for the payload and 3 bytes of padding. So, your header would start at word 1 (address 4), the payload would start at word 2 (address 8), and your footer would start at word 4 (address 16). This would allow your next header to start at word 5 (address 20) and the next payload to start at word 6 (address 24). 

         |header  |     payload     |footer  |        
|--------|--------|--------|--------|--------|--------|
0        1        2        3        4        5        6

Primary Functions
You will have four primary functions in your assignment, which MUST be named as follows:

myalloc(size)
takes an integer value indicating the number of bytes to allocate for the payload of the block
returns a "pointer" to the starting address of the payload of the allocated block
The "pointer" above can take any form you like, depending on the data structure you use to represent your heap
myrealloc(pointer, size)
takes a pointer to an allocated block and an integer value to resize the block to
returns a "pointer" to the new block
copies the payload from the old block to the new block
frees the old block
a call to myrealloc with a size of zero is equivalent to a call to myfree
myfree(pointer)
you must use a LIFO policy for explicit free lists
prev pointer should be first and next pointer should be second (opposite from slides)
frees the block pointed to by the input parameter "pointer"
returns nothing
only works if "pointer" represents a previously allocated or reallocated block that has not yet been freed
otherwise, does not change the heap
coalesce after freeing, coalesce lower before coalescing higher addresses, and update headers last
mysbrk(size)
grows or shrinks the size of the heap by a number of words specified by the input parameter "size"
you may call this whenever you need to in the course of a simulation, as you need to grow the heap only as much as needed for the allocation, do not extend a free block at end of previous space, use a totally new block
this call will return an error and halt the simulation if your heap would need to grow past the maximum size of 100,000 words
User Options
The user must be able to specify the following (either in a GUI or on the command line) for each run of your simulator:

Input text file
Implicit or Explicit free list
First-fit or Best-fit allocation
Input
You will process a text file to get the series of allocation, reallocation, and free calls that your simulator should make on your simulated heap. The input file will be in the form of a simple comma-separated value format in which each line describes a single call.

Example:

a, 5, 0
f, 0
a, 10, 1
r, 20, 1, 2
f, 2
Calls to myalloc will be indicated in the input file as follows:

An "a" to indicate that this is an allocation call
An integer to indicate the "size" parameter
An integer between 0 and 999 to act as a reference the block allocated by the call
This value will be used to tie future calls to "myfree" and "myrealloc"
You may use this value any way you wish. It can be used to name pointers returned by your simulator, as the key to key-value pairs that keep track of your simulated "pointer"s, etc. It is simply there to ensure that we are calling "myrealloc" and "myfree" on the correct blocks
Calls to myrealloc will be indicated in the input file as follows:

An "r" to indicate that this is a reallocation call
An integer to indicate the "size" parameter
An integer between 0 and 999 to reference which block created by a previous call to myalloc we are resizing
An integer between 0 and 999 to reference the new block allocated by the call
Calls to myfree will be indicated in the input file as follows:

An "f" to indicate that this is a free call
An integer between 0 and 999 to specify the allocation call that this call is freeing
So, let's break down the example above:

a, 5, 0      // ptr0 = myalloc(5)
f, 0         // myfree(ptr0)
a, 10, 1     // ptr1 = myalloc(10)
r, 20, 1, 2  // ptr2 = myrealloc(ptr1,20)
f, 2         // myfree(ptr2)
The above example assumes that you use the reference number appended to the string "ptr" as your returned pointer names, but, again, this is not required. You may use the reference numbers however you wish, but this is one example of how you can use them to make sure that you are calling your functions on the correct blocks.

You do not need to validate the input for this project. You may assume that all input files are formatted correctly when you process them.

Output
Your output from each run will be another comma-separated value text file called "output.txt"

Simply indicate the value of each word in your simulated heap in hexadecimal format. Start with word 0 and work your way up to whatever the last word in your heap ends up being after the simulation run. 

In the following example, we use bit 0 of header and footer to indicate the use of current block: 1 for allocated and 0 for free. We do not use bits 1 and 2. Instead, we use header of the next block and footer of the previous block to check for possible coalescing. For myrealloc(), we implement it as myalloc() followed by myfree(). The output of running the above example using implicit free list could be:

  0, 0x00000001 // placeholder
  1, 0x00000F98 // header
  2,            // payload
  3,            // payload
  4, 0x00000011 // remaining footer of myalloc(5)
  5, 0x00000F88 // remaining header of free block after myalloc(5)
  6, 0x00000018 // remaining footer of myalloc(10)
  7, 0x00000021 // remaining header of myrealloc(20)
     ......
 10, 0x00000011 // copied payload from word 4 of myrealloc(20)
 11, 0x00000F88 // copied payload from word 5 of myrealloc(20)
     ......
 14, 0x00000021 // remaining footer of myrealloc(20)
 15, 0x00000F60 // remaining header of free block after myrealloc(20)

     ......

998, 0x00000F98 // footer
999, 0x00000001 // placeholder

The first value indicates the word in question, and the second indicates the contents of that word (represented in hexadecimal). That is why there are 8 hexadecimal digits for each entry in the example above. Each word will contain 4 bytes since this is a 32-bit system. Your output file will be checked to ensure that the heap contains the correct headers, footers, and pointers (in the case of explicit free lists) for the simulation that has just been run.

Submission
Submit your source code and a PDF file containing your report to the Dropbox called "Memory_Allocation_Assignment" in Elearning. 

Your report will simply contain your name, all references, explanations of what each reference was used to help you with, and an explanation of how to run your code. Indicate what IDE (if any) you used, compilation instructions, invocation instructions, etc. You should explain in detail how to run your program. I will test it with input files in the form detailed above, and you can create your own to test things out as you work on your assignment.

Evaluation
Report: 10%
Programming Style: 10%
Program correctness: 80%
(Implicit first-fit: 20%, best-fit: 20%, Explicit first-fit: 20%, best-fit: 20%)