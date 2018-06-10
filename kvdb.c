#include "kvdb.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <sys/file.h>
#include <unistd.h>
//static char key[LENGTH];
//static char value[LENGTH];
//static char input[LENGTH];

static void RDLCK_init(int fd){
  struct flock locked;
  locked.l_whence=SEEK_SET;
  locked.l_len=0;
  locked.l_start=0;
  locked.l_type=F_RDLCK;
  locked.l_pid=getpid();
  fcntl(fd,F_SETLK,&locked);
}

static void WRLCK_init(int fd){
  struct flock locked;
  locked.l_whence=SEEK_SET;
  locked.l_len=0;
  locked.l_start=0;
  locked.l_type=F_WRLCK;
  locked.l_pid=getpid();
  fcntl(fd,F_SETLKW,&locked);
}

static void FILE_unlock(int fd){
  struct flock locked;
  locked.l_whence=SEEK_SET;
  locked.l_len=0;
  locked.l_start=0;
  locked.l_type=F_UNLCK;
  locked.l_pid=getpid();
  fcntl(fd,F_SETLKW,&locked);
}

static int unsafe_open(kvdb_t *db,const char *filename){
   strcpy(db->name,filename);
   if((db->fp=fopen(filename,"a+"))==NULL){
      EXIT_ERR("unsafe open");
   }
   RDLCK_init(db->fp->_fileno);
   WRLCK_init(db->fp->_fileno);
   return 0;
}

int kvdb_open(kvdb_t *db, const char *filename) {
  // BUG: no error checking
  //db->fp = fopen(filename, "a+");
  //return 0 
 
  pthread_mutex_init(&db->mutex,NULL);
  if(pthread_mutex_lock(&db->mutex)!=0){
      EXIT_ERR("open lock");
  }
  int ret=unsafe_open(db,filename);
  if(pthread_mutex_unlock(&db->mutex)!=0){
      EXIT_ERR("open unlock");
  }
  return ret;
}

static unsafe_close(kvdb_t *db){
  unlock(db->fp->_fileno);
  if(fclose(db->fp)!=0){
     EXIT_ERR("unsafe close");
  }
  return 0;
}

int kvdb_close(kvdb_t *db) {
  // BUG: no error checking
  //fclose(db->fp);
  //return 0;

  if(pthread_mutex_lock(&db->mutex)!=0){
     EXIT_ERR("close lock");
  }
  int ret=unsafe_close(db);
  if(pthread_mutex_unlock(&db->mutex)!=0){
     EXIT_ERR("close unlock");
  }
  return ret;
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
