#include "userprog/process.h"
#include <debug.h>
#include <inttypes.h>
#include <round.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "userprog/gdt.h"
#include "userprog/pagedir.h"
#include "userprog/tss.h"
#include "filesys/directory.h"
#include "filesys/file.h"
#include "filesys/filesys.h"
#include "threads/flags.h"
#include "threads/init.h"
#include "threads/interrupt.h"
#include "threads/malloc.h"
#include "threads/palloc.h"
#include "threads/synch.h"
#include "threads/thread.h"
#include "threads/vaddr.h"

static struct semaphore temporary;
static thread_func start_process NO_RETURN;
static thread_func start_pthread NO_RETURN;
static bool load(const char* file_name, void (**eip)(void), void** esp);
//bool setup_thread(void (**eip)(void), void** esp);
static void init_fdt(struct file_descriptor_table* fd_table);
static void init_cpt(struct child_proc_table* cp_table);



/* Initializes user programs in the system by ensuring the main
   thread has a minimal PCB so that it can execute and wait for
   the first user process. Any additions to the PCB should be also
   initialized here if main needs those members */
void userprog_init(void) {
  struct thread* t = thread_current();
  bool success;

  /* Allocate process control block
     It is imoprtant that this is a call to calloc and not malloc,
     so that t->pcb->pagedir is guaranteed to be NULL (the kernel's
     page directory) when t->pcb is assigned, because a timer interrupt
     can come at any time and activate our pagedir */
  t->pcb = calloc(sizeof(struct process), 1);
  success = t->pcb != NULL;

  /* Kill the kernel if we did not succeed */
  ASSERT(success);


  sema_init(&t->pcb->sema,0);
  init_fdt(&t->pcb->fd_table);
  init_cpt(&t->pcb->cp_table);
  t->pcb->exit_return=0;
  t->pcb->exe_file=NULL;
  list_init(&t->pcb->threads);
  list_push_back(&t->pcb->threads,&t->pcb_elem);
  t->pcb->next_thread_stack=PHYS_BASE;
  list_init(&t->pcb->exit_threads);

}

/* the optimize way is bitmap */
/* mycode: */
static void init_fdt(struct file_descriptor_table* fd_table)
{
  int i;
  for(i=0;i<FD_TABLE_SIZE;++i){
    fd_table->fds[i].fd_flags=0;
    fd_table->fds[i].file_ptr=NULL;
  }

  /* init the standard IO */
  fd_table->fds[STDIN_FILENO].fd_flags=1;
  fd_table->fds[STDOUT_FILENO].fd_flags=1;

  /* init the cur */
  fd_table->cur=2;
}

/* mycode: close file_descriptor_table */
static void destory_fdt(struct file_descriptor_table* fd_table)
{
  int i;
  for(i=2;i<FD_TABLE_SIZE;++i){
    if(fd_table->fds[i].fd_flags==1){
      file_close(fd_table->fds[i].file_ptr);
    }
  }
}

/* mycode: */
static int inc_cur_fdt(struct file_descriptor_table* fd_table)
{
  fd_table->cur=(fd_table->cur+1)%FD_TABLE_SIZE;
  return fd_table->cur;
}
/* mycode: */
static int dec_cur_fdt(struct file_descriptor_table* fd_table)
{
  fd_table->cur=(fd_table->cur-1+FD_TABLE_SIZE)%FD_TABLE_SIZE;
  return fd_table->cur;
}
/* mycode: next empty (-1 means the full) */
int next_empty_fd_table(struct file_descriptor_table* fd_table)
{
  int i;
  int head=fd_table->cur;
  if(fd_table->fds[head].fd_flags==0){
    return head;
  }
  for(i=inc_cur_fdt(fd_table);i!=head;i=inc_cur_fdt(fd_table)){
    if(fd_table->fds[i].fd_flags==0){
      return i;
    }
  }
  // the fd_table is full
  ASSERT(!"the fd_table is full");
  return -1;
}

/* mycode: release a fd */
struct file* release_fd_fdt(struct file_descriptor_table*fd_table,int fd)
{
  if(fd>STDOUT_FILENO&&fd<FD_TABLE_SIZE){
    if(fd_table->fds[fd].fd_flags==1){
      fd_table->fds[fd].fd_flags=0;
      return fd_table->fds[fd].file_ptr;
    }
  }
  return NULL;
}

/* mycode: get the file by fd */
struct file* get_file_fdt(struct file_descriptor_table* fd_table,int fd)
{
  if(fd>STDOUT_FILENO&&fd<FD_TABLE_SIZE){
    if(fd_table->fds[fd].fd_flags==1){
      return fd_table->fds[fd].file_ptr;
    }
  }
  return NULL;
}

