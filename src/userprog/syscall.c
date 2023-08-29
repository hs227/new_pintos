#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "userprog/process.h"

#include <stdbool.h>
#include "userprog/pagedir.h"
#include "threads/vaddr.h"
#include "filesys/file.h"
#include "filesys/inode.h"
#include "filesys/filesys.h"
#include "filesys/directory.h"
#include "devices/input.h"
#include "lib/float.h"
#include "threads/synch.h"
#include "threads/interrupt.h"


static void syscall_handler(struct intr_frame*);

static void syscall_halt(struct intr_frame*,uint32_t*);
static void syscall_exit(struct intr_frame*,uint32_t*);
static void syscall_exec(struct intr_frame*,uint32_t*);
static void syscall_wait(struct intr_frame*,uint32_t*);
static void syscall_create(struct intr_frame*,uint32_t*);
static void syscall_remove(struct intr_frame*,uint32_t*);
static void syscall_open(struct intr_frame*,uint32_t*);
static void syscall_filesize(struct intr_frame*,uint32_t*);
static void syscall_read(struct intr_frame*,uint32_t*);
static void syscall_write(struct intr_frame*,uint32_t*);
static void syscall_seek(struct intr_frame*,uint32_t*);
static void syscall_tell(struct intr_frame*,uint32_t*);
static void syscall_close(struct intr_frame*,uint32_t*);
static void syscall_practice(struct intr_frame*,uint32_t*);
static void syscall_compute_e(struct intr_frame*,uint32_t*);
static void syscall_pt_create(struct intr_frame*,uint32_t*);
static void syscall_pt_exit(struct intr_frame*,uint32_t*);
static void syscall_pt_join(struct intr_frame*,uint32_t*);
static void syscall_lock_init(struct intr_frame*,uint32_t*);
static void syscall_lock_acquire(struct intr_frame*,uint32_t*);
static void syscall_lock_release(struct intr_frame*,uint32_t*);
static void syscall_sema_init(struct intr_frame*,uint32_t*);
static void syscall_sema_down(struct intr_frame*,uint32_t*);
static void syscall_sema_up(struct intr_frame*,uint32_t*);
static void syscall_get_tid(struct intr_frame*,uint32_t*);
static void syscall_chdir(struct intr_frame*,uint32_t*);
static void syscall_mkdir(struct intr_frame*,uint32_t*);
static void syscall_readdir(struct intr_frame*,uint32_t*);
static void syscall_isdir(struct intr_frame*,uint32_t*);
static void syscall_inumber(struct intr_frame*,uint32_t*);


static bool valid_addr(void* addr)
{
  //bool res1=is_user_vaddr(addr);
  //bool res2=pagedir_get_page(thread_current()->pcb->pagedir,addr);
  //return res1&&res2;
  return is_user_vaddr(addr)&&
  pagedir_is_accessed(thread_current()->pcb->pagedir,addr)!=NULL;
}

static bool verify_ptr(void* ptr,size_t size)
{
  //bool res1=valid_addr(ptr);
  //bool res2=valid_addr(ptr+size);
  //return res1&&res2;
  return valid_addr(ptr)&&valid_addr(ptr+size);
}

static bool verify_str(char* ptr)
{
  while(1){
    if(valid_addr(ptr)){
      if(*ptr=='\0'){
        return true;
      }
    }else{
      return false;
    }
    ptr++;
  }

}

static struct lock file_open_lock;

void syscall_init(void) { 
  intr_register_int(0x30, 3, INTR_ON, syscall_handler, "syscall"); 
  
  lock_init(&file_open_lock);
}

