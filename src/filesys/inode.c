#include "filesys/inode.h"
#include <list.h>
#include <debug.h>
#include <round.h>
#include <string.h>
#include "filesys/filesys.h"
#include "filesys/free-map.h"
#include "threads/malloc.h"

/* Identifies an inode. */
#define INODE_MAGIC 0x494e4f44

/* num of sectors poiner in indirect block */
#define INODE_DISK_INODE_NUM (BLOCK_SECTOR_SIZE/sizeof(block_sector_t))


/* On-disk inode.
   Must be exactly BLOCK_SECTOR_SIZE bytes long. */
struct inode_disk{
  block_sector_t self;            /* to find yourself */
  block_sector_t direct;          /* direct block */
  block_sector_t indirect;        /* indirect block */
  block_sector_t double_indirect; /* double_indirect block */
  off_t length;                   /* file size in bytes */
  unsigned magic;                 /* magic number */
  uint32_t unused[122];           /* not used */
};


/* Returns the number of sectors to allocate for an inode SIZE
   bytes long. */
static inline size_t bytes_to_sectors(off_t size) { 
  return DIV_ROUND_UP(size, BLOCK_SECTOR_SIZE); 
}


/* the zero block */
static const char zeros[BLOCK_SECTOR_SIZE];

/* for better allocate sector and release if failed. */
struct free_sector
{
  block_sector_t sector;
  struct list_elem elem;
};

void free_sector_init(struct list* sectors)
{
  list_init(sectors);
}

void free_sector_done(struct list* sectors)
{
  while(!list_empty(sectors))
  {
    struct list_elem* e=list_pop_front(sectors);
    struct free_sector* s=list_entry(e,struct free_sector,elem);
    free(s);
  }
}

void free_sector_reserve(struct list* sectors,block_sector_t sector)
{
  struct free_sector* free_sector=malloc(sizeof *free_sector);
  free_sector->sector=sector;
  list_push_back(sectors,&free_sector->elem);
}

void free_sector_release(struct list* sectors)
{
  struct list_elem* elem;
  for(elem=list_begin(sectors);elem!=list_end(sectors);elem=list_next(elem)){
    struct free_sector* free_sector=list_entry(elem,struct free_sector,elem);
    free_map_release(free_sector->sector,1);
  }
}

/* init the sector_block */
static void* sector_block_init1(void);
static void* sector_block_init(void);

static void* (*sector_block_initer)(void)=sector_block_init1;
static block_sector_t sector_init_block[INODE_DISK_INODE_NUM];

static void* sector_block_init1(void)
{
  for(size_t i=0;i<INODE_DISK_INODE_NUM;++i)
    sector_init_block[i]=BLOCK_SECTOR_ERROR;
  sector_block_initer=sector_block_init;
  return sector_block_initer();
}

static void* sector_block_init(void)
{
  return sector_init_block;
}


static block_sector_t find_sector(block_sector_t* sector,struct list* sectors,bool is_sector)
{
  if(*sector==BLOCK_SECTOR_ERROR){
    if(!free_map_allocate(1,sector))
      return BLOCK_SECTOR_ERROR;
    free_sector_reserve(sectors,*sector);
    if(is_sector)
      block_write(fs_device,*sector,sector_block_initer());
    else
      block_write(fs_device,*sector,zeros);
  }
  return *sector;
}