/* mycode: about the child_proc_table */
static void init_cpt(struct child_proc_table* cp_table)
{
  int i;
  for(i=0;i<CP_TABLE_SIZE;++i){
    cp_table->cps[i].id=-1;
    cp_table->cps[i].child_res=0;
  }

  /* init the cur */
  cp_table->cur=0;
}

/* mycode: close child_proc_table */
static void destory_cpt(struct child_proc_table* cp_table)
{
  // do nothing
}

/* mycode: */
static int inc_cur_cpt(struct child_proc_table* cp_table)
{
  cp_table->cur=(cp_table->cur+1)%CP_TABLE_SIZE;
  return cp_table->cur;
}

/* mycode: */
static int dec_cur_cpt(struct child_proc_table* cp_table)
{
  cp_table->cur=(cp_table->cur-1+CP_TABLE_SIZE)%CP_TABLE_SIZE;
  return cp_table->cur;
}

/* mycode: next empty (-1 means the cp_table full) */
int next_empty_child_cpt(struct child_proc_table* cp_table)
{
  int i;
  int head=cp_table->cur;
  if(cp_table->cps[head].id==-1){
    return head;
  }
  for(i=inc_cur_cpt(cp_table);i!=head;i=inc_cur_cpt(cp_table)){
    if(cp_table->cps[i].id==-1){
      return i;
    }
  }
  // the cp_table is full
  ASSERT(!"the child_cpt is full");
  return -1;
}

/* mycode: child send the exit_res */
int child_return_cpt(struct child_proc_table* cp_table,struct process* child,int res)
{
  int i;
  pid_t pid=get_pid(child);
  for(i=0;i<CP_TABLE_SIZE;++i){
    if(cp_table->cps[i].id==pid){
      cp_table->cps[i].child_res=res;
      return;
    }
  }
  return -1;
}

/* mycode: father only wait child */
int father_wait_cpt(struct child_proc_table* cp_table,struct process* father,pid_t id)
{
  int i;
  for(i=0;i<CP_TABLE_SIZE;++i){
    if(cp_table->cps[i].id==id){
      return i;
    }
  }
  return -1;
}


/* Starts a new thread running a user program loaded from
   FILENAME.  The new thread may be scheduled (and may even exit)
   before process_execute() returns.  Returns the new process's
   process id, or TID_ERROR if the thread cannot be created. */
pid_t process_execute(const char* file_name) {
  char* fn_copy;
  tid_t tid;
  char* name;
  size_t name_len;

  struct file* file_check=NULL;
  struct thread* t=thread_current();

  fn_copy=palloc_get_page(0);
  if(fn_copy==NULL){
    return TID_ERROR;
  }
  strlcpy(fn_copy,file_name,strlen(file_name)+1);

  /* extract file_name */
  name_len=strcspn(fn_copy," ")+1;
  name=palloc_get_page(0);
  if(name==NULL){
    return TID_ERROR;
  }
  strlcpy(name,fn_copy,name_len);
  name[name_len]='\0';

  /* Check if the file exist */
  file_check=filesys_open(name);
  if(file_check!=NULL){
    file_close(file_check);
  }else{
  /* not exist */
    palloc_free_page(fn_copy);
    palloc_free_page(name);
    return -1;
  }


  /* Create a new thread to execute FILE_NAME */
  tid=thread_create(name,PRI_DEFAULT,start_process,fn_copy);
  if(tid==TID_ERROR){
    palloc_free_page(fn_copy);
  }
  palloc_free_page(name);

  /* register the cp_table */
  if(tid!=TID_ERROR){
    int cp=next_empty_child_cpt(&t->pcb->cp_table);
    t->pcb->cp_table.cps[cp].id=tid;
  }


  return tid;
}

/* delete the more space in str */
static void modify_str_space(char* src,char* des)
{
  char* current=des;
  char *last=NULL,*next=src;
  while(*next!='\0'){
    if(last&&*last==' '&&*next==' '){
      /* do nothing */
    }else{
      *current=*next;
      current++;
    }
    last=next;
    next++;
  }
  *current='\0';
}


