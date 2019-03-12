// This header is redefining every call to 'malloc' to our implementation
// 'mymalloc'
// Again, this is a simple textual replacement of the code by the preprocessor

void *mymalloc (size_t size_asked);

void *mycalloc (size_t size);

void myfree(void *ptr);

typedef struct block_header {
  unsigned int    size : 29,
                  zero : 2,
                  alloc: 1;
} block_header;

#define size4(x) (((((x)-1)>>2)<<2)+4)

#define BH_SIZE 4
