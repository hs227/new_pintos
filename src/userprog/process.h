#ifndef USERPROG_PROCESS_H
#define USERPROG_PROCESS_H

#include "threads/thread.h"
#include <stdint.h>

// At most 8MB can be allocated to the stack
// These defines will be used in Project 2: Multithreading
#define MAX_STACK_PAGES (1 << 11)
#define MAX_THREADS 127

/* PIDs and TIDs are the same type. PID should be
   the TID of the main thread of the process */
typedef tid_t pid_t;

/* Thread functions (Project 2: Multithreading) */
typedef void (*pthread_fun)(void*);
typedef void (*stub_fun)(pthread_fun, void*);

#define FD_TABLE_SIZE 16
struct file_descriptor{
  uint8_t fd_flags;
  struct file* file_ptr;
};

struct file_descriptor_table{
  struct file_descriptor fds[FD_TABLE_SIZE];
  uint8_t cur;// point to next empty

};

int next_empty_fdt(struct file_descriptor_table* fd_table);
struct file* release_fd_fdt(struct file_descriptor_table* fd_table,int fd);
struct file* get_file_fdt(struct file_descriptor_table* fd_table,int fd);


#define CP_TABLE_SIZE 16

struct child_proc{
  tid_t id;
  int child_res;
};

struct child_proc_table{
  struct child_proc cps[CP_TABLE_SIZE];
  uint8_t cur;// point to next empty
};

int next_empty_child_cpt(struct child_proc_table* cp_table);
int child_return_cpt(struct child_proc_table* cp_table,struct process* child,int res);
int father_wait_cpt(struct child_proc_table* cp_table,struct process* father,pid_t id);

struct exit_pthread_tid
{
  tid_t dead_tid;
  struct list_elem elem;
};


/* The process control block for a given process. Since
   there can be multiple threads per process, we need a separate
   PCB from the TCB. All TCBs in a process will have a pointer
   to the PCB, and the PCB will have a pointer to the main thread
   of the process, which is `special`. */
struct process {
  /* Owned by process.c. */
  uint32_t* pagedir;          /* Page directory. */
  char process_name[16];      /* Name of the main thread */

  struct list threads;

  uint32_t next_thread_stack;

  struct semaphore sema;

  struct file_descriptor_table fd_table;
  struct child_proc_table cp_table;
  int exit_return;
  struct list exit_threads;

  struct file* exe_file;
};


void userprog_init(void);

pid_t process_execute(const char* file_name);
int process_wait(pid_t);
void process_exit(void);
void process_activate(void);

bool is_main_thread(struct thread*, struct process*);
pid_t get_pid(struct process*);

tid_t pthread_execute(stub_fun, pthread_fun, void*);
tid_t pthread_join(tid_t);
void pthread_exit(void);
void pthread_exit_main(void);

#endif /* userprog/process.h */