static void syscall_handler(struct intr_frame* f UNUSED) {
  uint32_t* args = ((uint32_t*)f->esp);
  uint8_t init_fpu[108];
  /*
   * The following print statement, if uncommented, will print out the syscall
   * number whenever a process enters a system call. You might find it useful
   * when debugging. It will cause tests to fail, however, so you should not
   * include it in your final submission.
   */

  asm("fnsave (%0);fninit"::"g"(&init_fpu));

  /* verify the args pointer */
  /* args[0] */
  if(!verify_ptr(args,sizeof(uint32_t))){
    printf("%s: exit(-1)\n",thread_current()->pcb->process_name);
    process_exit();
  }
  /* args[i] */
  switch(args[0]){
    case SYS_READ:
    case SYS_WRITE:
    case SYS_PT_CREATE:
      if(!verify_ptr(&args[3],sizeof(uint32_t))){
        printf("%s: exit(-1)\n",thread_current()->pcb->process_name);
        process_exit(); 
      }
    case SYS_CREATE:
    case SYS_SEEK:
    case SYS_READDIR:
      if(!verify_ptr(&args[2],sizeof(uint32_t))){
        printf("%s: exit(-1)\n",thread_current()->pcb->process_name);
        process_exit();
      }
    case SYS_EXEC:
    case SYS_REMOVE:
    case SYS_OPEN:
    case SYS_FILESIZE:
    case SYS_TELL:
    case SYS_CLOSE:
    case SYS_COMPUTE_E:
    case SYS_LOCK_INIT:
    case SYS_LOCK_ACQUIRE:
    case SYS_LOCK_RELEASE:
    case SYS_SEMA_INIT:
    case SYS_SEMA_DOWN:
    case SYS_SEMA_UP:
    case SYS_MKDIR:
      if(!verify_ptr(&args[1],sizeof(uint32_t))){
        printf("%s: exit(-1)\n",thread_current()->pcb->process_name);
        process_exit();
      }
    default:
      break;
  }


  /* Verify the  args content */
  switch(args[0]){
    case SYS_EXEC:
    case SYS_CREATE:
    case SYS_REMOVE:
    case SYS_OPEN:
    case SYS_MKDIR:
      if(!verify_str((char*)args[1])){
        args[0]=SYS_EXIT;
        args[1]=-1;
      }
      break;
    case SYS_READDIR:
      if(!verify_str((char*)args[2])){
        args[0]=SYS_EXIT;
        args[1]=-1;
      }
      break;
    case SYS_READ:
    case SYS_WRITE:
      if(!verify_ptr(args[2],args[3]*sizeof(char))){
        args[0]=SYS_EXIT;
        args[1]=-1;
      }
      break;
    case SYS_LOCK_ACQUIRE:
    case SYS_LOCK_RELEASE:
      if(!verify_ptr(args[1],sizeof(struct lock))){
        args[0]=SYS_EXIT;
        args[1]=-1;
      }
      break;
    case SYS_SEMA_DOWN:
    case SYS_SEMA_UP:
      if(!verify_ptr(args[1],sizeof(struct semaphore))){
        args[0]=SYS_EXIT;
        args[1]=-1;
      }
      break;
    default:
      break;
  }

  /* handle */
  switch(args[0]){
    case SYS_HALT: /* Halt the operating system. */
      syscall_halt(f,args);
      break;
    case SYS_EXIT: /* Terminate this process. */
      syscall_exit(f,args);
      break;
    case SYS_EXEC: /* Start another process. */
      syscall_exec(f,args);
      break;
    case SYS_WAIT: /* Wait for a child process to die. */
      syscall_wait(f,args);
      break;
    case SYS_CREATE: /* Create a file. */
      syscall_create(f,args);
      break;
    case SYS_REMOVE: /* Delete a file. */
      syscall_remove(f,args);
      break;
    case SYS_OPEN: /* Open a file. */
      syscall_open(f,args);
      break;
    case SYS_FILESIZE: /* Obtain a file`s size. */
      syscall_filesize(f,args);
      break;
    case SYS_READ: /* Read from a file. */
      syscall_read(f,args);
      break;
    case SYS_WRITE: /* Write to a file. */
      syscall_write(f,args);
      break;
    case SYS_SEEK: /* Change position in a file. */
      syscall_seek(f,args);
      break;
    case SYS_TELL: /* Report current position in a file. */
      syscall_tell(f,args);
      break;
    case SYS_CLOSE: /* Close a file. */
      syscall_close(f,args);
      break;
    case SYS_PRACTICE: /* Returns args incremented by 1. */
      syscall_practice(f,args);
      break;
    case SYS_COMPUTE_E: /* Computes e */
      syscall_compute_e(f,args);
      break;
    case SYS_PT_CREATE: /* Creates a new thread */
      syscall_pt_create(f,args);
      break;
    case SYS_PT_EXIT: /* Exits the current thread */
      syscall_pt_exit(f,args);
      break;
    case SYS_PT_JOIN: /* Waits for thread to finish */
      syscall_pt_join(f,args);
      break;
    case SYS_LOCK_INIT: /* Initializes a lock */
      syscall_lock_init(f,args);
      break;
    case SYS_LOCK_ACQUIRE: /* Acquires a lock */
      syscall_lock_acquire(f,args);
      break;
    case SYS_LOCK_RELEASE: /* Releases a lock */
      syscall_lock_release(f,args);
      break;
    case SYS_SEMA_INIT: /* Initializes a semaphore */
      syscall_sema_init(f,args);
      break;
    case SYS_SEMA_DOWN: /* Downs a semaphore */
      syscall_sema_down(f,args);
      break;
    case SYS_SEMA_UP: /* Ups a semaphore */
      syscall_sema_up(f,args);
      break;
    case SYS_GET_TID: /* Gets TID of the current thread */
      syscall_get_tid(f,args);
      break;
    case SYS_CHDIR: /* Change the current directory */
      syscall_chdir(f,args);
      break;
    case SYS_MKDIR: /* Create a directory */
      syscall_mkdir(f,args);
      break; 
    case SYS_READDIR: /* Reads a directory entry */
      syscall_readdir(f,args);
      break;
    case SYS_ISDIR: /* Tests if a fd represents a directory */
      syscall_isdir(f,args);
      break;
    case SYS_INUMBER: /* Returns the inode number for a fd */
      syscall_inumber(f,args);
      break;
    default:
      printf("%s: exit(-1)\n",thread_current()->pcb->process_name);
      process_exit();
      break;
  }

  asm("frstor (%0)"::"g"(&init_fpu));
  
}