static void fillup_stack(char* file_name,struct intr_frame* if_)
{
#define STR_LEN 512
#define ARGV_LEN 64

  size_t fn_size=strlen(file_name)+1;
  char buf[STR_LEN];
  char* finder=buf-1;
  uint32_t argvs[ARGV_LEN];
  uint32_t argc=1;
  int i;

  uint32_t more;

  uint32_t argv_tmp;
  uint32_t argc_tmp;
  uint32_t rs_tmp=0;


  modify_str_space(file_name,buf);
  memset(argvs,0x0,sizeof(uint32_t)*ARGV_LEN);

  /* 1.push the argv[i][...] */
  if_->esp-=sizeof(char)*fn_size;
  argvs[0]=if_->esp;
  while((finder=strchr(finder+1,' '))!=NULL){
    argvs[argc]=(finder-buf+1)+if_->esp;
    *finder='\0';
    argc++;
  }
  memcpy(if_->esp,buf,fn_size);

  /* 2.push the stack-align */
  more=(uint32_t)(if_->esp-(sizeof(uint32_t)*(1+1+argc+1)))%16;
  if_->esp-=more;
  memset(if_->esp,0,sizeof(uint8_t)*more);

  /* 3.push the argv[i] */
  if_->esp-=(argc+1)*sizeof(uint32_t);
  memcpy(if_->esp,argvs,sizeof(uint32_t)*(argc+1));

  /* 4.push the argv ad argc */
  argv_tmp=if_->esp;
  if_->esp-=sizeof(uint32_t);
  memcpy(if_->esp,&argv_tmp,sizeof(uint32_t));
  argc_tmp=argc;
  if_->esp-=sizeof(uint32_t);
  memcpy(if_->esp,&argc_tmp,sizeof(uint32_t));

  /* 5.push the valid return address */
  if_->esp-=sizeof(uint32_t);
  memcpy(if_->esp,&rs_tmp,sizeof(uint32_t));

#undef ARGV_LEN
#undef STR_LEN
}



/* A thread function that loads a user process and starts it
   running. */
static void start_process(void* file_name_) {
#define FN_LEN 512
  char file_name[FN_LEN];
  size_t file_name_len;
  struct thread* t = thread_current();
  struct intr_frame if_;
  bool if_success, pcb_success;

  /* Allocate process control block */
  struct process* new_pcb = malloc(sizeof(struct process));
  pcb_success = new_pcb != NULL;

  file_name_len=strcspn(file_name_," ");
  memcpy(file_name,file_name_,file_name_len);
  file_name[file_name_len]='\0';


  if (pcb_success) {
    /* Initialize process control block */
    new_pcb->pagedir = NULL;
    t->pcb = new_pcb;
    sema_init(&t->pcb->sema,0);
    strlcpy(t->pcb->process_name, t->name, sizeof t->name);
    init_fdt(&t->pcb->fd_table);
    init_cpt(&t->pcb->cp_table);
    t->pcb->exit_return=0;
    list_init(&t->pcb->threads);
    list_push_back(&t->pcb->threads,&t->pcb_elem);
    t->pcb->next_thread_stack=PHYS_BASE;
    list_init(&t->pcb->exit_threads);

    /* Initialize interrupt frame and load executable. */
    memset(&if_, 0, sizeof if_);
    if_.gs = if_.fs = if_.es = if_.ds = if_.ss = SEL_UDSEG;
    if_.cs = SEL_UCSEG;
    if_.eflags = FLAG_IF | FLAG_MBS;
    if_success = load(file_name, &if_.eip, &if_.esp);


  }


  /* Handle failure with succesful PCB malloc. Must free the PCB */
  if (pcb_success && !if_success) {
    // Avoid race where PCB is freed before t->pcb is set to NULL
    // If this happens, then an unfortuantely timed timer interrupt
    // can try to activate the pagedir, but it is now freed memory
    struct process* pcb_to_free = t->pcb;
    t->pcb = NULL;
    free(pcb_to_free);
  }

  /* Clean up. Exit on failure or jump to userspace */
  if (!pcb_success || !if_success) {
    sema_up(&t->father->sema);
    destory_fdt(&t->pcb->fd_table);
    thread_exit();
  }

  /* build the argv and argc */
  fillup_stack(file_name_,&if_);

  /* destory the file_name_ */
  palloc_free_page(file_name_);

  /* init the fpu */
  asm("fninit;fsave (%0)"::"g"(&t->fpu));

  /* Start the user process by simulating a return from an
     interrupt, implemented by intr_exit (in
     threads/intr-stubs.S).  Because intr_exit takes all of its
     arguments on the stack in the form of a `struct intr_frame',
     we just point the stack pointer (%esp) to our stack frame
     and jump to it. */
  asm volatile("movl %0, %%esp; jmp intr_exit" : : "g"(&if_) : "memory");
  NOT_REACHED();
#undef FN_LEN
}

/* Waits for process with PID child_pid to die and returns its exit status.
   If it was terminated by the kernel (i.e. killed due to an
   exception), returns -1.  If child_pid is invalid or if it was not a
   child of the calling process, or if process_wait() has already
   been successfully called for the given PID, returns -1
   immediately, without waiting.

   This function will be implemented in problem 2-2.  For now, it
   does nothing. */
int process_wait(pid_t child_pid UNUSED) {
  struct thread* t=thread_current();
  int child=father_wait_cpt(&t->pcb->cp_table,t->pcb,child_pid);
  if(child!=-1){
    // child to wait
    //asm("fsave (%0)"::"g"(&t->pcb->fpu));
    sema_down(&t->pcb->sema);
    //asm("frstor (%0)"::"g"(&t->pcb->fpu));
    t->pcb->cp_table.cps[child].id=-1;
    return t->pcb->cp_table.cps[child].child_res;
  }
  return -1;
}