/* grow block version of find_sector_through_sectors */
static block_sector_t find_sector_through_sectors_fixed(struct inode_disk* disk_inode,size_t sectors)
{
  struct list new_sectors;
  free_sector_init(&new_sectors);
  block_sector_t res=BLOCK_SECTOR_ERROR;
  if(sectors<1){
    /* in the direct */
    if(find_sector(&disk_inode->direct,&new_sectors,false)==BLOCK_SECTOR_ERROR)
      goto FAIL_RET;
    res=disk_inode->direct;
  }else if(sectors<1+128){
    /* in the indirect */
    if(find_sector(&disk_inode->indirect,&new_sectors,true)==BLOCK_SECTOR_ERROR)
      goto FAIL_RET;
    block_sector_t buffer[INODE_DISK_INODE_NUM];
    block_read(fs_device,disk_inode->indirect,buffer);
    if(find_sector(&buffer[sectors-1],&new_sectors,false)==BLOCK_SECTOR_ERROR)
      goto FAIL_RET;
    block_write(fs_device,disk_inode->indirect,buffer);
    res=buffer[sectors-1];
  }else if(sectors<1+128+128*128){
    /* in the double_indirect */
    if(find_sector(&disk_inode->double_indirect,&new_sectors,true)==BLOCK_SECTOR_ERROR)
      goto FAIL_RET;
    block_sector_t buf1[INODE_DISK_INODE_NUM];
    block_sector_t buf2[INODE_DISK_INODE_NUM];
    block_read(fs_device,disk_inode->double_indirect,buf1);
    block_sector_t* indirect=&buf1[(sectors-129)/128];
    if(find_sector(indirect,&new_sectors,true)==BLOCK_SECTOR_ERROR)
      goto FAIL_RET;
    block_write(fs_device,disk_inode->double_indirect,buf1);
    block_read(fs_device,*indirect,buf2);
    block_sector_t* direct=&buf2[(sectors-129)%128];
    if(find_sector(direct,&new_sectors,false)==BLOCK_SECTOR_ERROR)
      goto FAIL_RET;
    block_write(fs_device,*indirect,buf2);
    res=*direct;
  }
  
  free_sector_done(&new_sectors);
  return res;
FAIL_RET:
  free_sector_release(&new_sectors);
  free_sector_done(&new_sectors);
  return BLOCK_SECTOR_ERROR;
}

/* get the inode chosen sector by sector_num */
static block_sector_t find_sector_through_sectors(struct inode_disk* disk_inode,size_t sectors)
{
  if(sectors<1){
    /* in the direct */
    if(disk_inode->direct==BLOCK_SECTOR_ERROR){
      return BLOCK_SECTOR_ERROR;
    }
    return disk_inode->direct;
  }else if(sectors<1+128){
    /* in the indirect */
    if(disk_inode->indirect==BLOCK_SECTOR_ERROR){
      return BLOCK_SECTOR_ERROR;
    }
    block_sector_t buffer[INODE_DISK_INODE_NUM];
    block_read(fs_device,disk_inode->indirect,buffer);
    return buffer[sectors-1];
  }else if(sectors<1+128+128*128){
    /* in the double_indirect */
    if(disk_inode->double_indirect==BLOCK_SECTOR_ERROR){
      return BLOCK_SECTOR_ERROR;
    }
    block_sector_t buf1[INODE_DISK_INODE_NUM];
    block_sector_t buf2[INODE_DISK_INODE_NUM];
    block_read(fs_device,disk_inode->double_indirect,buf1);
    block_sector_t indirect=buf1[(sectors-129)/128];
    block_read(fs_device,indirect,buf2);
    block_sector_t direct=buf2[(sectors-129)%128];
    return direct;
  }else{
    ASSERT(!"file size oversize");
  }
  return BLOCK_SECTOR_ERROR;
}

/* through the offset to find the sector */
static block_sector_t find_sector_through_offset(struct inode_disk* disk_inode,off_t offset)
{
  block_sector_t res=find_sector_through_sectors_fixed(disk_inode,offset/BLOCK_SECTOR_SIZE);
  if(res==BLOCK_SECTOR_ERROR)
    ASSERT(!"find_sector_through_sectors failed.")
  return res;
}


/* In-memory inode. */
struct inode {
  struct list_elem elem;  /* Element in inode list. */
  block_sector_t sector;  /* Sector number of disk location. */
  int open_cnt;           /* Number of openers. */
  bool removed;           /* True if deleted, false otherwise. */
  int deny_write_cnt;     /* 0: writes ok, >0: deny writes. */
  struct inode_disk data; /* Inode content. */
  bool isdir;             /* True if dir, false normal file */
};



/* Returns the block device sector that contains byte offset POS
   within INODE.
   Returns -1 if INODE does not contain data for a byte at offset
   POS. */
static block_sector_t byte_to_sector(const struct inode* inode, off_t pos) {
  ASSERT(inode != NULL);
  return find_sector_through_offset(&inode->data,pos);
}

/* List of open inodes, so that opening a single inode twice
   returns the same `struct inode'. */
static struct list open_inodes;

/* Initializes the inode module. */
void inode_init(void) { 
  list_init(&open_inodes); 
}