/* SYS_HALT */
static void syscall_halt(struct intr_frame* f UNUSED,uint32_t* args)
{
  //shutdown_power_off();
}

/* SYS_EXIT */
static void syscall_exit(struct intr_frame* f UNUSED,uint32_t* args)
{
  struct thread* cur=thread_current();
  int res=args[1];
  cur->pcb->exit_return=res;
  if(is_main_thread(cur,cur->pcb)){
    pthread_exit_main();
  }else
    pthread_exit();
}

/* SYS_EXEC */
static void syscall_exec(struct intr_frame* f UNUSED,uint32_t* args)
{
  f->eax=process_execute(args[1]);
}

/* SYS_WAIT */
static void syscall_wait(struct intr_frame* f UNUSED,uint32_t* args)
{
  f->eax=process_wait(args[1]);
}

/* SYS_CREATE */
static void syscall_create(struct intr_frame* f UNUSED,uint32_t* args)
{
  const char* name=args[1];
  unsigned initial_size=args[2];
  size_t name_len=strlen(name);
  if(name_len==0){
    /* empty */
    printf("%s: exit(-1)\n",thread_current()->pcb->process_name);
    process_exit();
  }

  f->eax=filesys_create(name,initial_size,false);
}

/* SYS_REMOVE */
static void syscall_remove(struct intr_frame* f UNUSED,uint32_t* args)
{
  f->eax=filesys_remove(args[1]);
}
/* SYS_OPEN */
static void syscall_open(struct intr_frame* f UNUSED,uint32_t* args)
{
  struct thread* t=thread_current();
  struct process* pcb=t->pcb;
  const char* fn=args[1];
  size_t name_len=strlen(fn);
  struct file* file=NULL;
  int fd;
  f->eax=-1;
  if(name_len==0){
    /* empty */
    return;
  }
  
  lock_acquire(&file_open_lock);
  fd=next_empty_fd_table(&pcb->fd_table);
  if(fd==-1){
    // fd_table is full
    lock_release(&file_open_lock);
    return;
  }
  pcb->fd_table.fds[fd].fd_flags=1;
  
  file=filesys_open(fn);
  lock_release(&file_open_lock);
  if(file==NULL){
    // open failed
    pcb->fd_table.fds[fd].fd_flags=0;
    return ;
  }
  pcb->fd_table.fds[fd].file_ptr=file;
  f->eax=fd;

}
/* SYS_FILESIZE */
static void syscall_filesize(struct intr_frame* f UNUSED,uint32_t* args)
{
  struct process* pcb=thread_current()->pcb;
  int fd=args[1];
  struct file* file=get_file_fdt(&pcb->fd_table,fd);
  if(file==NULL){
    f->eax=-1;
  }else{
    f->eax=file_length(file);
  }
}
/* SYS_READ */
static void syscall_read(struct intr_frame* f UNUSED,uint32_t* args)
{
  struct process* pcb=thread_current()->pcb;
  int fd=args[1];
  char* buffer=args[2];
  unsigned size=args[3];
  struct file* file;
  f->eax=-1;
  if(fd==STDIN_FILENO){
    unsigned int i;
    for(i=0;i<size;++i){
      buffer[i]=input_getc();
    }
    f->eax=i;
    return;
  }
  file=get_file_fdt(&pcb->fd_table,fd);
  if(file==NULL){
    return;
  }
  if(inode_get_isdir(file_get_inode(file)))
    return;
  f->eax=file_read(file,buffer,size);
}