/* Free the current process's resources. */
void process_exit(void) {
  struct thread* cur = thread_current();
  uint32_t* pd;
  struct file* file;

  /* If this thread does not have a PCB, don't worry */
  if (cur->pcb == NULL) {
    thread_exit();
    NOT_REACHED();
  }

  if(cur->father){
    child_return_cpt(&cur->father->cp_table,cur->pcb,cur->pcb->exit_return);
  }

  if(cur->pcb->exe_file){
    file_close(cur->pcb->exe_file);
  }

  destory_fdt(&cur->pcb->fd_table);
  destory_cpt(&cur->pcb->cp_table);


  /* Destroy the current process's page directory and switch back
     to the kernel-only page directory. */
  pd = cur->pcb->pagedir;
  if (pd != NULL) {
    /* Correct ordering here is crucial.  We must set
         cur->pcb->pagedir to NULL before switching page directories,
         so that a timer interrupt can't switch back to the
         process page directory.  We must activate the base page
         directory before destroying the process's page
         directory, or our active page directory will be one
         that's been freed (and cleared). */
    cur->pcb->pagedir = NULL;
    pagedir_activate(NULL);
    pagedir_destroy(pd);
  }

  /* Free the PCB of this process and kill this thread
     Avoid race where PCB is freed before t->pcb is set to NULL
     If this happens, then an unfortuantely timed timer interrupt
     can try to activate the pagedir, but it is now freed memory */
  struct process* pcb_to_free = cur->pcb;
  cur->pcb = NULL;
  free(pcb_to_free);

  sema_up(&thread_current()->father->sema);
  thread_exit();
}

/* Sets up the CPU for running user code in the current
   thread. This function is called on every context switch. */
void process_activate(void) {
  struct thread* t = thread_current();

  /* Activate thread's page tables. */
  if (t->pcb != NULL && t->pcb->pagedir != NULL)
    pagedir_activate(t->pcb->pagedir);
  else
    pagedir_activate(NULL);

  /* Set thread's kernel stack for use in processing interrupts.
     This does nothing if this is not a user process. */
  tss_update();
}

/* We load ELF binaries.  The following definitions are taken
   from the ELF specification, [ELF1], more-or-less verbatim.  */

/* ELF types.  See [ELF1] 1-2. */
typedef uint32_t Elf32_Word, Elf32_Addr, Elf32_Off;
typedef uint16_t Elf32_Half;

/* For use with ELF types in printf(). */
#define PE32Wx PRIx32 /* Print Elf32_Word in hexadecimal. */
#define PE32Ax PRIx32 /* Print Elf32_Addr in hexadecimal. */
#define PE32Ox PRIx32 /* Print Elf32_Off in hexadecimal. */
#define PE32Hx PRIx16 /* Print Elf32_Half in hexadecimal. */

/* Executable header.  See [ELF1] 1-4 to 1-8.
   This appears at the very beginning of an ELF binary. */
struct Elf32_Ehdr {
  unsigned char e_ident[16];
  Elf32_Half e_type;
  Elf32_Half e_machine;
  Elf32_Word e_version;
  Elf32_Addr e_entry;
  Elf32_Off e_phoff;
  Elf32_Off e_shoff;
  Elf32_Word e_flags;
  Elf32_Half e_ehsize;
  Elf32_Half e_phentsize;
  Elf32_Half e_phnum;
  Elf32_Half e_shentsize;
  Elf32_Half e_shnum;
  Elf32_Half e_shstrndx;
};

/* Program header.  See [ELF1] 2-2 to 2-4.
   There are e_phnum of these, starting at file offset e_phoff
   (see [ELF1] 1-6). */
struct Elf32_Phdr {
  Elf32_Word p_type;
  Elf32_Off p_offset;
  Elf32_Addr p_vaddr;
  Elf32_Addr p_paddr;
  Elf32_Word p_filesz;
  Elf32_Word p_memsz;
  Elf32_Word p_flags;
  Elf32_Word p_align;
};

/* Values for p_type.  See [ELF1] 2-3. */
#define PT_NULL 0           /* Ignore. */
#define PT_LOAD 1           /* Loadable segment. */
#define PT_DYNAMIC 2        /* Dynamic linking info. */
#define PT_INTERP 3         /* Name of dynamic loader. */
#define PT_NOTE 4           /* Auxiliary info. */
#define PT_SHLIB 5          /* Reserved. */
#define PT_PHDR 6           /* Program header table. */
#define PT_STACK 0x6474e551 /* Stack segment. */

/* Flags for p_flags.  See [ELF3] 2-3 and 2-4. */
#define PF_X 1 /* Executable. */
#define PF_W 2 /* Writable. */
#define PF_R 4 /* Readable. */

