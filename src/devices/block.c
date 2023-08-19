#include "devices/block.h"
#include <list.h>
#include <string.h>
#include <stdio.h>
#include "devices/ide.h"
#include "threads/malloc.h"

/* A block device. */
struct block {
  struct list_elem list_elem; /* Element in all_blocks. */

  char name[16];        /* Block device name. */
  enum block_type type; /* Type of block device. */
  block_sector_t size;  /* Size in sectors. */

  const struct block_operations* ops; /* Driver operations. */
  void* aux;                          /* Extra data owned by driver. */

  unsigned long long read_cnt;  /* Number of sectors read. */
  unsigned long long write_cnt; /* Number of sectors written. */
};

/* List of all block devices. */
static struct list all_blocks = LIST_INITIALIZER(all_blocks);

/* The block block assigned to each Pintos role. */
static struct block* block_by_role[BLOCK_ROLE_CNT];

static struct block* list_elem_to_block(struct list_elem*);

/* Returns a human-readable name for the given block device
   TYPE. */
const char* block_type_name(enum block_type type) {
  static const char* block_type_names[BLOCK_CNT] = {
      "kernel", "filesys", "scratch", "swap", "raw", "foreign",
  };

  ASSERT(type < BLOCK_CNT);
  return block_type_names[type];
}

/* Returns the block device fulfilling the given ROLE, or a null
   pointer if no block device has been assigned that role. */
struct block* block_get_role(enum block_type role) {
  ASSERT(role < BLOCK_ROLE_CNT);
  return block_by_role[role];
}

/* Assigns BLOCK the given ROLE. */
void block_set_role(enum block_type role, struct block* block) {
  ASSERT(role < BLOCK_ROLE_CNT);
  block_by_role[role] = block;
}

/* Returns the first block device in kernel probe order, or a
   null pointer if no block devices are registered. */
struct block* block_first(void) {
  return list_elem_to_block(list_begin(&all_blocks));
}

/* Returns the block device following BLOCK in kernel probe
   order, or a null pointer if BLOCK is the last block device. */
struct block* block_next(struct block* block) {
  return list_elem_to_block(list_next(&block->list_elem));
}

/* Returns the block device with the given NAME, or a null
   pointer if no block device has that name. */
struct block* block_get_by_name(const char* name) {
  struct list_elem* e;

  for (e = list_begin(&all_blocks); e != list_end(&all_blocks); e = list_next(e)) {
    struct block* block = list_entry(e, struct block, list_elem);
    if (!strcmp(name, block->name))
      return block;
  }

  return NULL;
}

/* Verifies that SECTOR is a valid offset within BLOCK.
   Panics if not. */
static void check_sector(struct block* block, block_sector_t sector) {
  if (sector >= block->size) {
    /* We do not use ASSERT because we want to panic here
         regardless of whether NDEBUG is defined. */
    PANIC("Access past end of device %s (sector=%" PRDSNu ", "
          "size=%" PRDSNu ")\n",
          block_name(block), sector, block->size);
  }
}

extern void blk_cache_read(struct block* block,block_sector_t sector,void* buffer);
extern void blk_cache_write(struct block* block,block_sector_t sector,void* buffer);

/* Reads sector SECTOR from BLOCK into BUFFER, which must
   have room for BLOCK_SECTOR_SIZE bytes.
   Internally synchronizes accesses to block devices, so external
   per-block device locking is unneeded. */
void block_read(struct block* block, block_sector_t sector, void* buffer) {
  check_sector(block, sector);
  blk_cache_read(block,sector,buffer);
  block->read_cnt++;
}

void block_raw_read(struct block* block,block_sector_t sector,void* buffer)
{
  block->ops->read(block->aux,sector,buffer);
}

/* Write sector SECTOR to BLOCK from BUFFER, which must contain
   BLOCK_SECTOR_SIZE bytes.  Returns after the block device has
   acknowledged receiving the data.
   Internally synchronizes accesses to block devices, so external
   per-block device locking is unneeded. */
void block_write(struct block* block, block_sector_t sector, const void* buffer) {
  check_sector(block, sector);
  ASSERT(block->type != BLOCK_FOREIGN);
  blk_cache_write(block,sector,buffer);
  block->write_cnt++;
}

void block_raw_write(struct block* block, block_sector_t sector,void* buffer)
{
  block->ops->write(block->aux, sector, buffer);
}
/* Returns the number of sectors in BLOCK. */
block_sector_t block_size(struct block* block) { return block->size; }

/* Returns BLOCK's name (e.g. "hda"). */
const char* block_name(struct block* block) { return block->name; }

/* Returns BLOCK's type. */
enum block_type block_type(struct block* block) { return block->type; }

/* Prints statistics for each block device used for a Pintos role. */
void block_print_stats(void) {
  int i;

  for (i = 0; i < BLOCK_ROLE_CNT; i++) {
    struct block* block = block_by_role[i];
    if (block != NULL) {
      printf("%s (%s): %llu reads, %llu writes\n", block->name, block_type_name(block->type),
             block->read_cnt, block->write_cnt);
    }
  }
}

/* Registers a new block device with the given NAME.  If
   EXTRA_INFO is non-null, it is printed as part of a user
   message.  The block device's SIZE in sectors and its TYPE must
   be provided, as well as the it operation functions OPS, which
   will be passed AUX in each function call. */
