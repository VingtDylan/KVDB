#include "kvdb.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

//static char key[LENGTH];
//static char value[LENGTH];
//static char input[LENGTH];

static int unsafe_open(kvdb_t *db,const char *filename){
   strcpy(db->name,filename);
   if((db->fp=fopen(filename,"a+"))=NULL){
      EXIT_ERR("unsafe open");
   }
 
 
}

int kvdb_open(kvdb_t *db, const char *filename) {
  // BUG: no error checking
  //db->fp = fopen(filename, "a+");
  //return 0 
 
  pthread_mutex_init(&db->mutex,NULL);
  if(pthread_mutex_lock(&db->mutex)!=0){
      EXIT_ERR("locked");
  }
  int ret=unsafe_open(db,filename);
  if(pthread_mutex_unlock(&db->mutex)!=0){
      EXIT_ERR("unlocked");
  }
  return ret;
}

int kvdb_close(kvdb_t *db) {
  // BUG: no error checking
  fclose(db->fp);
  return 0;
}

int kvdb_put(kvdb_t *db, const char *key, const char *value) {
  // BUG: no error checking
  fseek(db->fp, 0, SEEK_END);
  fwrite(key,   1, strlen(key),   db->fp);
  fwrite("\n",  1, 1,             db->fp);
  fwrite(value, 1, strlen(value), db->fp);
  fwrite("\n",  1, 1,             db->fp);
  return 0;
}

char *kvdb_get(kvdb_t *db, const char *key) {
  // BUG: no error checking
  static char _key[1 << 20], _value[1 << 20];
  char *ret = NULL;
  fseek(db->fp, 0, SEEK_SET);
  while (1) {
    if (!fgets(_key, sizeof(_key), db->fp)) break;
    if (!fgets(_value, sizeof(_value), db->fp)) break;
    _key[strlen(_key) - 1] = '\0';
    _value[strlen(_value) - 1] = '\0';
    if (strcmp(key, _key) == 0) {
      if (!ret) free(ret);
      ret = strdup(_value);
    }
  }
  return ret;
}