static bool setup_stack(void** esp);
static bool validate_segment(const struct Elf32_Phdr*, struct file*);
static bool load_segment(struct file* file, off_t ofs, uint8_t* upage, uint32_t read_bytes,
                         uint32_t zero_bytes, bool writable);

/* Loads an ELF executable from FILE_NAME into the current thread.
   Stores the executable's entry point into *EIP
   and its initial stack pointer into *ESP.
   Returns true if successful, false otherwise. */
bool load(const char* file_name, void (**eip)(void), void** esp) {
  struct thread* t = thread_current();
  struct Elf32_Ehdr ehdr;
  struct file* file = NULL;
  off_t file_ofs;
  bool success = false;
  int i;

  /* Allocate and activate page directory. */
  t->pcb->pagedir = pagedir_create();
  if (t->pcb->pagedir == NULL)
    goto done;
  process_activate();

  /* Open executable file. */
  file = filesys_open(file_name);
  if (file == NULL) {
    printf("load: %s: open failed\n", file_name);
    goto done;
  }

  t->pcb->exe_file=file;
  file_deny_write(t->pcb->exe_file);


  /* Read and verify executable header. */
  if (file_read(file, &ehdr, sizeof ehdr) != sizeof ehdr ||
      memcmp(ehdr.e_ident, "\177ELF\1\1\1", 7) || ehdr.e_type != 2 || ehdr.e_machine != 3 ||
      ehdr.e_version != 1 || ehdr.e_phentsize != sizeof(struct Elf32_Phdr) || ehdr.e_phnum > 1024) {
    printf("load: %s: error loading executable\n", file_name);
    goto done;
  }

  /* Read program headers. */
  file_ofs = ehdr.e_phoff;
  for (i = 0; i < ehdr.e_phnum; i++) {
    struct Elf32_Phdr phdr;

    if (file_ofs < 0 || file_ofs > file_length(file))
      goto done;
    file_seek(file, file_ofs);

    if (file_read(file, &phdr, sizeof phdr) != sizeof phdr)
      goto done;
    file_ofs += sizeof phdr;
    switch (phdr.p_type) {
      case PT_NULL:
      case PT_NOTE:
      case PT_PHDR:
      case PT_STACK:
      default:
        /* Ignore this segment. */
        break;
      case PT_DYNAMIC:
      case PT_INTERP:
      case PT_SHLIB:
        goto done;
      case PT_LOAD:
        if (validate_segment(&phdr, file)) {
          bool writable = (phdr.p_flags & PF_W) != 0;
          uint32_t file_page = phdr.p_offset & ~PGMASK;
          uint32_t mem_page = phdr.p_vaddr & ~PGMASK;
          uint32_t page_offset = phdr.p_vaddr & PGMASK;
          uint32_t read_bytes, zero_bytes;
          if (phdr.p_filesz > 0) {
            /* Normal segment.
                     Read initial part from disk and zero the rest. */
            read_bytes = page_offset + phdr.p_filesz;
            zero_bytes = (ROUND_UP(page_offset + phdr.p_memsz, PGSIZE) - read_bytes);
          } else {
            /* Entirely zero.
                     Don't read anything from disk. */
            read_bytes = 0;
            zero_bytes = ROUND_UP(page_offset + phdr.p_memsz, PGSIZE);
          }
          if (!load_segment(file, file_page, (void*)mem_page, read_bytes, zero_bytes, writable))
            goto done;
        } else
          goto done;
        break;
    }
  }

  /* Set up stack. */
  if (!setup_stack(esp))
    goto done;

  /* Start address. */
  *eip = (void (*)(void))ehdr.e_entry;

  success = true;

done:
  /* We arrive here whether the load is successful or not. */
  return success;
}

/* load() helpers. */

static bool install_page(void* upage, void* kpage, bool writable);

/* Checks whether PHDR describes a valid, loadable segment in
   FILE and returns true if so, false otherwise. */
static bool validate_segment(const struct Elf32_Phdr* phdr, struct file* file) {
  /* p_offset and p_vaddr must have the same page offset. */
  if ((phdr->p_offset & PGMASK) != (phdr->p_vaddr & PGMASK))
    return false;

  /* p_offset must point within FILE. */
  if (phdr->p_offset > (Elf32_Off)file_length(file))
    return false;

  /* p_memsz must be at least as big as p_filesz. */
  if (phdr->p_memsz < phdr->p_filesz)
    return false;

  /* The segment must not be empty. */
  if (phdr->p_memsz == 0)
    return false;

  /* The virtual memory region must both start and end within the
     user address space range. */
  if (!is_user_vaddr((void*)phdr->p_vaddr))
    return false;
  if (!is_user_vaddr((void*)(phdr->p_vaddr + phdr->p_memsz)))
    return false;

  /* The region cannot "wrap around" across the kernel virtual
     address space. */
  if (phdr->p_vaddr + phdr->p_memsz < phdr->p_vaddr)
    return false;

  /* Disallow mapping page 0.
     Not only is it a bad idea to map page 0, but if we allowed
     it then user code that passed a null pointer to system calls
     could quite likely panic the kernel by way of null pointer
     assertions in memcpy(), etc. */
  if (phdr->p_vaddr < PGSIZE)
    return false;

  /* It's okay. */
  return true;
}