/* SYS_WRITE */
static void syscall_write(struct intr_frame* f UNUSED,uint32_t* args)
{
  struct process* pcb=thread_current()->pcb;
  int fd=args[1];
  char*buffer=args[2];
  unsigned size=args[3];
  struct file* file;
  f->eax=-1;
  if(fd==STDOUT_FILENO){
    putbuf(buffer,size);
    f->eax=size;
    return;
  }
  file=get_file_fdt(&pcb->fd_table,fd);
  if(file==NULL){
    return;
  }
  if(inode_get_isdir(file_get_inode(file)))
    return;
  f->eax=file_write(file,buffer,size);
}

/* SYS_SEEK */
static void syscall_seek(struct intr_frame* f UNUSED,uint32_t* args)
{
  struct process* pcb=thread_current()->pcb;
  int fd=args[1];
  unsigned position=args[2];
  struct file* file;
  f->eax=-1;
  file=get_file_fdt(&pcb->fd_table,fd);
  if(file==NULL){
    return;
  }
  file_seek(file,position);

}
/* SYS_TELL */
static void syscall_tell(struct intr_frame* f UNUSED,uint32_t* args)
{
  struct process* pcb=thread_current()->pcb;
  int fd=args[1];
  struct file* file;
  f->eax=-1;
  file=get_file_fdt(&pcb->fd_table,fd);
  if(file==NULL){
    return;
  }
  f->eax=file_tell(file);

}
/* SYS_CLOSE */
static void syscall_close(struct intr_frame* f UNUSED,uint32_t* args)
{
  struct process* pcb=thread_current()->pcb;
  int fd=args[1];
  struct file* file;
  f->eax=-1;
  
  enum intr_level old_level=intr_disable(); 
  file=get_file_fdt(&pcb->fd_table,fd);
  if(file==NULL){
    intr_set_level(old_level);
    return;
  }
  pcb->fd_table.fds[fd].fd_flags=0;
  pcb->fd_table.fds[fd].file_ptr=NULL;
  intr_set_level(old_level);
  if(inode_get_isdir(file_get_inode(file)))
    dir_close((struct dir*)file);
  else
    file_close(file);
  f->eax=1;

}
/* SYS_PRACTICE */
static void syscall_practice(struct intr_frame* f,uint32_t* args)
{
  f->eax=args[1]+1;
}


/* SYS_COMPUTE_E */
static void syscall_compute_e(struct intr_frame* f,uint32_t* args)
{
  int n=args[1];
  f->eax=sys_sum_to_e(n);
}

/* SYS_PT_CREATE */
static void syscall_pt_create(struct intr_frame* f,uint32_t* args)
{
  stub_fun sf= args[1];
  pthread_fun tf=args[2];
  void* arg=args[3];

  f->eax=pthread_execute(sf,tf,arg);

  struct thread* t=thread_current();
  int size=list_size(&t->pcb->threads);
}

/* SYS_PT_EXIT */
static void syscall_pt_exit(struct intr_frame* f,uint32_t* args)
{
  struct thread* t=thread_current();
  if(is_main_thread(t,t->pcb)){
    pthread_exit_main();
  }else
    pthread_exit();
}