/* Initializes an inode with LENGTH bytes of data and
   writes the new inode to sector SECTOR on the file system
   device.
   Returns true if successful.
   Returns false if memory or disk allocation fails. */
bool inode_create(block_sector_t sector, off_t length) {
  struct inode_disk* disk_inode = NULL;
  bool success = false;
  struct list sectors;

  ASSERT(length >= 0);

  /* If this assertion fails, the inode structure is not exactly
     one sector in size, and you should fix that. */
  ASSERT(sizeof *disk_inode == BLOCK_SECTOR_SIZE);

  free_sector_init(&sectors);

  disk_inode = calloc(1, sizeof *disk_inode);
  if(disk_inode!=NULL){
    size_t num_of_sectors=bytes_to_sectors(length);
    ASSERT(num_of_sectors<1+128+128*128);
    disk_inode->self=sector;
    disk_inode->direct=BLOCK_SECTOR_ERROR;
    disk_inode->indirect=BLOCK_SECTOR_ERROR;
    disk_inode->double_indirect=BLOCK_SECTOR_ERROR;
    disk_inode->length=length;
    disk_inode->magic=INODE_MAGIC;


    /* allocate in direct */
    if(num_of_sectors>0){
      if(find_sector(&disk_inode->direct,&sectors,false)==BLOCK_SECTOR_ERROR)
        goto FAIL_RET;
      num_of_sectors-=1;
    }
  
    /* allocate in indirect */
    if(num_of_sectors>0){
      size_t need_sectors=num_of_sectors>128?128:num_of_sectors;
      num_of_sectors-=need_sectors;

      if(find_sector(&disk_inode->indirect,&sectors,true)==BLOCK_SECTOR_ERROR)
        goto FAIL_RET;

      block_sector_t directs[128];
      memcpy(directs,sector_block_initer(),BLOCK_SECTOR_SIZE);

      for(size_t i=0;i<need_sectors;++i)
      {
        if(find_sector(&directs[i],&sectors,false)==BLOCK_SECTOR_ERROR)
          goto FAIL_RET;
      }
      block_write(fs_device,disk_inode->indirect,directs);
    }

    /* allocate in double-indirect */
    if(num_of_sectors>0){
      if(find_sector(&disk_inode->double_indirect,&sectors,true)==BLOCK_SECTOR_ERROR)
        goto FAIL_RET;
      
      block_sector_t indirects[128];
      memcpy(indirects,sector_block_initer(),BLOCK_SECTOR_SIZE);
      block_sector_t directs[128];

      for(size_t idx_dd=0;idx_dd<128&&num_of_sectors>0;idx_dd++)
      {
        if(find_sector(&indirects[idx_dd],&sectors,true)==BLOCK_SECTOR_ERROR)
          goto FAIL_RET;
      
        memcpy(directs,sector_block_initer(),BLOCK_SECTOR_SIZE);
        for(size_t idx_id=0;idx_id<128&&num_of_sectors>0;idx_id++)
        {
          if(find_sector(&directs[idx_id],&sectors,false)==BLOCK_SECTOR_ERROR)
            goto FAIL_RET;
          num_of_sectors-=1;
        }
        block_write(fs_device,indirects[idx_dd],directs);
      }
      block_write(fs_device,disk_inode->double_indirect,indirects);
    }

    /* write in the inode */
    block_write(fs_device,sector,disk_inode);
    free(disk_inode);
  }
  
  free_sector_done(&sectors);
  return true;

FAIL_RET:
  free_sector_release(&sectors);
  free_sector_done(&sectors);
  return false;
}

/* Reads an inode from SECTOR
   and returns a `struct inode' that contains it.
   Returns a null pointer if memory allocation fails. */
struct inode* inode_open(block_sector_t sector,bool isdir) {
  struct list_elem* e;
  struct inode* inode;

  /* Check whether this inode is already open. */
  for (e = list_begin(&open_inodes); e != list_end(&open_inodes); e = list_next(e)) {
    inode = list_entry(e, struct inode, elem);
    if (inode->sector == sector) {
      inode_reopen(inode);
      return inode;
    }
  }

  /* Allocate memory. */
  inode = malloc(sizeof *inode);
  if (inode == NULL)
    return NULL;