/* Loads a segment starting at offset OFS in FILE at address
   UPAGE.  In total, READ_BYTES + ZERO_BYTES bytes of virtual
   memory are initialized, as follows:

        - READ_BYTES bytes at UPAGE must be read from FILE
          starting at offset OFS.

        - ZERO_BYTES bytes at UPAGE + READ_BYTES must be zeroed.

   The pages initialized by this function must be writable by the
   user process if WRITABLE is true, read-only otherwise.

   Return true if successful, false if a memory allocation error
   or disk read error occurs. */
static bool load_segment(struct file* file, off_t ofs, uint8_t* upage, uint32_t read_bytes,
                         uint32_t zero_bytes, bool writable) {
  ASSERT((read_bytes + zero_bytes) % PGSIZE == 0);
  ASSERT(pg_ofs(upage) == 0);
  ASSERT(ofs % PGSIZE == 0);

  file_seek(file, ofs);
  while (read_bytes > 0 || zero_bytes > 0) {
    /* Calculate how to fill this page.
         We will read PAGE_READ_BYTES bytes from FILE
         and zero the final PAGE_ZERO_BYTES bytes. */
    size_t page_read_bytes = read_bytes < PGSIZE ? read_bytes : PGSIZE;
    size_t page_zero_bytes = PGSIZE - page_read_bytes;

    /* Get a page of memory. */
    uint8_t* kpage = palloc_get_page(PAL_USER);
    if (kpage == NULL)
      return false;

    /* Load this page. */
    if (file_read(file, kpage, page_read_bytes) != (int)page_read_bytes) {
      palloc_free_page(kpage);
      return false;
    }
    memset(kpage + page_read_bytes, 0, page_zero_bytes);

    /* Add the page to the process's address space. */
    if (!install_page(upage, kpage, writable)) {
      palloc_free_page(kpage);
      return false;
    }

    /* Advance. */
    read_bytes -= page_read_bytes;
    zero_bytes -= page_zero_bytes;
    upage += PGSIZE;
  }
  return true;
}

/* Create a minimal stack by mapping a zeroed page at the top of
   user virtual memory. */
static bool setup_stack(void** esp) {
  uint8_t* kpage;
  bool success = false;
  struct thread* t=thread_current();

  kpage = palloc_get_page(PAL_USER | PAL_ZERO);
  if (kpage != NULL) {
    success = install_page(((uint8_t*)t->pcb->next_thread_stack) - PGSIZE, kpage, true);
    if (success){
      *esp = t->pcb->next_thread_stack;
      t->pcb->next_thread_stack-=PGSIZE;
    }else
      palloc_free_page(kpage);
  }
  return success;
}

/* Adds a mapping from user virtual address UPAGE to kernel
   virtual address KPAGE to the page table.
   If WRITABLE is true, the user process may modify the page;
   otherwise, it is read-only.
   UPAGE must not already be mapped.
   KPAGE should probably be a page obtained from the user pool
   with palloc_get_page().
   Returns true on success, false if UPAGE is already mapped or
   if memory allocation fails. */
static bool install_page(void* upage, void* kpage, bool writable) {
  struct thread* t = thread_current();

  /* Verify that there's not already a page at that virtual
     address, then map our page there. */
  return (pagedir_get_page(t->pcb->pagedir, upage) == NULL &&
          pagedir_set_page(t->pcb->pagedir, upage, kpage, writable));
}

/* Returns true if t is the main thread of the process p */
bool is_main_thread(struct thread* t, struct process* p) { 
  return 
    list_entry(list_front(&p->threads),struct thread,pcb_elem)==t;
}

/* Gets the PID of a process */
pid_t get_pid(struct process* p) { 
  return 
    list_entry(list_front(&p->threads),struct thread,pcb_elem)->tid;
}

/* mycode: find the thread by id */
struct thread* pthread_self(tid_t id)
{
  struct list_elem*e;
  struct thread* cur=thread_current();
  if(id==TID_ERROR||list_empty(&cur->pcb->threads)){
    return NULL;
  }
  int size=list_size(&cur->pcb->threads);
  for(e=list_begin(&cur->pcb->threads);
      e!=list_end(&cur->pcb->threads);e=list_next(e))
  {
    struct thread* t=list_entry(e,struct thread,pcb_elem);
    if(t->tid==id){
      return t;
    }
  }
  return NULL;
}

