#include "filesys/directory.h"
#include <stdio.h>
#include <string.h>
#include <list.h>
#include "filesys/filesys.h"
#include "filesys/inode.h"
#include "threads/malloc.h"

/* A directory. */
struct dir {
  struct inode* inode; /* Backing store. */
  off_t pos;           /* Current position. */
};

/* A single directory entry. */
struct dir_entry {
  block_sector_t inode_sector; /* Sector number of header. */
  char name[NAME_MAX + 1];     /* Null terminated file name. */
  bool in_use;                 /* In use or free? */
  bool isdir;                  /* If the dir */
};

size_t dir_entry_sizeof(void)
{
  return sizeof(struct dir_entry);
}

/* Creates a directory with space for ENTRY_CNT entries in the
   given SECTOR.  Returns true if successful, false on failure. */
bool dir_create(block_sector_t sector, size_t entry_cnt,block_sector_t father) {
  entry_cnt+=2;
  ASSERT(entry_cnt+2<DIR_ENTRY_MAX);

  if(inode_create(sector,entry_cnt*sizeof(struct dir_entry))){
    struct dir* dir=dir_open(inode_open(sector,true));
    dir_add(dir,".",sector,true);
    dir_add(dir,"..",sector,true);
    dir_close(dir);
    return true;
  }
  return false;
}

/* Opens and returns the directory for the given INODE, of which
   it takes ownership.  Returns a null pointer on failure. */
struct dir* dir_open(struct inode* inode) {
  struct dir* dir = calloc(1, sizeof *dir);
  if (inode != NULL && dir != NULL) {
    dir->inode = inode;
    dir->pos = 0;
    return dir;
  } else {
    inode_close(inode);
    free(dir);
    return NULL;
  }
}

/* Opens the root directory and returns a directory for it.
   Return true if successful, false on failure. */
struct dir* dir_open_root(void) {
  return dir_open(inode_open(ROOT_DIR_SECTOR,true));
}

/* Opens and returns a new directory for the same inode as DIR.
   Returns a null pointer on failure. */
struct dir* dir_reopen(struct dir* dir) {
  return dir_open(inode_reopen(dir->inode));
}

/* Destroys DIR and frees associated resources. */
void dir_close(struct dir* dir) {
  if (dir != NULL) {
    inode_close(dir->inode);
    free(dir);
  }
}

/* Returns the inode encapsulated by DIR. */
struct inode* dir_get_inode(struct dir* dir) {
  return dir->inode;
}

/* Searches DIR for a file with the given NAME.
   If successful, returns true, sets *EP to the directory entry
   if EP is non-null, and sets *OFSP to the byte offset of the
   directory entry if OFSP is non-null.
   otherwise, returns false and ignores EP and OFSP. */
static bool lookup(const struct dir* dir, const char* name, struct dir_entry* ep, off_t* ofsp) {
  struct dir_entry e;

  ASSERT(dir != NULL);
  ASSERT(name != NULL);

  const size_t step=sizeof(struct dir_entry);
  for(size_t i=0;i<DIR_ENTRY_MAX&&inode_read_at(dir->inode,&e,sizeof e,i*step)==sizeof e; ++i){
    if(e.in_use&&strcmp(name,e.name)==0){
      if(ep!=NULL)
        *ep=e;
      if(ofsp!=NULL)
        *ofsp=i*step;
      return true;
    }
  }
  return false;
}

/* Searches DIR for a file with the given NAME
   and returns true if one exists, false otherwise.
   On success, sets *INODE to an inode for the file, otherwise to
   a null pointer.  The caller must close *INODE. */
bool dir_lookup(const struct dir* dir, const char* name, struct inode** inode) {
  struct dir_entry e;

  ASSERT(dir != NULL);
  ASSERT(name != NULL);

  if (lookup(dir, name, &e, NULL))
    *inode = inode_open(e.inode_sector,e.isdir);
  else
    *inode = NULL;

  return *inode != NULL;
}

/* Adds a file named NAME to DIR, which must not already contain a
   file by that name.  The file's inode is in sector
   INODE_SECTOR.
   Returns true if successful, false on failure.
   Fails if NAME is invalid (i.e. too long) or a disk or memory
   error occurs. */
