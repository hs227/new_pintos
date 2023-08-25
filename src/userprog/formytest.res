pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/args-single -a args-single -- -q   -f run 'args-single onearg' < /dev/null 2> tests/userprog/args-single.errors > tests/userprog/args-single.output
perl -I../.. ../../tests/userprog/args-single.ck tests/userprog/args-single tests/userprog/args-single.result
FAIL tests/userprog/args-single
Kernel panic in run: PANIC at ../../userprog/exception.c:97 in kill(): Kernel bug - unexpected interrupt in kernel
Call stack: 0xc002b5a8 0xc00305d1 0xc00306ef 0xc002256a 0xc00227c0 0xc0033834 0xc0032971 0xc002de51 0xc00203a5
Translation of call stack:
0xc002b5a8: debug_panic (pintos/src/lib/kernel/debug.c:35)
0xc00305d1: kill (pintos/src/userprog/exception.c:102)
0xc00306ef: page_fault (pintos/src/userprog/exception.c:152)
0xc002256a: intr_handler (pintos/src/threads/interrupt.c:339)
0xc00227c0: intr_entry (threads/intr-stubs.S:38)
0xc0033834: inode_open (pintos/src/filesys/inode.c:275)
0xc0032971: dir_open_root (pintos/src/filesys/directory.c:57)
0xc002de51: userprog_init (pintos/src/userprog/process.c:63)
0xc00203a5: main (pintos/src/threads/init.c:128)
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/args-none -a args-none -- -q   -f run args-none < /dev/null 2> tests/userprog/args-none.errors > tests/userprog/args-none.output
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/args-multiple -a args-multiple -- -q   -f run 'args-multiple some arguments for you!' < /dev/null 2> tests/userprog/args-multiple.errors > tests/userprog/args-multiple.output