  /* Initialize. */
  list_push_front(&open_inodes, &inode->elem);
  inode->sector = sector;
  inode->open_cnt = 1;
  inode->deny_write_cnt = 0;
  inode->removed = false;
  inode->isdir=isdir;
  block_read(fs_device, inode->sector, &inode->data);
  return inode;
}

/* Reopens and returns INODE. */
struct inode* inode_reopen(struct inode* inode) {
  if (inode != NULL)
    inode->open_cnt++;
  return inode;
}

/* Returns INODE's inode number. */
block_sector_t inode_get_inumber(const struct inode* inode) { 
  return inode->sector; 
}

/* mycode: */
void inode_deallocate_blocks(struct inode* inode)
{
  ASSERT(inode->removed);

  free_map_release(inode->sector,1);
  struct inode_disk* disk_inode=&inode->data;
  /* release the direct */
  if(disk_inode->direct!=BLOCK_SECTOR_ERROR){
    free_map_release(disk_inode->direct,1);
  }
  /* release the indirect */
  if(disk_inode->indirect!=BLOCK_SECTOR_ERROR){
    block_sector_t directs[128];
    block_read(fs_device,disk_inode->indirect,directs);
    for(size_t idx_id=0;idx_id<128;++idx_id){
      if(directs[idx_id]!=BLOCK_SECTOR_ERROR){
        free_map_release(directs[idx_id],1);
      }
    }
    free_map_release(disk_inode->indirect,1);
  }
  /* release the double_indirect */
  if(disk_inode->double_indirect!=BLOCK_SECTOR_ERROR){
    block_sector_t indirects[128];
    block_sector_t directs[128];
    block_read(fs_device,disk_inode->double_indirect,indirects);
    for(size_t idx_dd=0;idx_dd<128;idx_dd++){
      if(indirects[idx_dd]!=BLOCK_SECTOR_ERROR){
        block_read(fs_device,indirects[idx_dd],directs);
        for(size_t idx_id=0;idx_id<128;idx_id++){
          if(directs[idx_id]!=BLOCK_SECTOR_ERROR){
            free_map_release(directs[idx_id],1);
          }
        }
        free_map_release(indirects[idx_dd],1);
      }
    }
    free_map_release(disk_inode->double_indirect,1);
  }

}

/* Closes INODE and writes it to disk.
   If this was the last reference to INODE, frees its memory.
   If INODE was also a removed inode, frees its blocks. */
void inode_close(struct inode* inode) {
  /* Ignore null pointer. */
  if (inode == NULL)
    return;

  /* Release resources if this was the last opener. */
  if (--inode->open_cnt == 0) {
    /* Remove from inode list and release lock. */
    list_remove(&inode->elem);

    /* Deallocate blocks if removed. */
    if (inode->removed) {
      inode_deallocate_blocks(inode);    
    }

    free(inode);
  }
}

/* Marks INODE to be deleted when it is closed by the last caller who
   has it open. */
void inode_remove(struct inode* inode) {
  ASSERT(inode != NULL);
  inode->removed = true;
}

// 8MB
#define INODE_MAX_LENGTH (0x100000000)

static void inode_extend(struct inode* inode,size_t new_len)
{
  if( new_len<INODE_MAX_LENGTH)
    inode->data.length=new_len;
  else
    ASSERT(!"inode_extend over the max_length");
  block_write(fs_device,inode->data.self,&inode->data);
}

/* Reads SIZE bytes from INODE into BUFFER, starting at position OFFSET.
   Returns the number of bytes actually read, which may be less
   than SIZE if an error occurs or end of file is reached. */
