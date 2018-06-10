#include "kvdb.h"
#include <stdlib.h>

#define M 100

int main() {
  kvdb_t db;
  const char *key = "operating-systems";
  const char *test= "case" ;
  char *value;

  kvdb_open(&db, "a.db");
  kvdb_put(&db, key, "three-easy-pieces");
  
  char str[M] = {0};  
  int i = 0;  
  char symbol[4] = {'|','/','-','\\'};  
  for(i = 0; i<M; ++i){  
      int c = i%5;  
      switch(c){  
            case 0:printf(RED);    break;  
            case 1:printf(GREEN);  break;  
            case 2:printf(BLUE);   break;  
            case 3:printf(YELLOW); break;  
            case 4:printf(BROWN);  break;  
     }  
     kvdb_put(&db,test,str);
     str[i] = symbol[c];
     str[i+1] = '\0';  
  }  
  value = kvdb_get(&db, key);
  printf("[%s]: [%s]\n", key, value);
  value = kvdb_get(&db, test);
  printf("[%s]: [%s]\n", test, value);
  free(value);
  return 0;
}