bool dir_add(struct dir* dir, const char* name, block_sector_t inode_sector,bool isdir) {
  struct dir_entry e;
  bool success = false;

  ASSERT(dir != NULL);
  ASSERT(name != NULL);

  /* Check NAME for validity. */
  if (*name == '\0' || strlen(name) > NAME_MAX)
    return false;

  /* Check that NAME is not in use. */
  if (lookup(dir, name, NULL, NULL))
    goto done;

  /* Set OFS to offset of free slot.
     If there are no free slots, then it will be set to the
     current end-of-file.

     inode_read_at() will only return a short read at end of file.
     Otherwise, we'd need to verify that we didn't get a short
     read due to something intermittent such as low memory. */
  const size_t step=sizeof(struct dir_entry);
  size_t i=0;
  for(;i<DIR_ENTRY_MAX&&inode_read_at(dir->inode,&e,sizeof e,i*step)==sizeof e; ++i){
    if(!e.in_use){
      /* Write slot. */
      e.in_use = true;
      strlcpy(e.name, name, sizeof e.name);
      e.inode_sector = inode_sector;
      e.isdir=isdir;
      success = inode_write_at(dir->inode, &e, sizeof e, i*step) == sizeof e;
      goto done;
    }
  }

  char error_msg[32];
  snprintf(error_msg,sizeof error_msg,"dir is too full to add dir_entry[%llu]",i);
  ASSERT(!error_msg);

done:
  return success;
}

/* Removes any entry for NAME in DIR.
   Returns true if successful, false on failure,
   which occurs only if there is no file with the given NAME. */
bool dir_remove(struct dir* dir, const char* name) {
  struct dir_entry e;
  struct inode* inode = NULL;
  bool success = false;
  off_t ofs;

  ASSERT(dir != NULL);
  ASSERT(name != NULL);

  /* Find directory entry. */
  if (!lookup(dir, name, &e, &ofs))
    goto done;

  /* Open inode. */
  inode = inode_open(e.inode_sector,e.isdir);
  if (inode == NULL)
    goto done;

  /* if as a dir, can be deleted */
  if(e.isdir){
    struct dir* as_dir=dir_open(inode);
    if(!dir_deletable(as_dir)){
      dir_close(as_dir);
      goto done;
    }
    dir_close(as_dir);
  }

  /* Erase directory entry. */
  e.in_use = false;
  if (inode_write_at(dir->inode, &e, sizeof e, ofs) != sizeof e)
    goto done;

  /* Remove inode. */
  inode_remove(inode);
  success = true;

done:
  inode_close(inode);
  return success;
}

/* Reads the next directory entry in DIR and stores the name in
   NAME.  Returns true if successful, false if the directory
   contains no more entries. */
bool dir_readdir(struct dir* dir, char name[NAME_MAX + 1]) {
  struct dir_entry e;
  size_t i=dir->pos/sizeof(e);

  for(;i<DIR_ENTRY_MAX&&inode_read_at(dir->inode,&e,sizeof e,dir->pos) == sizeof e;++i)
  {
    dir->pos += sizeof e;
    if (e.in_use) {
      if(!strcmp(e.name,".")||!strcmp(e.name,".."))
        continue;
      strlcpy(name, e.name, NAME_MAX + 1);
      return true;
    }
  }

  return false;
}

/* check if the dir can be deleted */
bool dir_deletable(struct dir* dir)
{
  if(dir==NULL)
    return false;
  
  struct dir* root=dir_open_root();
  if(dir->inode==root->inode){
    dir_close(root);
    return false;
  }
  dir_close(root);

  struct dir* cwd=get_cwd();
  if(dir->inode==cwd->inode)
    return false;

  const size_t step=sizeof(struct dir_entry);
  struct dir_entry e;
  size_t dir_size=0;
  for(size_t i=0;i<DIR_ENTRY_MAX&&inode_read_at(dir->inode,&e,sizeof e,i*step)==sizeof e; ++i){
    if(e.in_use){
      dir_size++;
    }
  }
  if(dir_size>2)
    return false;

  return true;
}



