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

/* because the sector_0 has been used */
#define BLOCK_SECTOR_ERROR 0
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

/* get the inode chosen sector by sector_num */
static block_sector_t find_sector_through_sectors(struct inode_disk* disk_inode,size_t sectors)
{
  if(sectors<=1){
    /* in the direct */
    if(disk_inode->direct==BLOCK_SECTOR_ERROR){
      return BLOCK_SECTOR_ERROR;
    }
    return disk_inode->direct;
  }else if(sectors<=1+128){
    /* in the indirect */
    if(disk_inode->indirect==BLOCK_SECTOR_ERROR){
      return BLOCK_SECTOR_ERROR;
    }
    block_sector_t buffer[INODE_DISK_INODE_NUM];
    block_read(fs_device,disk_inode->indirect,buffer);
    return buffer[sectors-1];
  }else if(sectors<=1+128+128*128){
    /* in the double_indirect */
    if(disk_inode->double_indirect==BLOCK_SECTOR_ERROR){
      return BLOCK_SECTOR_ERROR;
    }
    block_sector_t* buf1=malloc(sizeof(*buf1)*INODE_DISK_INODE_NUM);
    block_sector_t* buf2=malloc(sizeof(*buf2)*INODE_DISK_INODE_NUM);
    block_read(fs_device,disk_inode->double_indirect,buf1);
    block_sector_t indirect=buf1[(sectors-129)/(128*128)];
    block_read(fs_device,indirect,buf2);
    block_sector_t direct=buf2[(sectors-129)/128];
    free(buf1);
    free(buf2);
    return direct;
  }else{
    ASSERT(!"file size oversize");
  }
  return BLOCK_SECTOR_ERROR;
}
/* through the offset to find the sector */
static block_sector_t find_sector_through_offset(struct inode_disk* disk_inode,off_t offset)
{
  return find_sector_through_sectors(disk_inode,bytes_to_sectors(offset));
}

/* In-memory inode. */
struct inode {
  struct list_elem elem;  /* Element in inode list. */
  block_sector_t sector;  /* Sector number of disk location. */
  int open_cnt;           /* Number of openers. */
  bool removed;           /* True if deleted, false otherwise. */
  int deny_write_cnt;     /* 0: writes ok, >0: deny writes. */
  struct inode_disk data; /* Inode content. */
};

#define BLK_CACHE_SIZE 64

struct blk_cache_entry{
  uint8_t disk_data[BLOCK_SECTOR_SIZE]; /* block_size */
  block_sector_t sector; /* sector identify */
  bool is_dirty; /* the sign for write back */
  bool is_used; /* the sign for NRU */
};

struct blk_cache{
  struct blk_cache_entry entrys[BLK_CACHE_SIZE]; /* datas */
  int size;
  int nru_ptr; /* ptr for NRU */
};

/* the disk_cache:
    64 disk block
 * */
struct blk_cache blk_cache;

void blk_cache_init(void);
int blk_cache_size(void);
void blk_cache_nru_ptr_move(void);
struct blk_cache_entry* blk_cache_held(block_sector_t sector);
void blk_cache_read(struct block* block,block_sector_t sector,void* buffer);
void blk_cache_write(struct block* block,block_sector_t sector,void* buffer);
struct blk_cache_entry* blk_cache_in(struct block* block,block_sector_t sector);
struct blk_cache_entry* blk_cache_out(struct block* block);

/*cache init */
void blk_cache_init(void)
{
  for(int i=0;i<BLK_CACHE_SIZE;++i){
    struct blk_cache_entry* entry=&blk_cache.entrys[i];
    entry->is_dirty=false;
    entry->is_used=false;
  }
  blk_cache.size=0;
  blk_cache.nru_ptr=0;
}

void blk_cache_done(void)
{
  /* update all the dirty block */
  for(int i=0;i<BLK_CACHE_SIZE;++i){
    struct blk_cache_entry* entry=&blk_cache.entrys[i];
    if(entry->is_used&&entry->is_dirty){
      blk_cache_write(fs_device,entry->sector,entry->disk_data);
    }
  }
}

int blk_cache_size(void)
{
  return blk_cache.size;
}

void blk_cache_nru_ptr_move(void)
{
  blk_cache.nru_ptr=(blk_cache.nru_ptr+1)%BLK_CACHE_SIZE;
}

/* is the cache has held the block */
struct blk_cache_entry* blk_cache_held(block_sector_t sector)
{
  for(int i=0;i<blk_cache.size;++i){
    if(blk_cache.entrys[i].sector==sector){
      &blk_cache.entrys[i];
    }
  }
  return NULL;
}

/* indirect layer for reading */
void blk_cache_read(struct block* block,block_sector_t sector,void* buffer)
{
  struct blk_cache_entry* entry;
  /* 1. if the sector already in cache */
  if((entry=blk_cache_held(sector))==NULL){
    /* 2. not in cache,and cache in it */
    entry=blk_cache_in(block,sector);
  }
  ASSERT(entry!=NULL);

  /* 3. read from cache */
  memcpy(buffer,entry->disk_data,BLOCK_SECTOR_SIZE);
}

