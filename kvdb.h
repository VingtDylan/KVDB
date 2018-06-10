#include <stdio.h>
#include <pthread.h>

#define M 100
#define MAX_LEN 1024
#define LENGTH  1 << 20

#define RED                    "\033[0;32;31m"   
#define GREEN                  "\033[0;32;32m"   
#define BLUE                   "\033[0;32;34m"   
#define BROWN                  "\033[0;33m"   
#define YELLOW                 "\033[1;33m"  
#define PRUPLE                 "\033[0;35m"
#define CYAN                   "\033[0;36m"
#define NONE                   "\033[m"

#define EXIT_ERR(m)\
do\
{\
   perror(m);\
   exit(EXIT_FAILURE);\
}\
while(0)\

struct spinlock{
  int locked;
  char name[32];
};

typedef struct spinlock spinlock_t;

struct kvdb {
  FILE *fp; 
  char name[MAX_LEN]; 
  spinlock_t lock;
  pthread_mutex_t mutex;
};

typedef struct kvdb kvdb_t;

int kvdb_open(kvdb_t *db, const char *filename);
int kvdb_close(kvdb_t *db);
int kvdb_put(kvdb_t *db, const char *key, const char *value);
char *kvdb_get(kvdb_t *db, const char *key);