off_t inode_read_at(struct inode* inode, void* buffer_, off_t size, off_t offset) {
  uint8_t* buffer = buffer_;
  off_t bytes_read = 0;
  uint8_t* bounce = NULL;

  if(inode_length(inode)<offset+size)
    inode_extend(inode,offset+size);

  while (size > 0) {
    /* Disk sector to read, starting byte offset within sector. */
    block_sector_t sector_idx = byte_to_sector(inode, offset);
    int sector_ofs = offset % BLOCK_SECTOR_SIZE;

    /* Bytes left in inode, bytes left in sector, lesser of the two. */
    off_t inode_left = inode_length(inode) - offset;
    int sector_left = BLOCK_SECTOR_SIZE - sector_ofs;
    int min_left = inode_left < sector_left ? inode_left : sector_left;

    /* Number of bytes to actually copy out of this sector. */
    int chunk_size = size < min_left ? size : min_left;
    if (chunk_size <= 0)
      break;

    if (sector_ofs == 0 && chunk_size == BLOCK_SECTOR_SIZE) {
      /* Read full sector directly into caller's buffer. */
      block_read(fs_device, sector_idx, buffer + bytes_read);
    } else {
      /* Read sector into bounce buffer, then partially copy
             into caller's buffer. */
      if (bounce == NULL) {
        bounce = malloc(BLOCK_SECTOR_SIZE);
        if (bounce == NULL)
          break;
      }
      block_read(fs_device, sector_idx, bounce);
      memcpy(buffer + bytes_read, bounce + sector_ofs, chunk_size);
    }

    /* Advance. */
    size -= chunk_size;
    offset += chunk_size;
    bytes_read += chunk_size;
  }
  free(bounce);

  return bytes_read;
}

/* Writes SIZE bytes from BUFFER into INODE, starting at OFFSET.
   Returns the number of bytes actually written, which may be
   less than SIZE if end of file is reached or an error occurs.
   (Normally a write at end of file would extend the inode, but
   growth is not yet implemented.) */
off_t inode_write_at(struct inode* inode, const void* buffer_, off_t size, off_t offset) {
  const uint8_t* buffer = buffer_;
  off_t bytes_written = 0;
  uint8_t* bounce = NULL;

  if (inode->deny_write_cnt)
    return 0;

  if(inode_length(inode)<offset+size)
    inode_extend(inode,offset+size);
  
  while (size > 0) {
    /* Sector to write, starting byte offset within sector. */
    block_sector_t sector_idx = byte_to_sector(inode, offset);
    int sector_ofs = offset % BLOCK_SECTOR_SIZE;

    /* Bytes left in inode, bytes left in sector, lesser of the two. */
    off_t inode_left = inode_length(inode) - offset;
    int sector_left = BLOCK_SECTOR_SIZE - sector_ofs;
    int min_left = inode_left < sector_left ? inode_left : sector_left;

    /* Number of bytes to actually write into this sector. */
    int chunk_size = size < min_left ? size : min_left;
    if (chunk_size <= 0)
      break;

    if (sector_ofs == 0 && chunk_size == BLOCK_SECTOR_SIZE) {
      /* Write full sector directly to disk. */
      block_write(fs_device, sector_idx, buffer + bytes_written);
    } else {
      /* We need a bounce buffer. */
      if (bounce == NULL) {
        bounce = malloc(BLOCK_SECTOR_SIZE);
        if (bounce == NULL)
          break;
      }

      /* If the sector contains data before or after the chunk
             we're writing, then we need to read in the sector
             first.  Otherwise we start with a sector of all zeros. */
      if (sector_ofs > 0 || chunk_size < sector_left)
        block_read(fs_device, sector_idx, bounce);
      else
        memset(bounce, 0, BLOCK_SECTOR_SIZE);
      memcpy(bounce + sector_ofs, buffer + bytes_written, chunk_size);
      block_write(fs_device, sector_idx, bounce);
    }

    /* Advance. */
    size -= chunk_size;
    offset += chunk_size;
    bytes_written += chunk_size;
  }
  free(bounce);

  return bytes_written;
}

/* Disables writes to INODE.
   May be called at most once per inode opener. */
void inode_deny_write(struct inode* inode) {
  inode->deny_write_cnt++;
  ASSERT(inode->deny_write_cnt <= inode->open_cnt);
}

/* Re-enables writes to INODE.
   Must be called once by each inode opener who has called
   inode_deny_write() on the inode, before closing the inode. */
void inode_allow_write(struct inode* inode) {
  ASSERT(inode->deny_write_cnt > 0);
  ASSERT(inode->deny_write_cnt <= inode->open_cnt);
  inode->deny_write_cnt--;
}

/* Returns the length, in bytes, of INODE's data. */
off_t inode_length(const struct inode* inode) { 
  return inode->data.length; 
}

int inode_open_cnt(struct inode* inode){
  return inode->open_cnt;
}

bool inode_get_isdir(const struct inode* inode){
  return inode->isdir;
}

void inode_set_isdir(struct inode* inode,bool isdir){
  inode->isdir=isdir;
}


