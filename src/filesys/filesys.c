#include "filesys/filesys.h"
#include <debug.h>
#include <stdio.h>
#include <string.h>
#include "filesys/file.h"
#include "filesys/free-map.h"
#include "filesys/inode.h"
#include "filesys/directory.h"
#include "userprog/process.h"

/* Partition that contains the file system. */
struct block* fs_device;


static void do_format(void);

/* Initializes the file system module.
   If FORMAT is true, reformats the file system. */
void filesys_init(bool format) {
  fs_device = block_get_role(BLOCK_FILESYS);
  if (fs_device == NULL)
    PANIC("No file system device found, can't initialize file system.");

  inode_init();
  free_map_init();

  if (format)
    do_format();

  free_map_open();
  
}

/* Shuts down the file system module, writing any unwritten data
   to disk. */
void filesys_done(void) {
  blk_cache_done();
  free_map_close(); 
}

static struct dir* climb_path(struct dir* dir,const char* path)
{
  struct inode* next;
  char* find=strchr(path,'/');
  if(find==NULL){
    if(*path=='\0')
      return dir;
    dir_lookup(dir,path,&next);
    dir_close(dir);
    return dir_open(next);
  }
  *find='\0';
  dir_lookup(dir,path,&next);
  if(next==NULL)
    return NULL;
  dir_close(dir);
  return climb_path(dir_open(next),find+1);
}

static struct dir* arrive_dir(char* path)
{
  struct dir* dir=NULL;
  struct dir* old=NULL;
  if(path[0]=='/'){
    /* absolute path */
    old=dir_open_root();
    dir=climb_path(old,path+1);
  }else{
    /* relative path */
    old=dir_reopen(get_cwd());
    dir=climb_path(old,path);
  }
  return dir;
}

static void path_extract(const char* name,char path[128],char inode_name[NAME_MAX+1])
{
  const int name_len=strlen(name);
  strlcpy(path,name,name_len+1);
  char* find=strrchr(path,'/');
  if(find==NULL){
    // like "a"
    strlcpy(inode_name,path,NAME_MAX+1);
    path[0]='\0';
  }else{
    // like "/b" "/"
    strlcpy(inode_name,find+1,NAME_MAX+1);
    *(find+1)='\0';
  }
}

/* Creates a file named NAME with the given INITIAL_SIZE.
   Returns true if successful, false otherwise.
   Fails if a file named NAME already exists,
   or if internal memory allocation fails. */
bool filesys_create(const char* name, off_t initial_size,bool isdir) {
  block_sector_t new_sector;
  char path[128];
  char inode_name[NAME_MAX+1];

  if(*name=='\0')
    return false;

  path_extract(name,path,inode_name);

  if(*inode_name=='\0')
    return false;



  if(!free_map_allocate(1,&new_sector))
    return false;

  if(!inode_create(new_sector,initial_size))
    goto false_batch;

  struct dir* dirp=get_cwd();
  if(!(dirp=arrive_dir(path)))
    goto false_batch;

  if(isdir){
    struct dir* new_dir=dir_open(inode_open(new_sector,true));
    dir_add(new_dir,".",new_sector,true);
    dir_add(new_dir,"..",inode_get_inumber(dir_get_inode(dirp)),true);
    dir_close(new_dir);
  }

  if(!dir_add((struct dir*)dirp,inode_name,new_sector,isdir))
    goto false_batch;


  return true;
false_batch:
  free_map_release(new_sector,1);
  return false;
}

/* Opens the file with the given NAME.
   Returns the new file if successful or a null pointer
   otherwise.
   Fails if no file named NAME exists,
   or if an internal memory allocation fails. */
struct file* filesys_open(const char* name) {
  char path[128];
  char inode_name[NAME_MAX+1];
  struct inode* inode;

  if(*name=='\0')
    return NULL;

  path_extract(name,path,inode_name);

  if(*inode_name=='\0'){
    if(!strcmp(path,"/"))
      return dir_open_root();
    return NULL;
  }
  
  struct dir* dirp=get_cwd();
  if(!(dirp=arrive_dir(path)))
      return NULL;

  dir_lookup(dirp,inode_name,&inode);

  return inode_get_isdir(inode)?(struct file*)dir_open(inode):file_open(inode);

}

/* Deletes the file named NAME.
   Returns true if successful, false on failure.
   Fails if no file named NAME exists,
   or if an internal memory allocation fails. */
bool filesys_remove(const char* name) {
  char path[128];
  char inode_name[NAME_MAX+1];
  struct inode* inode;

  if(*name=='\0')
    return false;
 
  path_extract(name,path,inode_name);

  if(*inode_name=='\0')
    return false;

  struct dir* dirp=get_cwd();
  if(!(dirp=arrive_dir(path)))
    return false;

  bool success= dirp!=NULL && dir_remove(dirp,inode_name);

  return success;
}

/* Formats the file system. */
static void do_format(void) {
  printf("Formatting file system...");
  free_map_create();
  if (!dir_create(ROOT_DIR_SECTOR, 16, ROOT_DIR_SECTOR))
    PANIC("root directory creation failed");
  free_map_close();
  printf("done.\n");
}


bool filesys_chdir(const char* path)
{
  struct dir* dir=arrive_dir(path);
  if(dir==NULL)
    return false;
  dir_close(get_cwd());
  set_cwd(dir);
  return true;
}

bool filesys_mkdir(const char* p)
{
  return filesys_create(p,(16+2)*dir_entry_sizeof(),true);
}