struct block* block_register(const char* name, enum block_type type, const char* extra_info,
                             block_sector_t size, const struct block_operations* ops, void* aux) {
  struct block* block = malloc(sizeof *block);
  if (block == NULL)
    PANIC("Failed to allocate memory for block device descriptor");

  list_push_back(&all_blocks, &block->list_elem);
  strlcpy(block->name, name, sizeof block->name);
  block->type = type;
  block->size = size;
  block->ops = ops;
  block->aux = aux;
  block->read_cnt = 0;
  block->write_cnt = 0;

  printf("%s: %'" PRDSNu " sectors (", block->name, block->size);
  print_human_readable_size((uint64_t)block->size * BLOCK_SECTOR_SIZE);
  printf(")");
  if (extra_info != NULL)
    printf(", %s", extra_info);
  printf("\n");

  return block;
}

/* Returns the block device corresponding to LIST_ELEM, or a null
   pointer if LIST_ELEM is the list end of all_blocks. */
static struct block* list_elem_to_block(struct list_elem* list_elem) {
  return (list_elem != list_end(&all_blocks) ? list_entry(list_elem, struct block, list_elem)
                                             : NULL);
}


#define BLK_CACHE_SIZE 64

struct blk_cache_entry{
  uint8_t disk_data[BLOCK_SECTOR_SIZE]; /* block_size */
  block_sector_t sector; /* sector identify */
  struct block* block; /* block identify */
  bool is_dirty; /* the sign for write back */
  bool is_out; /* the sign for NRU */
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
struct blk_cache_entry* blk_cache_held(struct block* block,block_sector_t sector);
void blk_cache_read(struct block* block,block_sector_t sector,void* buffer);
void blk_cache_write(struct block* block,block_sector_t sector,void* buffer);
struct blk_cache_entry* blk_cache_in(struct block* block,block_sector_t sector);
struct blk_cache_entry* blk_cache_out(struct block* block);

/*cache init */
void blk_cache_init(void)
{
  for(int i=0;i<BLK_CACHE_SIZE;++i){
    struct blk_cache_entry* entry=&blk_cache.entrys[i];
    entry->sector=BLOCK_SECTOR_ERROR;
    entry->block=NULL;
    entry->is_dirty=false;
    entry->is_out=false;
  }
  blk_cache.size=0;
  blk_cache.nru_ptr=0;
}

void blk_cache_done(void)
{
  /* update all the dirty block */
  for(int i=0;i<BLK_CACHE_SIZE;++i){
    struct blk_cache_entry* entry=&blk_cache.entrys[i];
    if( entry->sector!=BLOCK_SECTOR_ERROR&&entry->is_dirty){
      block_raw_write(entry->block,entry->sector,entry->disk_data);
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
struct blk_cache_entry* blk_cache_held(struct block* block,block_sector_t sector)
{
  for(int i=0;i<blk_cache.size;++i){
    if(blk_cache.entrys[i].block==block&&
        blk_cache.entrys[i].sector==sector){
     return  &blk_cache.entrys[i];
    }
  }
  return NULL;
}

/* indirect layer for reading */
void blk_cache_read(struct block* block,block_sector_t sector,void* buffer)
{
  struct blk_cache_entry* entry;
  /* 1. if the sector already in cache */
  if((entry=blk_cache_held(block,sector))==NULL){
    /* 2. not in cache,and cache in it */
    entry=blk_cache_in(block,sector);
  }
  ASSERT(entry!=NULL);

  /* 3. read from cache */
  memcpy(buffer,entry->disk_data,BLOCK_SECTOR_SIZE);
  entry->is_out=true;
}

/* indirect layer for writing */
void blk_cache_write(struct block* block, block_sector_t sector,void* buffer)
{
  struct blk_cache_entry* entry;
  /* 1. if the sector already in cache */
  if((entry=blk_cache_held(block,sector))==NULL){
    /* 2. not in cache,and cache in it */
    entry=blk_cache_in(block,sector);
  }
  ASSERT(entry!=NULL);

  /* 3. write to cache */
  memcpy(entry->disk_data,buffer,BLOCK_SECTOR_SIZE);
  entry->is_dirty=true;
  entry->is_out=true;
}

/* let block cache in */
struct blk_cache_entry* blk_cache_in(struct block* block,block_sector_t sector)
{
  ASSERT(!blk_cache_held(block,sector));

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
  entry->block=block;
  entry->is_dirty=false;
  entry->is_out=true;
  block_raw_read(block,entry->sector,entry->disk_data);

  return entry;
}

/* evict the old block using the NRU */
struct blk_cache_entry* blk_cache_out(struct block* block)
{
  /* hand circuit */
  while(blk_cache.entrys[blk_cache.nru_ptr].is_out==true){
    blk_cache.entrys[blk_cache.nru_ptr].is_out=false;
    blk_cache_nru_ptr_move();
    
  }
  struct blk_cache_entry* entry=&blk_cache.entrys[blk_cache.nru_ptr];
  blk_cache_nru_ptr_move();

  /* write back */
  if(entry->is_dirty){
    block_raw_write(block,entry->sector,entry->disk_data);
  }
  entry->sector=BLOCK_SECTOR_ERROR;
  entry->block=NULL;
  return entry;
}














