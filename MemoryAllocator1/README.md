# Due Date: October 29, 2018 at 11:59 PM.

TODO Please edit the following information in your assignment

- Did you implement a best-fit or first-fit allocator?
best fit
- Name: walter geanacopoulos
- How many hours did it take you to complete this assignment? 20
- Did you collaborate with any other students/TAs/Professors? no
- Did you use any external resources? (Cite them below)
  - google
  - 
- (Optional) What was your favorite part of the assignment? learning how a memory allocator works
- (Optional) How would you improve the assignment? n/a

# Logistics

For this assignment, you must login into the servers through `your_ccis_name@login.ccs.neu.edu` to complete the assignment. The reason is because the examples I will provide below are compiled strictly for our machines architecture, and this is where your submission will be graded.

**Assignment Strategy**

As with most assignments, it can be useful to compile, save, and test often. If you get stuck, try making small .c programs and running experiments and testing your assumptions.

# Introduction - Memory Allocator 1

For this assignment, you will be writing your own memory allocator! Writing a custom memory allocator is something you might do, especially if you work on any performance sensitive system (games, graphics, quantitative finance, embedded devices or just an application you want to run fast!). [Malloc](https://linux.die.net/man/3/malloc) and [Free](http://man7.org/linux/man-pages/man1/free.1.html) are general purpose functions written for the average use case, but they can always be optimized for a given workload (aside: a lot of smart folks have worked on making malloc/free quite performant!). And optimization aside, you might simply write an allocator to add in cool debugging features, and swap it in as needed.

For this assignment, we will implement a portion of a custom memory allocator for the C language. This includes writing:

- malloc
- calloc
- free

### Part 1 - sbrk allocator

First you will want to think about what data structure you want to use for your allocator. Your data structure needs to be able to dynamically expand. Once you are convinced of this data structure, you are ready to begin implementing a memory allocator.

Your objective will be to create three functions in 'mymalloc.c'

1. malloc
2. free
3. calloc 

You may define any helping data structures and functions that assist you in this task. I recommend functions for traversing your data structure and finding free blocks/requesting blocks for example.

A `compile.py` file and `run.py` file is also provided for you.

1. First run `python compile.py` to make sure everything builds by default.
2. Then run `python run.py` to make sure everything runs (`run.py` also compiles the code).
3. You will notice in the tests directory, some small tests have been created for you. 
	- test1, test2, etc. use the default implementation of malloc. 
	- test1_mymalloc, test2_mymalloc, etc. use your custom implementation of malloc and free. 
	- You will notice in the compile script, the use of compile-time interpositioning to replace the default C memory allocator with your own.
4. Okay, now you are ready to begin implementing malloc, calloc, and free. Some details on the specifications are below.

#### Your task is to implement malloc and free using either a best-fit(for full marks) or first-fit(almost full marks) memory allocator.

Let me give you some tips on how to do this.

1. You should have a linked list data structure. This data structure will keep track of the blocks that you have allocated within your malloc function. You should have a global variable that serves as the "head" or "first block of memory" in your linked list.
	- You should have some notion of a 'block' of memory in your program.
	- An example is provided here with some fields that may be useful:
	```c
	  struct block{
	    size_t size; // How many bytes beyond this block have been allocated in the heap
	    block* next; // Where is the next block
	    int free; // Is this memory free?
	    int debug; // (optional) Perhaps you can embed other information--remember, you are the boss!
	  };
	```
2. You will want to keep track of how big this block structure is. A little trick is to use the preprocessor so you can simply use BLOCK_SIZE in your code.
	```c
	#define BLOCK_SIZE sizeof(block)
	```
3. The `sbrk` command will be your friend. Understand what sbrk(0) does and sbrk(10) before getting started. Your malloc function (or helper functions) are what will use malloc.
4. Your free function can be as simple as setting a block of memory to be free. Consider how memory is laid out in the heap however to make sure you are only accessing your struct. Here is a simple diagram: 
```
|block|----actual memory----|block|--------actual memory-------|block|--actual memory--|

  ^ Here is where your struct lives, this is what you want to update.
```
5. Your `calloc` function can make a call to malloc. Remember what is different about calloc however!
6. A first-fit memory allocator means, we are looking for any block in our linked list that is free (any previously allocated blocks that is). Remind yourself what the trade-off is between the other allocators (e.g. compare to a 'best-fit' allocator).
	- A best-fit allocator (which will earn full marks on this assignment) scans through a list and finds the optimal position to minimize external fragmentation.
7. Your malloc function should print out "malloc %zu bytes\n"
8. Your calloc function should print out "calloc %zu bytes\n"
9. Your free function should print out "Freed %zu bytes\n"
10. With these print outs, you can see if they match the original programs.
11. I will look through your code to make sure you do not use malloc/calloc/free from <stdlib.h>. You should only be using syscalls such as sbrk (you may also use mmap if desired).

## Rubric

- 20% on good programming style
	- Did you break your code into functions that are < 50 lines.
	- Are you using lots of globals? (Note: It *may be okay* to use a few for an allocator)
- 80% for a working Memory Allocator(i.e. malloc/calloc/free)
  - 75% You implement a working first-fit allocator that does not leak memory.
  - or
  - 80% You implement a working best-fit allocator that does not leak memory.
  - If you implement both, let us know which one to grade.
    
# Resources to help
- Advanced Material: https://www.youtube.com/watch?v=kSWfushlvB8 ("How to Write a Custom Allocator" for C++)
- The book Programming in C (K&R book) has a nice description of memory allocators.

### Glossary and Tips
1. Commit your code changes to the repository often. This is best practice.
2. Do not commit your .o file or your executable file to the repository. This is considered bad practice!
3. On Functionality/Style
	1. You should have comments in your code to explain what is going on, and use good style (80 characters per line maximum for example, functions that are less than 50 lines, well named variables, well named functions etc.).

# Going Further

- You do not have to align your memory, but you may choose to (I will only run test on nicely sized memory blocks).
- Blocks of memory typically are aligned. That is, they are aligned to some address boundary (every 8, 16, 32, 64, etc.). In order to make our memory allocator more simple, we can force alignment of every block. Here is a trick using a union from the K&R book.

```c
typedef long Align; // Alignment to long size

union header{
  struct{
    union header *ptr;  // next block in free list
    unsigned size;      // Size of this block
  } s;
  Align x;  // Force alignment of blocks to our boundary.
            // Note this field is never used, but space is allocated for it.
            // Convince yourself this 'trick' works and why.
};
```


Other Notes:

* In order to avoid fragmentation, most allocators combine blocks that are adjacent(on both sides) into bigger ones. They may then split those bigger blocks as need as well. You could write some helper functions to assist with this, and have a more optimal memory allocator. 
	- This is optional for this assignment.
* The malloc implementation is actually open source, so you can take a look at it if you are curious.

# Feedback Loop

(An optional task that will reinforce your learning throughout the semester)

Not applicable, as we will do more with allocators later!