struct pthread_resource
{
  stub_fun sf;
  pthread_fun tf;
  void* arg;
  struct thread* main_thread;
};


/* Creates a new stack for the thread and sets up its arguments.
   Stores the thread's entry point into *EIP and its initial stack
   pointer into *ESP. Handles all cleanup if unsuccessful. Returns
   true if successful, false otherwise.

   This function will be implemented in Project 2: Multithreading. For
   now, it does nothing. You may find it necessary to change the
   function signature. */
bool setup_thread(struct pthread_resource* resource,void (**eip)(void) UNUSED, void** esp UNUSED) { 
  stub_fun sf=resource->sf;
  pthread_fun tf=resource->tf;
  void* arg=resource->arg;
  bool success=setup_stack(esp);

  if(success){
    *eip=sf;
    /* fill up the stack */
    *esp-=sizeof(void*);
    memcpy(*esp,&arg,sizeof(arg));

    *esp-=sizeof(pthread_fun);
    memcpy(*esp,&tf,sizeof(pthread_fun));

    *esp-=sizeof(void*);
    *(uint32_t*)*esp=0;
  
  }
  return success;
}

/* Starts a new thread with a new user stack running SF, which takes
   TF and ARG as arguments on its user stack. This new thread may be
   scheduled (and may even exit) before pthread_execute () returns.
   Returns the new thread's TID or TID_ERROR if the thread cannot
   be created properly.

   This function will be implemented in Project 2: Multithreading and
   should be similar to process_execute (). For now, it does nothing.
   */
tid_t pthread_execute(stub_fun sf UNUSED, pthread_fun tf UNUSED, void* arg UNUSED) { 
  struct thread* t=thread_current();
  struct pthread_resource* resource=palloc_get_page(0);
  resource->sf=sf;
  resource->tf=tf;
  resource->arg=arg;
  resource->main_thread=t;

  enum intr_level old_level=intr_disable();

  tid_t tid=thread_create(t->name,PRI_DEFAULT,start_pthread,resource);

  if(tid==TID_ERROR){
    palloc_free_page(resource);
  }else{
    struct thread* p=thread_self(tid);
    list_push_back(&t->pcb->threads,&p->pcb_elem);
  }

  intr_set_level(old_level);
  return tid;
}


/* A thread function that creates a new user thread and starts it
   running. Responsible for adding itself to the list of threads in
   the PCB.

   This function will be implemented in Project 2: Multithreading and
   should be similar to start_process (). For now, it does nothing. */
static void start_pthread(void* exec_ UNUSED) 
{
  struct thread* t=thread_current();
  struct pthread_resource* resource=
    (struct pthread_resource*)exec_;
  struct intr_frame if_;
  bool success=false;
  struct thread* main_thread=resource->main_thread;

  t->pcb=main_thread->pcb;
  t->father=main_thread->father;
  //list_push_back(&main_thread->pcb->threads,&t->pcb_elem);
  int main_size=list_size(&t->pcb->threads);
  int size=list_size(&t->pcb->threads);

  /* Initialize interrupt frame and load executable. */
  memset(&if_, 0, sizeof if_);
  if_.gs = if_.fs = if_.es = if_.ds = if_.ss = SEL_UDSEG;
  if_.cs = SEL_UCSEG;
  if_.eflags = FLAG_IF | FLAG_MBS;
  success = setup_thread(resource,&if_.eip,&if_.esp);

  if(!success){
    pthread_exit();
  }

  palloc_free_page(resource);

  asm("fninit;fsave (%0)"::"g"(&t->fpu));

  asm volatile("movl %0,%%esp; jmp intr_exit"::"g"(&if_):"memory");

  NOT_REACHED();
}

/* Waits for thread with TID to die, if that thread was spawned
   in the same process and has not been waited on yet. Returns TID on
   success and returns TID_ERROR on failure immediately, without
   waiting.

   This function will be implemented in Project 2: Multithreading. For
   now, it does nothing. */
tid_t pthread_join(tid_t tid UNUSED) { 
  struct thread* waiter=thread_current();
  struct thread* waitee;
  struct semaphore_elem sema;
  
  if(waiter->tid==tid){
    return TID_ERROR;
  }

  enum intr_level old_level=intr_disable();
  struct list_elem* e;
  for(e=list_begin(&waiter->pcb->exit_threads);
      e!=list_end(&waiter->pcb->exit_threads);
      e=list_next(e))
  {
    struct exit_pthread_tid* exit_tid=
      list_entry(e,struct exit_pthread_tid,elem);
    if(exit_tid->dead_tid==tid){
      intr_set_level(old_level);
      return tid;
    }
  }
  intr_set_level(old_level);
  waitee=pthread_self(tid);
  if(!waitee){
    return TID_ERROR;
  }




  if(waitee){
    /* init sema for using */
    sema_init(&sema.semaphore,0);
    int size1=list_size(&waitee->join_waiter_list);
    /* register the waiter name in heart of waitee */
    list_push_back(&waitee->join_waiter_list,&sema);
    int size2=list_size(&waitee->join_waiter_list);
    /* waiting here */
    sema_down(&sema.semaphore);
    return tid;
  }
  /* already be joined */
  return -1;
}