/* indirect layer for writing */
void blk_cache_write(struct block* block, block_sector_t sector,void* buffer)
{
  struct blk_cache_entry* entry;
  /* 1. if the sector already in cache */
  if((entry=blk_cache_held(sector))==NULL){
    /* 2. not in cache,and cache in it */
    entry=blk_cache_in(block,sector);
  }
  ASSERT(entry!=NULL);

  /* 3. write to cache */
  memcpy(entry->disk_data,buffer,BLOCK_SECTOR_SIZE);
  entry->is_dirty=true;
}

/* let block cache in */
struct blk_cache_entry* blk_cache_in(struct block* block,block_sector_t sector)
{
  ASSERT(!blk_cache_held(sector));

  struct blk_cache_entry* entry;
  /* 1.check if the cache full */
  if(blk_cache.size==BLK_CACHE_SIZE){
    /* 2.evict the old block */
    entry=blk_cache_out(block);
  }else{
    /* 3.get next free cache */
    entry=&blk_cache.entrys[blk_cache.size++];
  }
  ASSERT(entry!=NULL);

  entry->sector=sector;
  entry->is_used=true;
  block_raw_read(block,entry->sector,entry->disk_data);

  return entry;
}

/* evict the old block using the NRU */
struct blk_cache_entry* blk_cache_out(struct block* block)
{
  /* hand circuit */
  while(blk_cache.entrys[blk_cache.nru_ptr].is_used==true){
    blk_cache.entrys[blk_cache.nru_ptr].is_used=false;
    blk_cache_nru_ptr_move();
    
  }
  struct blk_cache_entry* entry=&blk_cache.entrys[blk_cache.nru_ptr];

  /* write back */
  if(entry->is_dirty){
    block_raw_write(block,entry->sector,entry->disk_data);
  }

  entry->is_dirty=false;
  entry->is_used=false;

  return entry;
}


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
  blk_cache_init();
}

/* the zero block */
static const char zeros[BLOCK_SECTOR_SIZE];

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
    disk_inode->self=BLOCK_SECTOR_ERROR;
    disk_inode->direct=BLOCK_SECTOR_ERROR;
    disk_inode->indirect=BLOCK_SECTOR_ERROR;
    disk_inode->double_indirect=BLOCK_SECTOR_ERROR;
    disk_inode->length=length;
    disk_inode->magic=INODE_MAGIC;


    /* allocate in direct */
    if(num_of_sectors>0){
      success=free_map_allocate(1,&disk_inode->direct);
      if(!success){
        goto FAIL_RET;
      }
      free_sector_reserve(&sectors,disk_inode->direct);
      block_write(fs_device,disk_inode->direct,zeros);
      num_of_sectors-=1;
    }
  
    /* allocate in indirect */
    if(num_of_sectors>0){
      size_t need_sectors=num_of_sectors>128?128:num_of_sectors;
      num_of_sectors-=need_sectors;

      success=free_map_allocate(1,&disk_inode->indirect);
      if(!success){
        goto FAIL_RET;
      }
      free_sector_reserve(&sectors,disk_inode->indirect);
      block_sector_t directs[128];
      memcpy(directs,zeros,BLOCK_SECTOR_SIZE);

      for(size_t i=0;i<need_sectors;++i)
      {
        success=free_map_allocate(1,&directs[i]);
        if(!success){
          goto FAIL_RET;
        }
        free_sector_reserve(&sectors,directs[i]);
        block_write(fs_device,directs[i],zeros);
        num_of_sectors-=1;
      }
      block_write(fs_device,disk_inode->indirect,directs);
    }

    /* allocate in double-indirect */
    if(num_of_sectors>0){
      success=free_map_allocate(1,&disk_inode->double_indirect);
      if(!success){
        goto FAIL_RET;
      }
      free_sector_reserve(&sectors,disk_inode->double_indirect);
      block_sector_t indirects[128];
      memcpy(indirects,zeros,BLOCK_SECTOR_SIZE);
      block_sector_t directs[128];
      memcpy(directs,zeros,BLOCK_SECTOR_SIZE);

      for(size_t idx_dd=0;idx_dd<128&&num_of_sectors>0;idx_dd++)
      {
        success=free_map_allocate(1,&indirects[idx_dd]);
        if(!success){
          goto FAIL_RET;
        }
        free_sector_reserve(&sectors,indirects[idx_dd]);
        memcpy(directs,zeros,BLOCK_SECTOR_SIZE);
        for(size_t idx_id=0;idx_id<128&&num_of_sectors>0;idx_id++)
        {
          success=free_map_allocate(1,&directs[idx_id]);
          if(!success){
            goto FAIL_RET;
          }
          free_sector_reserve(&sectors,directs[idx_id]);
          block_write(fs_device,directs[idx_id],zeros);
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
struct inode* inode_open(block_sector_t sector) {
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
      }else{
        break;
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
          }else{
            break;
          }    
        }
        free_map_release(indirects[idx_dd],1);
      }else{
        break;
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

/* Reads SIZE bytes from INODE into BUFFER, starting at position OFFSET.
   Returns the number of bytes actually read, which may be less
   than SIZE if an error occurs or end of file is reached. */
off_t inode_read_at(struct inode* inode, void* buffer_, off_t size, off_t offset) {
  uint8_t* buffer = buffer_;
  off_t bytes_read = 0;
  uint8_t* bounce = NULL;

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