/* SYS_PT_JOIN */
static void syscall_pt_join(struct intr_frame* f,uint32_t* args)
{
  tid_t tid=args[1];
  struct thread* t=thread_current();
  int size=list_size(&t->pcb->threads);

  f->eax=pthread_join(tid);
}


/* SYS_LOCK_INIT */
static void syscall_lock_init(struct intr_frame* f,uint32_t* args)
{
  struct lock* lock=args[1];
  if(lock==NULL){
    f->eax=false;
    return;
  }
  lock_init(lock);
  f->eax=true;
}

/* SYS_LOCK_ACQUIRE */
static void syscall_lock_acquire(struct intr_frame* f,uint32_t* args)
{
  struct lock* lock=args[1];
  if(!lock_is_init(lock)||
      lock_held_by_current_thread(lock)){
    args[1]=1;
    syscall_exit(f,args);
  }
  lock_acquire(lock);
  f->eax=true;
}

/* SYS_LOCK_RELEASE */
static void syscall_lock_release(struct intr_frame* f,uint32_t* args)
{
  struct lock* lock=args[1];
  if(!lock_is_init(lock)||
      !lock_held_by_current_thread(lock)){
    args[1]=1;
    syscall_exit(f,args);
  }
  lock_release(lock);
  f->eax=true;
}

/* SYS_SEMA_INIT */
static void syscall_sema_init(struct intr_frame* f,uint32_t* args)
{
  struct semaphore* sema=args[1];
  int val=args[2];
  if(sema==NULL||val<0){
    f->eax=false;
    return;
  }
  sema_init(sema,val);
  f->eax=true;
}

/* SYS_SEMA_DOWN */
static void syscall_sema_down(struct intr_frame* f,uint32_t* args)
{
  struct semaphore* sema=args[1];
  if(!sema_is_init(sema)){
    args[1]=1;
    syscall_exit(f,args);
  }
  sema_down(sema);
  f->eax=true;
}

/* SYS_SEMA_UP */
static void syscall_sema_up(struct intr_frame* f,uint32_t* args)
{
  struct semaphore* sema=args[1];
  if(!sema_is_init(sema)){
    args[1]=1;
    syscall_exit(f,args);
  }
  sema_up(sema);
  f->eax=true;
}

/* SYS_GET_TID */
static void syscall_get_tid(struct intr_frame* f,uint32_t* args)
{
  f->eax=thread_current()->tid;
}


/* SYS_CHDIR */
static void syscall_chdir(struct intr_frame* f,uint32_t* args)
{
  const char* dir=args[1];
  f->eax=filesys_chdir(dir);
}

/* SYS_MKDIR */
static void syscall_mkdir(struct intr_frame* f,uint32_t* args)
{
  const char* path=args[1];
  f->eax=filesys_mkdir(path);
}

/* SYS_READDIR */
static void syscall_readdir(struct intr_frame* f,uint32_t* args)
{
  struct process* pcb=thread_current()->pcb;
  int fd=args[1];
  char* name=args[2];
  struct file* file;
  f->eax=false;

  enum intr_level old_level=intr_disable();
  file=get_file_fdt(&pcb->fd_table,fd);
  if(file==NULL){
    intr_set_level(old_level);
    return;
  }
  intr_set_level(old_level);

  if(inode_get_isdir(file_get_inode(file)))
    f->eax=dir_readdir(file,name);

}


/* SYS_ISDIR */
static void syscall_isdir(struct intr_frame* f,uint32_t* args)
{
  struct process* pcb=thread_current()->pcb;
  int fd=(int)args[1];
  struct file* file;
  f->eax=-1;
  file=get_file_fdt(&pcb->fd_table,fd);
  if(file==NULL)
    return;
  f->eax=inode_get_isdir(file_get_inode(file));
}


/* SYS_INUMBER */
static void syscall_inumber(struct intr_frame* f,uint32_t* args)
{
  struct process* pcb=thread_current()->pcb;
  int fd=args[1];
  struct file* file;
  f->eax=-1;
  file=get_file_fdt(&pcb->fd_table,fd);
  if(file==NULL){
    return;
  }
  f->eax=inode_get_inumber(file_get_inode(file));
}