/* mycode: pthread begin to exit and wakeup the waiters */
static bool pthread_waited(struct thread* waitee)
{
  ASSERT(waitee);
  if(list_empty(&waitee->join_waiter_list)){
    return true;
  }
  int size=list_size(&waitee->join_waiter_list);
  enum intr_level old_level=intr_disable();
  for(struct list_elem* e=list_begin(&waitee->join_waiter_list);
      e!=list_end(&waitee->join_waiter_list);
      e=list_next(e))
  {
    struct semaphore_elem* elem=list_entry(e,struct semaphore_elem,elem);
    sema_up(&elem->semaphore);
  }
  intr_set_level(old_level);
  size=list_size(&waitee->join_waiter_list);
  if(size>0){
    return false;
  }else{
    return true;
  }
}

/* mycode: maybe the main_thread has been waitting the thread_exit  */
static void pthread_exited(struct thread* exitee)
{
  if(exitee->exit_monitor){
    struct pthread_exit_monitor* monitor=exitee->exit_monitor;
    monitor->value--;
    sema_up(&monitor->sema);
  }

}

static void pthread_lock_release(struct thread* t)
{
  while(!list_empty(&t->holding_lock_list))
  {
    struct list_elem* e=list_pop_front(&t->holding_lock_list);
    struct lock* lock=list_entry(e,struct lock,elem);
    lock_release(lock);
  }
}

/* Free the current thread's resources. Most resources will
   be freed on thread_exit(), so all we have to do is deallocate the
   thread's userspace stack. Wake any waiters on this thread.

   The main thread should not use this function. See
   pthread_exit_main() below.

   This function will be implemented in Project 2: Multithreading. For
   now, it does nothing. */
void pthread_exit(void) 
{
  struct thread* t=thread_current();
  ASSERT(!is_main_thread(t,t->pcb));
 

  intr_disable();
  
  /* remove from the pcb->threads */
  list_remove(&t->pcb_elem);
  
  /* add to the pcb->exit_threads */
  struct exit_pthread_tid* exit_tid=
    malloc(sizeof(struct exit_pthread_tid));
  exit_tid->dead_tid=t->tid;
  list_push_back(&t->pcb->exit_threads,&exit_tid->elem);
 
  /* wakeup all the waiter */
  pthread_waited(t);

  /* release all lock */
  pthread_lock_release(t);

  /* deallocate the userspace stack */
  //palloc_free_page(pg_round_down(t->user_stack-1));

  /* wakeup the exiter */
  pthread_exited(t);

  thread_exit();
  NOT_REACHED();
}


/* Only to be used when the main thread explicitly calls pthread_exit.
   The main thread should wait on all threads in the process to
   terminate properly, before exiting itself. When it exits itself, it
   must terminate the process in addition to all necessary duties in
   pthread_exit.

   This function will be implemented in Project 2: Multithreading. For
   now, it does nothing. */
void pthread_exit_main(void) 
{
  struct thread* t=thread_current();
  ASSERT(is_main_thread(t,t->pcb));
  struct pthread_exit_monitor monitor;

  enum intr_level old_level=intr_disable();
  t->exit_monitor=&monitor;
  /* add to the pcb->exit_threads */
  struct exit_pthread_tid* exit_tid=
    malloc(sizeof(struct exit_pthread_tid));
  exit_tid->dead_tid=t->tid;
  list_push_back(&t->pcb->exit_threads,&exit_tid->elem);

  
  
  /* wakeup all the waiter */
  pthread_waited(t);
 
  /* release all lock */
  pthread_lock_release(t);

  int size=list_size(&t->pcb->threads);
  monitor_init(&monitor,size-1);
  if(size>1){
    struct list_elem* elem;
    for(elem=list_rbegin(&t->pcb->threads);
        elem!=list_begin(&t->pcb->threads);
        elem=list_prev(elem))
    {
      struct thread* thread=list_entry(elem,struct thread,pcb_elem);
      thread->exit_monitor=&monitor;
    }
  }

  int debug_timer=0;
  while(monitor.value!=0){
    sema_down(&monitor.sema);
    debug_timer++;
  }
  intr_set_level(old_level);

  printf("%s: exit(%d)\n",t->pcb->process_name,t->pcb->exit_return);
  process_exit();
  NOT_REACHED();
}



