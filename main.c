#include "kvdb.h"
#include <stdlib.h>

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
  while(1){  
      if(i==M-1) break;
      int c = i%5;  
      switch(c){  
            case 0:printf(RED);    break;  
            case 1:printf(GREEN);  break;  
            case 2:printf(BLUE);   break;  
            case 3:printf(YELLOW); break;  
            case 4:printf(BROWN);  break;  
     }  
     kvdb_put(&db,test,str);
     printf("tmpstr:%s\n",str);
     str[i] = symbol[c];
     str[i+1] = '\0';  
     i++;
  }  
  value = kvdb_get(&db, key);
  printf(PRUPLE"[%s]: [%s]\n"NONE, key, value);
  value = kvdb_get(&db, test);
  printf(CYAN"[%s]: [%s]\n"NONE, test, value);
  free(value);
  return 0;
}

