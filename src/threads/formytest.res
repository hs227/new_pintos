pintos -v -k -T 60 --bochs  --filesys-size=2  -- -q  -sched=fifo -f rtkt alarm-negative < /dev/null 2> tests/threads/alarm-negative.errors > tests/threads/alarm-negative.output
perl -I../.. ../../tests/threads/alarm-negative.ck tests/threads/alarm-negative tests/threads/alarm-negative.result
pass tests/threads/alarm-negative
pintos -v -k -T 60 --bochs  --filesys-size=2  -- -q  -sched=fifo -f rtkt alarm-simultaneous < /dev/null 2> tests/threads/alarm-simultaneous.errors > tests/threads/alarm-simultaneous.output
perl -I../.. ../../tests/threads/alarm-simultaneous.ck tests/threads/alarm-simultaneous tests/threads/alarm-simultaneous.result
pass tests/threads/alarm-simultaneous
pintos -v -k -T 60 --bochs  --filesys-size=2  -- -q  -sched=fifo -f rtkt alarm-single < /dev/null 2> tests/threads/alarm-single.errors > tests/threads/alarm-single.output
perl -I../.. ../../tests/threads/alarm-single.ck tests/threads/alarm-single tests/threads/alarm-single.result
pass tests/threads/alarm-single
pintos -v -k -T 60 --bochs  --filesys-size=2  -- -q  -sched=fifo -f rtkt alarm-zero < /dev/null 2> tests/threads/alarm-zero.errors > tests/threads/alarm-zero.output
perl -I../.. ../../tests/threads/alarm-zero.ck tests/threads/alarm-zero tests/threads/alarm-zero.result
pass tests/threads/alarm-zero
pintos -v -k -T 60 --bochs  --filesys-size=2  -- -q  -sched=fifo -f rtkt alarm-multiple < /dev/null 2> tests/threads/alarm-multiple.errors > tests/threads/alarm-multiple.output
perl -I../.. ../../tests/threads/alarm-multiple.ck tests/threads/alarm-multiple tests/threads/alarm-multiple.result
pass tests/threads/alarm-multiple
pintos -v -k -T 60 --bochs  --filesys-size=2  -- -q  -sched=prio -f rtkt alarm-priority < /dev/null 2> tests/threads/alarm-priority.errors > tests/threads/alarm-priority.output
perl -I../.. ../../tests/threads/alarm-priority.ck tests/threads/alarm-priority tests/threads/alarm-priority.result
pass tests/threads/alarm-priority
pintos -v -k -T 60 --bochs  --filesys-size=2  -- -q  -sched=prio -f rtkt priority-donate-one < /dev/null 2> tests/threads/priority-donate-one.errors > tests/threads/priority-donate-one.output
perl -I../.. ../../tests/threads/priority-donate-one.ck tests/threads/priority-donate-one tests/threads/priority-donate-one.result
pass tests/threads/priority-donate-one
pintos -v -k -T 60 --bochs  --filesys-size=2  -- -q  -sched=prio -f rtkt priority-preempt < /dev/null 2> tests/threads/priority-preempt.errors > tests/threads/priority-preempt.output
perl -I../.. ../../tests/threads/priority-preempt.ck tests/threads/priority-preempt tests/threads/priority-preempt.result
pass tests/threads/priority-preempt
pintos -v -k -T 60 --bochs  --filesys-size=2  -- -q  -sched=prio -f rtkt priority-starve < /dev/null 2> tests/threads/priority-starve.errors > tests/threads/priority-starve.output
perl -I../.. ../../tests/threads/priority-starve.ck tests/threads/priority-starve tests/threads/priority-starve.result
pass tests/threads/priority-starve
pintos -v -k -T 60 --bochs  --filesys-size=2  -- -q  -sched=prio -f rtkt priority-starve-sema < /dev/null 2> tests/threads/priority-starve-sema.errors > tests/threads/priority-starve-sema.output
perl -I../.. ../../tests/threads/priority-starve-sema.ck tests/threads/priority-starve-sema tests/threads/priority-starve-sema.result
pass tests/threads/priority-starve-sema
pintos -v -k -T 60 --bochs  --filesys-size=2  -- -q  -sched=prio -f rtkt priority-sema < /dev/null 2> tests/threads/priority-sema.errors > tests/threads/priority-sema.output
perl -I../.. ../../tests/threads/priority-sema.ck tests/threads/priority-sema tests/threads/priority-sema.result
pass tests/threads/priority-sema
pintos -v -k -T 60 --bochs  --filesys-size=2  -- -q  -sched=prio -f rtkt priority-condvar < /dev/null 2> tests/threads/priority-condvar.errors > tests/threads/priority-condvar.output
perl -I../.. ../../tests/threads/priority-condvar.ck tests/threads/priority-condvar tests/threads/priority-condvar.result
pass tests/threads/priority-condvar
pintos -v -k -T 60 --bochs  --filesys-size=2  -- -q  -sched=prio -f rtkt priority-change < /dev/null 2> tests/threads/priority-change.errors > tests/threads/priority-change.output
perl -I../.. ../../tests/threads/priority-change.ck tests/threads/priority-change tests/threads/priority-change.result
pass tests/threads/priority-change
pintos -v -k -T 60 --bochs  --filesys-size=2  -- -q  -sched=prio -f rtkt priority-donate-chain < /dev/null 2> tests/threads/priority-donate-chain.errors > tests/threads/priority-donate-chain.output
perl -I../.. ../../tests/threads/priority-donate-chain.ck tests/threads/priority-donate-chain tests/threads/priority-donate-chain.result
pass tests/threads/priority-donate-chain
pintos -v -k -T 60 --bochs  --filesys-size=2  -- -q  -sched=prio -f rtkt priority-donate-lower < /dev/null 2> tests/threads/priority-donate-lower.errors > tests/threads/priority-donate-lower.output
perl -I../.. ../../tests/threads/priority-donate-lower.ck tests/threads/priority-donate-lower tests/threads/priority-donate-lower.result
pass tests/threads/priority-donate-lower
pintos -v -k -T 60 --bochs  --filesys-size=2  -- -q  -sched=prio -f rtkt priority-fifo < /dev/null 2> tests/threads/priority-fifo.errors > tests/threads/priority-fifo.output
perl -I../.. ../../tests/threads/priority-fifo.ck tests/threads/priority-fifo tests/threads/priority-fifo.result
pass tests/threads/priority-fifo
pintos -v -k -T 60 --bochs  --filesys-size=2  -- -q  -sched=prio -f rtkt priority-donate-nest < /dev/null 2> tests/threads/priority-donate-nest.errors > tests/threads/priority-donate-nest.output
perl -I../.. ../../tests/threads/priority-donate-nest.ck tests/threads/priority-donate-nest tests/threads/priority-donate-nest.result
pass tests/threads/priority-donate-nest
pintos -v -k -T 60 --bochs  --filesys-size=2  -- -q  -sched=prio -f rtkt priority-donate-sema < /dev/null 2> tests/threads/priority-donate-sema.errors > tests/threads/priority-donate-sema.output
perl -I../.. ../../tests/threads/priority-donate-sema.ck tests/threads/priority-donate-sema tests/threads/priority-donate-sema.result
pass tests/threads/priority-donate-sema
pintos -v -k -T 60 --bochs  --filesys-size=2  -- -q  -sched=prio -f rtkt priority-donate-multiple < /dev/null 2> tests/threads/priority-donate-multiple.errors > tests/threads/priority-donate-multiple.output
perl -I../.. ../../tests/threads/priority-donate-multiple.ck tests/threads/priority-donate-multiple tests/threads/priority-donate-multiple.result
pass tests/threads/priority-donate-multiple
pintos -v -k -T 60 --bochs  --filesys-size=2  -- -q  -sched=prio -f rtkt priority-donate-multiple2 < /dev/null 2> tests/threads/priority-donate-multiple2.errors > tests/threads/priority-donate-multiple2.output
perl -I../.. ../../tests/threads/priority-donate-multiple2.ck tests/threads/priority-donate-multiple2 tests/threads/priority-donate-multiple2.result
pass tests/threads/priority-donate-multiple2
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/create-simple -a create-simple -- -q   -f run create-simple < /dev/null 2> tests/userprog/multithreading/create-simple.errors > tests/userprog/multithreading/create-simple.output
perl -I../.. ../../tests/userprog/multithreading/create-simple.ck tests/userprog/multithreading/create-simple tests/userprog/multithreading/create-simple.result
pass tests/userprog/multithreading/create-simple
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/create-many -a create-many -- -q   -f run create-many < /dev/null 2> tests/userprog/multithreading/create-many.errors > tests/userprog/multithreading/create-many.output
perl -I../.. ../../tests/userprog/multithreading/create-many.ck tests/userprog/multithreading/create-many tests/userprog/multithreading/create-many.result
pass tests/userprog/multithreading/create-many
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/create-reuse -a create-reuse -- -q   -f run create-reuse < /dev/null 2> tests/userprog/multithreading/create-reuse.errors > tests/userprog/multithreading/create-reuse.output
perl -I../.. ../../tests/userprog/multithreading/create-reuse.ck tests/userprog/multithreading/create-reuse tests/userprog/multithreading/create-reuse.result
pass tests/userprog/multithreading/create-reuse
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exec-thread-1 -a exec-thread-1 -- -q   -f run 'exec-thread-1 thread' < /dev/null 2> tests/userprog/multithreading/exec-thread-1.errors > tests/userprog/multithreading/exec-thread-1.output
perl -I../.. ../../tests/userprog/multithreading/exec-thread-1.ck tests/userprog/multithreading/exec-thread-1 tests/userprog/multithreading/exec-thread-1.result
pass tests/userprog/multithreading/exec-thread-1
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exec-thread-n -a exec-thread-n -- -q   -f run 'exec-thread-n root' < /dev/null 2> tests/userprog/multithreading/exec-thread-n.errors > tests/userprog/multithreading/exec-thread-n.output
perl -I../.. ../../tests/userprog/multithreading/exec-thread-n.ck tests/userprog/multithreading/exec-thread-n tests/userprog/multithreading/exec-thread-n.result
pass tests/userprog/multithreading/exec-thread-n
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exit-simple -a exit-simple -- -q   -f run exit-simple < /dev/null 2> tests/userprog/multithreading/exit-simple.errors > tests/userprog/multithreading/exit-simple.output
perl -I../.. ../../tests/userprog/multithreading/exit-simple.ck tests/userprog/multithreading/exit-simple tests/userprog/multithreading/exit-simple.result
pass tests/userprog/multithreading/exit-simple
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/create-simple -a create-simple -- -q   -f run create-simple < /dev/null 2> tests/userprog/multithreading/create-simple.errors > tests/userprog/multithreading/create-simple.output
perl -I../.. ../../tests/userprog/multithreading/create-simple.ck tests/userprog/multithreading/create-simple tests/userprog/multithreading/create-simple.result
pass tests/userprog/multithreading/create-simple
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/create-many -a create-many -- -q   -f run create-many < /dev/null 2> tests/userprog/multithreading/create-many.errors > tests/userprog/multithreading/create-many.output
perl -I../.. ../../tests/userprog/multithreading/create-many.ck tests/userprog/multithreading/create-many tests/userprog/multithreading/create-many.result
pass tests/userprog/multithreading/create-many
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/create-reuse -a create-reuse -- -q   -f run create-reuse < /dev/null 2> tests/userprog/multithreading/create-reuse.errors > tests/userprog/multithreading/create-reuse.output
perl -I../.. ../../tests/userprog/multithreading/create-reuse.ck tests/userprog/multithreading/create-reuse tests/userprog/multithreading/create-reuse.result
pass tests/userprog/multithreading/create-reuse
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exec-thread-1 -a exec-thread-1 -- -q   -f run 'exec-thread-1 thread' < /dev/null 2> tests/userprog/multithreading/exec-thread-1.errors > tests/userprog/multithreading/exec-thread-1.output
perl -I../.. ../../tests/userprog/multithreading/exec-thread-1.ck tests/userprog/multithreading/exec-thread-1 tests/userprog/multithreading/exec-thread-1.result
FAIL tests/userprog/multithreading/exec-thread-1
Kernel panic in run: PANIC at ../../lib/kernel/list.c:72 in list_next(): assertion `is_head(elem) || is_interior(elem)' failed.
Call stack: 0xc002b087 0xc002b387 0xc002b8a4 0xc002efa9 0xc002f0a0 0xc0030b53 0xc003045a 0xc00225b0 0xc0022806 0x804ae79 0x804b018 0x804b047
Translation of call stack:
In kernel.o:
0xc002b087: debug_panic (/home/vagrant/mycode/group/group0/src/lib/kernel/debug.c:35)
0xc002b387: list_next (/home/vagrant/mycode/group/group0/src/lib/kernel/list.c:73)
0xc002b8a4: list_size (/home/vagrant/mycode/group/group0/src/lib/kernel/list.c:258)
0xc002efa9: pthread_exited (/home/vagrant/mycode/group/group0/src/userprog/process.c:1034)
0xc002f0a0: pthread_exit (/home/vagrant/mycode/group/group0/src/userprog/process.c:1073)
0xc0030b53: syscall_pt_exit (/home/vagrant/mycode/group/group0/src/userprog/syscall.c:496)
0xc003045a: syscall_handler (/home/vagrant/mycode/group/group0/src/userprog/syscall.c:230)
0xc00225b0: intr_handler (/home/vagrant/mycode/group/group0/src/threads/interrupt.c:339)
0xc0022806: intr_entry (threads/intr-stubs.S:38)
In tests/userprog/multithreading/exec-thread-1:
0x0804ae79: sys_pthread_exit (/home/vagrant/mycode/group/group0/src/lib/user/syscall.c:126)
0x0804b018: pthread_join (/home/vagrant/mycode/group/group0/src/lib/user/pthread.c:21)
0x0804b047: vprintf (/home/vagrant/mycode/group/group0/src/lib/user/console.c:8)
Translations of user virtual addresses above are based on a guess at
the binary to use.  If this guess is incorrect, then those
translations will be misleading.
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exec-thread-n -a exec-thread-n -- -q   -f run 'exec-thread-n root' < /dev/null 2> tests/userprog/multithreading/exec-thread-n.errors > tests/userprog/multithreading/exec-thread-n.output
perl -I../.. ../../tests/userprog/multithreading/exec-thread-n.ck tests/userprog/multithreading/exec-thread-n tests/userprog/multithreading/exec-thread-n.result
pass tests/userprog/multithreading/exec-thread-n
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exit-simple -a exit-simple -- -q   -f run exit-simple < /dev/null 2> tests/userprog/multithreading/exit-simple.errors > tests/userprog/multithreading/exit-simple.output
perl -I../.. ../../tests/userprog/multithreading/exit-simple.ck tests/userprog/multithreading/exit-simple tests/userprog/multithreading/exit-simple.result
pass tests/userprog/multithreading/exit-simple
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/create-simple -a create-simple -- -q   -f run create-simple < /dev/null 2> tests/userprog/multithreading/create-simple.errors > tests/userprog/multithreading/create-simple.output
perl -I../.. ../../tests/userprog/multithreading/create-simple.ck tests/userprog/multithreading/create-simple tests/userprog/multithreading/create-simple.result
pass tests/userprog/multithreading/create-simple
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/create-many -a create-many -- -q   -f run create-many < /dev/null 2> tests/userprog/multithreading/create-many.errors > tests/userprog/multithreading/create-many.output
perl -I../.. ../../tests/userprog/multithreading/create-many.ck tests/userprog/multithreading/create-many tests/userprog/multithreading/create-many.result
pass tests/userprog/multithreading/create-many
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/create-reuse -a create-reuse -- -q   -f run create-reuse < /dev/null 2> tests/userprog/multithreading/create-reuse.errors > tests/userprog/multithreading/create-reuse.output
perl -I../.. ../../tests/userprog/multithreading/create-reuse.ck tests/userprog/multithreading/create-reuse tests/userprog/multithreading/create-reuse.result
pass tests/userprog/multithreading/create-reuse
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exec-thread-1 -a exec-thread-1 -- -q   -f run 'exec-thread-1 thread' < /dev/null 2> tests/userprog/multithreading/exec-thread-1.errors > tests/userprog/multithreading/exec-thread-1.output
perl -I../.. ../../tests/userprog/multithreading/exec-thread-1.ck tests/userprog/multithreading/exec-thread-1 tests/userprog/multithreading/exec-thread-1.result
pass tests/userprog/multithreading/exec-thread-1
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exec-thread-n -a exec-thread-n -- -q   -f run 'exec-thread-n root' < /dev/null 2> tests/userprog/multithreading/exec-thread-n.errors > tests/userprog/multithreading/exec-thread-n.output
perl -I../.. ../../tests/userprog/multithreading/exec-thread-n.ck tests/userprog/multithreading/exec-thread-n tests/userprog/multithreading/exec-thread-n.result
pass tests/userprog/multithreading/exec-thread-n
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exit-simple -a exit-simple -- -q   -f run exit-simple < /dev/null 2> tests/userprog/multithreading/exit-simple.errors > tests/userprog/multithreading/exit-simple.output
perl -I../.. ../../tests/userprog/multithreading/exit-simple.ck tests/userprog/multithreading/exit-simple tests/userprog/multithreading/exit-simple.result
pass tests/userprog/multithreading/exit-simple
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/create-simple -a create-simple -- -q   -f run create-simple < /dev/null 2> tests/userprog/multithreading/create-simple.errors > tests/userprog/multithreading/create-simple.output
perl -I../.. ../../tests/userprog/multithreading/create-simple.ck tests/userprog/multithreading/create-simple tests/userprog/multithreading/create-simple.result
pass tests/userprog/multithreading/create-simple
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/create-many -a create-many -- -q   -f run create-many < /dev/null 2> tests/userprog/multithreading/create-many.errors > tests/userprog/multithreading/create-many.output
perl -I../.. ../../tests/userprog/multithreading/create-many.ck tests/userprog/multithreading/create-many tests/userprog/multithreading/create-many.result
pass tests/userprog/multithreading/create-many
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/create-reuse -a create-reuse -- -q   -f run create-reuse < /dev/null 2> tests/userprog/multithreading/create-reuse.errors > tests/userprog/multithreading/create-reuse.output
perl -I../.. ../../tests/userprog/multithreading/create-reuse.ck tests/userprog/multithreading/create-reuse tests/userprog/multithreading/create-reuse.result
pass tests/userprog/multithreading/create-reuse
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exec-thread-1 -a exec-thread-1 -- -q   -f run 'exec-thread-1 thread' < /dev/null 2> tests/userprog/multithreading/exec-thread-1.errors > tests/userprog/multithreading/exec-thread-1.output
perl -I../.. ../../tests/userprog/multithreading/exec-thread-1.ck tests/userprog/multithreading/exec-thread-1 tests/userprog/multithreading/exec-thread-1.result
pass tests/userprog/multithreading/exec-thread-1
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exec-thread-n -a exec-thread-n -- -q   -f run 'exec-thread-n root' < /dev/null 2> tests/userprog/multithreading/exec-thread-n.errors > tests/userprog/multithreading/exec-thread-n.output
perl -I../.. ../../tests/userprog/multithreading/exec-thread-n.ck tests/userprog/multithreading/exec-thread-n tests/userprog/multithreading/exec-thread-n.result
pass tests/userprog/multithreading/exec-thread-n
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exit-simple -a exit-simple -- -q   -f run exit-simple < /dev/null 2> tests/userprog/multithreading/exit-simple.errors > tests/userprog/multithreading/exit-simple.output
perl -I../.. ../../tests/userprog/multithreading/exit-simple.ck tests/userprog/multithreading/exit-simple tests/userprog/multithreading/exit-simple.result
pass tests/userprog/multithreading/exit-simple
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exit-clean-1 -a exit-clean-1 -p ../../tests/userprog/sample.txt -a sample.txt -- -q   -f run 'exit-clean-1 root exit 1' < /dev/null 2> tests/userprog/multithreading/exit-clean-1.errors > tests/userprog/multithreading/exit-clean-1.output
perl -I../.. ../../tests/userprog/multithreading/exit-clean-1.ck tests/userprog/multithreading/exit-clean-1 tests/userprog/multithreading/exit-clean-1.result
pass tests/userprog/multithreading/exit-clean-1
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exit-clean-2 -a exit-clean-2 -p ../../tests/userprog/sample.txt -a sample.txt -- -q   -f run 'exit-clean-2 root exception 2' < /dev/null 2> tests/userprog/multithreading/exit-clean-2.errors > tests/userprog/multithreading/exit-clean-2.output
perl -I../.. ../../tests/userprog/multithreading/exit-clean-2.ck tests/userprog/multithreading/exit-clean-2 tests/userprog/multithreading/exit-clean-2.result
pass tests/userprog/multithreading/exit-clean-2
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/create-simple -a create-simple -- -q   -f run create-simple < /dev/null 2> tests/userprog/multithreading/create-simple.errors > tests/userprog/multithreading/create-simple.output
perl -I../.. ../../tests/userprog/multithreading/create-simple.ck tests/userprog/multithreading/create-simple tests/userprog/multithreading/create-simple.result
FAIL tests/userprog/multithreading/create-simple
Kernel panic in run: PANIC at ../../userprog/exception.c:97 in kill(): Kernel bug - unexpected interrupt in kernel
Call stack: 0xc002b0dd 0xc002fe95 0xc002ffb3 0xc00225ad 0xc0022803 0xc002b388 0xc002b924 0xc002b84a 0xc002eb28 0xc002eea7 0xc0030b58 0xc003047c 0xc00225ad 0xc0022803 0x804ae1d 0x804af96 0x80483d2 0x80480dd 0x8048978 0x80489a3
Translation of call stack:
In kernel.o:
0xc002b0dd: debug_panic (/home/vagrant/mycode/group/group0/src/lib/kernel/debug.c:35)
0xc002fe95: kill (/home/vagrant/mycode/group/group0/src/userprog/exception.c:102)
0xc002ffb3: page_fault (/home/vagrant/mycode/group/group0/src/userprog/exception.c:152)
0xc00225ad: intr_handler (/home/vagrant/mycode/group/group0/src/threads/interrupt.c:339)
0xc0022803: intr_entry (threads/intr-stubs.S:38)
0xc002b388: list_begin (/home/vagrant/mycode/group/group0/src/lib/kernel/list.c:65)
0xc002b924: list_empty (/home/vagrant/mycode/group/group0/src/lib/kernel/list.c:264)
0xc002b84a: list_front (/home/vagrant/mycode/group/group0/src/lib/kernel/list.c:241)
0xc002eb28: is_main_thread (/home/vagrant/mycode/group/group0/src/userprog/process.c:838)
0xc002eea7: pthread_join (/home/vagrant/mycode/group/group0/src/userprog/process.c:995)
0xc0030b58: syscall_pt_join (/home/vagrant/mycode/group/group0/src/userprog/syscall.c:510)
0xc003047c: syscall_handler (/home/vagrant/mycode/group/group0/src/userprog/syscall.c:237)
0xc00225ad: intr_handler (/home/vagrant/mycode/group/group0/src/threads/interrupt.c:339)
0xc0022803: intr_entry (threads/intr-stubs.S:38)
In tests/userprog/multithreading/create-simple:
0x0804ae1d: sys_pthread_join (/home/vagrant/mycode/group/group0/src/lib/user/syscall.c:130)
0x0804af96: pthread_join (/home/vagrant/mycode/group/group0/src/lib/user/pthread.c:21)
0x080483d2: pthread_check_join (/home/vagrant/mycode/group/group0/src/tests/lib.c:111)
0x080480dd: test_main (/home/vagrant/mycode/group/group0/src/tests/userprog/multithreading/create-simple.c:19)
0x08048978: main (/home/vagrant/mycode/group/group0/src/tests/main.c:14)
0x080489a3: _start (/home/vagrant/mycode/group/group0/src/lib/user/entry.c:7)
Translations of user virtual addresses above are based on a guess at
the binary to use.  If this guess is incorrect, then those
translations will be misleading.
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/create-many -a create-many -- -q   -f run create-many < /dev/null 2> tests/userprog/multithreading/create-many.errors > tests/userprog/multithreading/create-many.output
perl -I../.. ../../tests/userprog/multithreading/create-many.ck tests/userprog/multithreading/create-many tests/userprog/multithreading/create-many.result
pass tests/userprog/multithreading/create-many
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/create-reuse -a create-reuse -- -q   -f run create-reuse < /dev/null 2> tests/userprog/multithreading/create-reuse.errors > tests/userprog/multithreading/create-reuse.output
perl -I../.. ../../tests/userprog/multithreading/create-reuse.ck tests/userprog/multithreading/create-reuse tests/userprog/multithreading/create-reuse.result
pass tests/userprog/multithreading/create-reuse
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exec-thread-1 -a exec-thread-1 -- -q   -f run 'exec-thread-1 thread' < /dev/null 2> tests/userprog/multithreading/exec-thread-1.errors > tests/userprog/multithreading/exec-thread-1.output
perl -I../.. ../../tests/userprog/multithreading/exec-thread-1.ck tests/userprog/multithreading/exec-thread-1 tests/userprog/multithreading/exec-thread-1.result
pass tests/userprog/multithreading/exec-thread-1
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exec-thread-n -a exec-thread-n -- -q   -f run 'exec-thread-n root' < /dev/null 2> tests/userprog/multithreading/exec-thread-n.errors > tests/userprog/multithreading/exec-thread-n.output
perl -I../.. ../../tests/userprog/multithreading/exec-thread-n.ck tests/userprog/multithreading/exec-thread-n tests/userprog/multithreading/exec-thread-n.result
pass tests/userprog/multithreading/exec-thread-n
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exit-simple -a exit-simple -- -q   -f run exit-simple < /dev/null 2> tests/userprog/multithreading/exit-simple.errors > tests/userprog/multithreading/exit-simple.output
perl -I../.. ../../tests/userprog/multithreading/exit-simple.ck tests/userprog/multithreading/exit-simple tests/userprog/multithreading/exit-simple.result
FAIL tests/userprog/multithreading/exit-simple
Kernel panic in run: PANIC at ../../userprog/exception.c:97 in kill(): Kernel bug - unexpected interrupt in kernel
Call stack: 0xc002b0dd 0xc002fe95 0xc002ffb3 0xc00225ad 0xc0022803 0xc002b388 0xc002b924 0xc002b84a 0xc002eb28 0xc002eea7 0xc0030b58 0xc003047c 0xc00225ad 0xc0022803 0x804ae1f 0x804af98 0x80483d4 0x80480df 0x804897a 0x80489a5
Translation of call stack:
In kernel.o:
0xc002b0dd: debug_panic (/home/vagrant/mycode/group/group0/src/lib/kernel/debug.c:35)
0xc002fe95: kill (/home/vagrant/mycode/group/group0/src/userprog/exception.c:102)
0xc002ffb3: page_fault (/home/vagrant/mycode/group/group0/src/userprog/exception.c:152)
0xc00225ad: intr_handler (/home/vagrant/mycode/group/group0/src/threads/interrupt.c:339)
0xc0022803: intr_entry (threads/intr-stubs.S:38)
0xc002b388: list_begin (/home/vagrant/mycode/group/group0/src/lib/kernel/list.c:65)
0xc002b924: list_empty (/home/vagrant/mycode/group/group0/src/lib/kernel/list.c:264)
0xc002b84a: list_front (/home/vagrant/mycode/group/group0/src/lib/kernel/list.c:241)
0xc002eb28: is_main_thread (/home/vagrant/mycode/group/group0/src/userprog/process.c:838)
0xc002eea7: pthread_join (/home/vagrant/mycode/group/group0/src/userprog/process.c:995)
0xc0030b58: syscall_pt_join (/home/vagrant/mycode/group/group0/src/userprog/syscall.c:510)
0xc003047c: syscall_handler (/home/vagrant/mycode/group/group0/src/userprog/syscall.c:237)
0xc00225ad: intr_handler (/home/vagrant/mycode/group/group0/src/threads/interrupt.c:339)
0xc0022803: intr_entry (threads/intr-stubs.S:38)
In tests/userprog/multithreading/exit-simple:
0x0804ae1f: sys_pthread_join (/home/vagrant/mycode/group/group0/src/lib/user/syscall.c:130)
0x0804af98: pthread_join (/home/vagrant/mycode/group/group0/src/lib/user/pthread.c:21)
0x080483d4: pthread_check_join (/home/vagrant/mycode/group/group0/src/tests/lib.c:111)
0x080480df: test_main (/home/vagrant/mycode/group/group0/src/tests/userprog/multithreading/exit-simple.c:21)
0x0804897a: main (/home/vagrant/mycode/group/group0/src/tests/main.c:14)
0x080489a5: _start (/home/vagrant/mycode/group/group0/src/lib/user/entry.c:7)
Translations of user virtual addresses above are based on a guess at
the binary to use.  If this guess is incorrect, then those
translations will be misleading.
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exit-clean-1 -a exit-clean-1 -p ../../tests/userprog/sample.txt -a sample.txt -- -q   -f run 'exit-clean-1 root exit 1' < /dev/null 2> tests/userprog/multithreading/exit-clean-1.errors > tests/userprog/multithreading/exit-clean-1.output
perl -I../.. ../../tests/userprog/multithreading/exit-clean-1.ck tests/userprog/multithreading/exit-clean-1 tests/userprog/multithreading/exit-clean-1.result
pass tests/userprog/multithreading/exit-clean-1
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exit-clean-2 -a exit-clean-2 -p ../../tests/userprog/sample.txt -a sample.txt -- -q   -f run 'exit-clean-2 root exception 2' < /dev/null 2> tests/userprog/multithreading/exit-clean-2.errors > tests/userprog/multithreading/exit-clean-2.output
perl -I../.. ../../tests/userprog/multithreading/exit-clean-2.ck tests/userprog/multithreading/exit-clean-2 tests/userprog/multithreading/exit-clean-2.result
pass tests/userprog/multithreading/exit-clean-2
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/join-exit-1 -a join-exit-1 -- -q   -f run join-exit-1 < /dev/null 2> tests/userprog/multithreading/join-exit-1.errors > tests/userprog/multithreading/join-exit-1.output
perl -I../.. ../../tests/userprog/multithreading/join-exit-1.ck tests/userprog/multithreading/join-exit-1 tests/userprog/multithreading/join-exit-1.result
pass tests/userprog/multithreading/join-exit-1
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/join-exit-2 -a join-exit-2 -- -q   -f run join-exit-2 < /dev/null 2> tests/userprog/multithreading/join-exit-2.errors > tests/userprog/multithreading/join-exit-2.output
perl -I../.. ../../tests/userprog/multithreading/join-exit-2.ck tests/userprog/multithreading/join-exit-2 tests/userprog/multithreading/join-exit-2.result
pass tests/userprog/multithreading/join-exit-2
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/create-simple -a create-simple -- -q   -f run create-simple < /dev/null 2> tests/userprog/multithreading/create-simple.errors > tests/userprog/multithreading/create-simple.output
perl -I../.. ../../tests/userprog/multithreading/create-simple.ck tests/userprog/multithreading/create-simple tests/userprog/multithreading/create-simple.result
pass tests/userprog/multithreading/create-simple
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/create-many -a create-many -- -q   -f run create-many < /dev/null 2> tests/userprog/multithreading/create-many.errors > tests/userprog/multithreading/create-many.output
perl -I../.. ../../tests/userprog/multithreading/create-many.ck tests/userprog/multithreading/create-many tests/userprog/multithreading/create-many.result
pass tests/userprog/multithreading/create-many
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/create-reuse -a create-reuse -- -q   -f run create-reuse < /dev/null 2> tests/userprog/multithreading/create-reuse.errors > tests/userprog/multithreading/create-reuse.output
perl -I../.. ../../tests/userprog/multithreading/create-reuse.ck tests/userprog/multithreading/create-reuse tests/userprog/multithreading/create-reuse.result
pass tests/userprog/multithreading/create-reuse
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exec-thread-1 -a exec-thread-1 -- -q   -f run 'exec-thread-1 thread' < /dev/null 2> tests/userprog/multithreading/exec-thread-1.errors > tests/userprog/multithreading/exec-thread-1.output
perl -I../.. ../../tests/userprog/multithreading/exec-thread-1.ck tests/userprog/multithreading/exec-thread-1 tests/userprog/multithreading/exec-thread-1.result
pass tests/userprog/multithreading/exec-thread-1
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exec-thread-n -a exec-thread-n -- -q   -f run 'exec-thread-n root' < /dev/null 2> tests/userprog/multithreading/exec-thread-n.errors > tests/userprog/multithreading/exec-thread-n.output
perl -I../.. ../../tests/userprog/multithreading/exec-thread-n.ck tests/userprog/multithreading/exec-thread-n tests/userprog/multithreading/exec-thread-n.result
pass tests/userprog/multithreading/exec-thread-n
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exit-simple -a exit-simple -- -q   -f run exit-simple < /dev/null 2> tests/userprog/multithreading/exit-simple.errors > tests/userprog/multithreading/exit-simple.output
perl -I../.. ../../tests/userprog/multithreading/exit-simple.ck tests/userprog/multithreading/exit-simple tests/userprog/multithreading/exit-simple.result
pass tests/userprog/multithreading/exit-simple
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exit-clean-1 -a exit-clean-1 -p ../../tests/userprog/sample.txt -a sample.txt -- -q   -f run 'exit-clean-1 root exit 1' < /dev/null 2> tests/userprog/multithreading/exit-clean-1.errors > tests/userprog/multithreading/exit-clean-1.output
perl -I../.. ../../tests/userprog/multithreading/exit-clean-1.ck tests/userprog/multithreading/exit-clean-1 tests/userprog/multithreading/exit-clean-1.result
pass tests/userprog/multithreading/exit-clean-1
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exit-clean-2 -a exit-clean-2 -p ../../tests/userprog/sample.txt -a sample.txt -- -q   -f run 'exit-clean-2 root exception 2' < /dev/null 2> tests/userprog/multithreading/exit-clean-2.errors > tests/userprog/multithreading/exit-clean-2.output
perl -I../.. ../../tests/userprog/multithreading/exit-clean-2.ck tests/userprog/multithreading/exit-clean-2 tests/userprog/multithreading/exit-clean-2.result
pass tests/userprog/multithreading/exit-clean-2
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/join-exit-1 -a join-exit-1 -- -q   -f run join-exit-1 < /dev/null 2> tests/userprog/multithreading/join-exit-1.errors > tests/userprog/multithreading/join-exit-1.output
perl -I../.. ../../tests/userprog/multithreading/join-exit-1.ck tests/userprog/multithreading/join-exit-1 tests/userprog/multithreading/join-exit-1.result
pass tests/userprog/multithreading/join-exit-1
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/join-exit-2 -a join-exit-2 -- -q   -f run join-exit-2 < /dev/null 2> tests/userprog/multithreading/join-exit-2.errors > tests/userprog/multithreading/join-exit-2.output
perl -I../.. ../../tests/userprog/multithreading/join-exit-2.ck tests/userprog/multithreading/join-exit-2 tests/userprog/multithreading/join-exit-2.result
pass tests/userprog/multithreading/join-exit-2
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/create-simple -a create-simple -- -q   -f run create-simple < /dev/null 2> tests/userprog/multithreading/create-simple.errors > tests/userprog/multithreading/create-simple.output
perl -I../.. ../../tests/userprog/multithreading/create-simple.ck tests/userprog/multithreading/create-simple tests/userprog/multithreading/create-simple.result
pass tests/userprog/multithreading/create-simple
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/create-many -a create-many -- -q   -f run create-many < /dev/null 2> tests/userprog/multithreading/create-many.errors > tests/userprog/multithreading/create-many.output
perl -I../.. ../../tests/userprog/multithreading/create-many.ck tests/userprog/multithreading/create-many tests/userprog/multithreading/create-many.result
FAIL tests/userprog/multithreading/create-many
Test output failed to match any acceptable form.

Acceptable output:
  (create-many) begin
  (create-many) Thread prints 162
  (create-many) Thread prints 162
  (create-many) Thread prints 162
  (create-many) Thread prints 162
  (create-many) Thread prints 162
  (create-many) Thread prints 162
  (create-many) Thread prints 162
  (create-many) Thread prints 162
  (create-many) Thread prints 162
  (create-many) Thread prints 162
  (create-many) Main finished
  (create-many) end
  create-many: exit(0)
Differences in `diff -u' format:
  (create-many) begin
  (create-many) Thread prints 162
  (create-many) Thread prints 162
  (create-many) Thread prints 162
  (create-many) Thread prints 162
  (create-many) Thread prints 162
  (create-many) Thread prints 162
  (create-many) Thread prints 162
  (create-many) Thread prints 162
  (create-many) Thread prints 162
  (create-many) Thread prints 162
- (create-many) Main finished
- (create-many) end
- create-many: exit(0)
+ create-many: exit(1)

(User fault messages are excluded for matching purposes.)
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/create-reuse -a create-reuse -- -q   -f run create-reuse < /dev/null 2> tests/userprog/multithreading/create-reuse.errors > tests/userprog/multithreading/create-reuse.output
perl -I../.. ../../tests/userprog/multithreading/create-reuse.ck tests/userprog/multithreading/create-reuse tests/userprog/multithreading/create-reuse.result
pass tests/userprog/multithreading/create-reuse
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exec-thread-1 -a exec-thread-1 -- -q   -f run 'exec-thread-1 thread' < /dev/null 2> tests/userprog/multithreading/exec-thread-1.errors > tests/userprog/multithreading/exec-thread-1.output
perl -I../.. ../../tests/userprog/multithreading/exec-thread-1.ck tests/userprog/multithreading/exec-thread-1 tests/userprog/multithreading/exec-thread-1.result
pass tests/userprog/multithreading/exec-thread-1
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exec-thread-n -a exec-thread-n -- -q   -f run 'exec-thread-n root' < /dev/null 2> tests/userprog/multithreading/exec-thread-n.errors > tests/userprog/multithreading/exec-thread-n.output
perl -I../.. ../../tests/userprog/multithreading/exec-thread-n.ck tests/userprog/multithreading/exec-thread-n tests/userprog/multithreading/exec-thread-n.result
FAIL tests/userprog/multithreading/exec-thread-n
Test output failed to match any acceptable form.

Acceptable output:
  (exec-thread-n) Root process starting
  (exec-thread-n) Spawning child threads
  (exec-thread-n) Child thread ran
  (exec-thread-n) Child thread ran
  (exec-thread-n) Child thread ran
  (exec-thread-n) Child thread ran
  (exec-thread-n) Child thread ran
  (exec-thread-n) Child thread ran
  (exec-thread-n) Child thread ran
  (exec-thread-n) Child thread ran
  (exec-thread-n) Child thread ran
  (exec-thread-n) Child thread ran
  (exec-thread-n) Finished waiting on children
  exec-thread-n: exit(0)
  (exec-thread-n) Root process finishing
  exec-thread-n: exit(0)
Differences in `diff -u' format:
  (exec-thread-n) Root process starting
  (exec-thread-n) Spawning child threads
  (exec-thread-n) Child thread ran
  (exec-thread-n) Child thread ran
  (exec-thread-n) Child thread ran
  (exec-thread-n) Child thread ran
  (exec-thread-n) Child thread ran
  (exec-thread-n) Child thread ran
  (exec-thread-n) Child thread ran
  (exec-thread-n) Child thread ran
  (exec-thread-n) Child thread ran
  (exec-thread-n) Child thread ran
- (exec-thread-n) Finished waiting on children
- exec-thread-n: exit(0)
+ exec-thread-n: exit(1)
  (exec-thread-n) Root process finishing
  exec-thread-n: exit(0)
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exit-simple -a exit-simple -- -q   -f run exit-simple < /dev/null 2> tests/userprog/multithreading/exit-simple.errors > tests/userprog/multithreading/exit-simple.output
perl -I../.. ../../tests/userprog/multithreading/exit-simple.ck tests/userprog/multithreading/exit-simple tests/userprog/multithreading/exit-simple.result
pass tests/userprog/multithreading/exit-simple
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exit-clean-1 -a exit-clean-1 -p ../../tests/userprog/sample.txt -a sample.txt -- -q   -f run 'exit-clean-1 root exit 1' < /dev/null 2> tests/userprog/multithreading/exit-clean-1.errors > tests/userprog/multithreading/exit-clean-1.output
perl -I../.. ../../tests/userprog/multithreading/exit-clean-1.ck tests/userprog/multithreading/exit-clean-1 tests/userprog/multithreading/exit-clean-1.result
pass tests/userprog/multithreading/exit-clean-1
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exit-clean-2 -a exit-clean-2 -p ../../tests/userprog/sample.txt -a sample.txt -- -q   -f run 'exit-clean-2 root exception 2' < /dev/null 2> tests/userprog/multithreading/exit-clean-2.errors > tests/userprog/multithreading/exit-clean-2.output
perl -I../.. ../../tests/userprog/multithreading/exit-clean-2.ck tests/userprog/multithreading/exit-clean-2 tests/userprog/multithreading/exit-clean-2.result
pass tests/userprog/multithreading/exit-clean-2
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/join-exit-1 -a join-exit-1 -- -q   -f run join-exit-1 < /dev/null 2> tests/userprog/multithreading/join-exit-1.errors > tests/userprog/multithreading/join-exit-1.output
perl -I../.. ../../tests/userprog/multithreading/join-exit-1.ck tests/userprog/multithreading/join-exit-1 tests/userprog/multithreading/join-exit-1.result
pass tests/userprog/multithreading/join-exit-1
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/join-exit-2 -a join-exit-2 -- -q   -f run join-exit-2 < /dev/null 2> tests/userprog/multithreading/join-exit-2.errors > tests/userprog/multithreading/join-exit-2.output
perl -I../.. ../../tests/userprog/multithreading/join-exit-2.ck tests/userprog/multithreading/join-exit-2 tests/userprog/multithreading/join-exit-2.result
pass tests/userprog/multithreading/join-exit-2
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/create-simple -a create-simple -- -q   -f run create-simple < /dev/null 2> tests/userprog/multithreading/create-simple.errors > tests/userprog/multithreading/create-simple.output
perl -I../.. ../../tests/userprog/multithreading/create-simple.ck tests/userprog/multithreading/create-simple tests/userprog/multithreading/create-simple.result
pass tests/userprog/multithreading/create-simple
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/create-many -a create-many -- -q   -f run create-many < /dev/null 2> tests/userprog/multithreading/create-many.errors > tests/userprog/multithreading/create-many.output
perl -I../.. ../../tests/userprog/multithreading/create-many.ck tests/userprog/multithreading/create-many tests/userprog/multithreading/create-many.result
pass tests/userprog/multithreading/create-many
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/create-reuse -a create-reuse -- -q   -f run create-reuse < /dev/null 2> tests/userprog/multithreading/create-reuse.errors > tests/userprog/multithreading/create-reuse.output
perl -I../.. ../../tests/userprog/multithreading/create-reuse.ck tests/userprog/multithreading/create-reuse tests/userprog/multithreading/create-reuse.result
pass tests/userprog/multithreading/create-reuse
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exec-thread-1 -a exec-thread-1 -- -q   -f run 'exec-thread-1 thread' < /dev/null 2> tests/userprog/multithreading/exec-thread-1.errors > tests/userprog/multithreading/exec-thread-1.output
perl -I../.. ../../tests/userprog/multithreading/exec-thread-1.ck tests/userprog/multithreading/exec-thread-1 tests/userprog/multithreading/exec-thread-1.result
pass tests/userprog/multithreading/exec-thread-1
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exec-thread-n -a exec-thread-n -- -q   -f run 'exec-thread-n root' < /dev/null 2> tests/userprog/multithreading/exec-thread-n.errors > tests/userprog/multithreading/exec-thread-n.output
perl -I../.. ../../tests/userprog/multithreading/exec-thread-n.ck tests/userprog/multithreading/exec-thread-n tests/userprog/multithreading/exec-thread-n.result
pass tests/userprog/multithreading/exec-thread-n
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exit-simple -a exit-simple -- -q   -f run exit-simple < /dev/null 2> tests/userprog/multithreading/exit-simple.errors > tests/userprog/multithreading/exit-simple.output
perl -I../.. ../../tests/userprog/multithreading/exit-simple.ck tests/userprog/multithreading/exit-simple tests/userprog/multithreading/exit-simple.result
pass tests/userprog/multithreading/exit-simple
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exit-clean-1 -a exit-clean-1 -p ../../tests/userprog/sample.txt -a sample.txt -- -q   -f run 'exit-clean-1 root exit 1' < /dev/null 2> tests/userprog/multithreading/exit-clean-1.errors > tests/userprog/multithreading/exit-clean-1.output
perl -I../.. ../../tests/userprog/multithreading/exit-clean-1.ck tests/userprog/multithreading/exit-clean-1 tests/userprog/multithreading/exit-clean-1.result
pass tests/userprog/multithreading/exit-clean-1
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exit-clean-2 -a exit-clean-2 -p ../../tests/userprog/sample.txt -a sample.txt -- -q   -f run 'exit-clean-2 root exception 2' < /dev/null 2> tests/userprog/multithreading/exit-clean-2.errors > tests/userprog/multithreading/exit-clean-2.output
perl -I../.. ../../tests/userprog/multithreading/exit-clean-2.ck tests/userprog/multithreading/exit-clean-2 tests/userprog/multithreading/exit-clean-2.result
pass tests/userprog/multithreading/exit-clean-2
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/join-exit-1 -a join-exit-1 -- -q   -f run join-exit-1 < /dev/null 2> tests/userprog/multithreading/join-exit-1.errors > tests/userprog/multithreading/join-exit-1.output
perl -I../.. ../../tests/userprog/multithreading/join-exit-1.ck tests/userprog/multithreading/join-exit-1 tests/userprog/multithreading/join-exit-1.result
pass tests/userprog/multithreading/join-exit-1
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/join-exit-2 -a join-exit-2 -- -q   -f run join-exit-2 < /dev/null 2> tests/userprog/multithreading/join-exit-2.errors > tests/userprog/multithreading/join-exit-2.output
perl -I../.. ../../tests/userprog/multithreading/join-exit-2.ck tests/userprog/multithreading/join-exit-2 tests/userprog/multithreading/join-exit-2.result
pass tests/userprog/multithreading/join-exit-2
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/create-simple -a create-simple -- -q   -f run create-simple < /dev/null 2> tests/userprog/multithreading/create-simple.errors > tests/userprog/multithreading/create-simple.output
perl -I../.. ../../tests/userprog/multithreading/create-simple.ck tests/userprog/multithreading/create-simple tests/userprog/multithreading/create-simple.result
pass tests/userprog/multithreading/create-simple
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/create-many -a create-many -- -q   -f run create-many < /dev/null 2> tests/userprog/multithreading/create-many.errors > tests/userprog/multithreading/create-many.output
perl -I../.. ../../tests/userprog/multithreading/create-many.ck tests/userprog/multithreading/create-many tests/userprog/multithreading/create-many.result
pass tests/userprog/multithreading/create-many
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/create-reuse -a create-reuse -- -q   -f run create-reuse < /dev/null 2> tests/userprog/multithreading/create-reuse.errors > tests/userprog/multithreading/create-reuse.output
perl -I../.. ../../tests/userprog/multithreading/create-reuse.ck tests/userprog/multithreading/create-reuse tests/userprog/multithreading/create-reuse.result
pass tests/userprog/multithreading/create-reuse
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exec-thread-1 -a exec-thread-1 -- -q   -f run 'exec-thread-1 thread' < /dev/null 2> tests/userprog/multithreading/exec-thread-1.errors > tests/userprog/multithreading/exec-thread-1.output
perl -I../.. ../../tests/userprog/multithreading/exec-thread-1.ck tests/userprog/multithreading/exec-thread-1 tests/userprog/multithreading/exec-thread-1.result
pass tests/userprog/multithreading/exec-thread-1
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exec-thread-n -a exec-thread-n -- -q   -f run 'exec-thread-n root' < /dev/null 2> tests/userprog/multithreading/exec-thread-n.errors > tests/userprog/multithreading/exec-thread-n.output
perl -I../.. ../../tests/userprog/multithreading/exec-thread-n.ck tests/userprog/multithreading/exec-thread-n tests/userprog/multithreading/exec-thread-n.result
pass tests/userprog/multithreading/exec-thread-n
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exit-simple -a exit-simple -- -q   -f run exit-simple < /dev/null 2> tests/userprog/multithreading/exit-simple.errors > tests/userprog/multithreading/exit-simple.output
perl -I../.. ../../tests/userprog/multithreading/exit-simple.ck tests/userprog/multithreading/exit-simple tests/userprog/multithreading/exit-simple.result
pass tests/userprog/multithreading/exit-simple
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exit-clean-1 -a exit-clean-1 -p ../../tests/userprog/sample.txt -a sample.txt -- -q   -f run 'exit-clean-1 root exit 1' < /dev/null 2> tests/userprog/multithreading/exit-clean-1.errors > tests/userprog/multithreading/exit-clean-1.output
perl -I../.. ../../tests/userprog/multithreading/exit-clean-1.ck tests/userprog/multithreading/exit-clean-1 tests/userprog/multithreading/exit-clean-1.result
pass tests/userprog/multithreading/exit-clean-1
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exit-clean-2 -a exit-clean-2 -p ../../tests/userprog/sample.txt -a sample.txt -- -q   -f run 'exit-clean-2 root exception 2' < /dev/null 2> tests/userprog/multithreading/exit-clean-2.errors > tests/userprog/multithreading/exit-clean-2.output
perl -I../.. ../../tests/userprog/multithreading/exit-clean-2.ck tests/userprog/multithreading/exit-clean-2 tests/userprog/multithreading/exit-clean-2.result
pass tests/userprog/multithreading/exit-clean-2
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/join-exit-1 -a join-exit-1 -- -q   -f run join-exit-1 < /dev/null 2> tests/userprog/multithreading/join-exit-1.errors > tests/userprog/multithreading/join-exit-1.output
perl -I../.. ../../tests/userprog/multithreading/join-exit-1.ck tests/userprog/multithreading/join-exit-1 tests/userprog/multithreading/join-exit-1.result
pass tests/userprog/multithreading/join-exit-1
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/join-exit-2 -a join-exit-2 -- -q   -f run join-exit-2 < /dev/null 2> tests/userprog/multithreading/join-exit-2.errors > tests/userprog/multithreading/join-exit-2.output
perl -I../.. ../../tests/userprog/multithreading/join-exit-2.ck tests/userprog/multithreading/join-exit-2 tests/userprog/multithreading/join-exit-2.result
pass tests/userprog/multithreading/join-exit-2
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/create-simple -a create-simple -- -q   -f run create-simple < /dev/null 2> tests/userprog/multithreading/create-simple.errors > tests/userprog/multithreading/create-simple.output
perl -I../.. ../../tests/userprog/multithreading/create-simple.ck tests/userprog/multithreading/create-simple tests/userprog/multithreading/create-simple.result
pass tests/userprog/multithreading/create-simple
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/create-many -a create-many -- -q   -f run create-many < /dev/null 2> tests/userprog/multithreading/create-many.errors > tests/userprog/multithreading/create-many.output
perl -I../.. ../../tests/userprog/multithreading/create-many.ck tests/userprog/multithreading/create-many tests/userprog/multithreading/create-many.result
FAIL tests/userprog/multithreading/create-many
Test output failed to match any acceptable form.

Acceptable output:
  (create-many) begin
  (create-many) Thread prints 162
  (create-many) Thread prints 162
  (create-many) Thread prints 162
  (create-many) Thread prints 162
  (create-many) Thread prints 162
  (create-many) Thread prints 162
  (create-many) Thread prints 162
  (create-many) Thread prints 162
  (create-many) Thread prints 162
  (create-many) Thread prints 162
  (create-many) Main finished
  (create-many) end
  create-many: exit(0)
Differences in `diff -u' format:
  (create-many) begin
  (create-many) Thread prints 162
- (create-many) Thread prints 162
- (create-many) Thread prints 162
- (create-many) Thread prints 162
- (create-many) Thread prints 162
- (create-many) Thread prints 162
- (create-many) Thread prints 162
- (create-many) Thread prints 162
- (create-many) Thread prints 162
- (create-many) Thread prints 162
- (create-many) Main finished
- (create-many) end
- create-many: exit(0)
+ create-many: exit(1)

(User fault messages are excluded for matching purposes.)
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/create-reuse -a create-reuse -- -q   -f run create-reuse < /dev/null 2> tests/userprog/multithreading/create-reuse.errors > tests/userprog/multithreading/create-reuse.output
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exec-thread-1 -a exec-thread-1 -- -q   -f run 'exec-thread-1 thread' < /dev/null 2> tests/userprog/multithreading/exec-thread-1.errors > tests/userprog/multithreading/exec-thread-1.output
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exec-thread-n -a exec-thread-n -- -q   -f run 'exec-thread-n root' < /dev/null 2> tests/userprog/multithreading/exec-thread-n.errors > tests/userprog/multithreading/exec-thread-n.output
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/create-simple -a create-simple -- -q   -f run create-simple < /dev/null 2> tests/userprog/multithreading/create-simple.errors > tests/userprog/multithreading/create-simple.output
perl -I../.. ../../tests/userprog/multithreading/create-simple.ck tests/userprog/multithreading/create-simple tests/userprog/multithreading/create-simple.result
pass tests/userprog/multithreading/create-simple
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/create-many -a create-many -- -q   -f run create-many < /dev/null 2> tests/userprog/multithreading/create-many.errors > tests/userprog/multithreading/create-many.output
perl -I../.. ../../tests/userprog/multithreading/create-many.ck tests/userprog/multithreading/create-many tests/userprog/multithreading/create-many.result
pass tests/userprog/multithreading/create-many
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/create-reuse -a create-reuse -- -q   -f run create-reuse < /dev/null 2> tests/userprog/multithreading/create-reuse.errors > tests/userprog/multithreading/create-reuse.output
perl -I../.. ../../tests/userprog/multithreading/create-reuse.ck tests/userprog/multithreading/create-reuse tests/userprog/multithreading/create-reuse.result
pass tests/userprog/multithreading/create-reuse
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exec-thread-1 -a exec-thread-1 -- -q   -f run 'exec-thread-1 thread' < /dev/null 2> tests/userprog/multithreading/exec-thread-1.errors > tests/userprog/multithreading/exec-thread-1.output
perl -I../.. ../../tests/userprog/multithreading/exec-thread-1.ck tests/userprog/multithreading/exec-thread-1 tests/userprog/multithreading/exec-thread-1.result
pass tests/userprog/multithreading/exec-thread-1
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exec-thread-n -a exec-thread-n -- -q   -f run 'exec-thread-n root' < /dev/null 2> tests/userprog/multithreading/exec-thread-n.errors > tests/userprog/multithreading/exec-thread-n.output
perl -I../.. ../../tests/userprog/multithreading/exec-thread-n.ck tests/userprog/multithreading/exec-thread-n tests/userprog/multithreading/exec-thread-n.result
pass tests/userprog/multithreading/exec-thread-n
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exit-simple -a exit-simple -- -q   -f run exit-simple < /dev/null 2> tests/userprog/multithreading/exit-simple.errors > tests/userprog/multithreading/exit-simple.output
perl -I../.. ../../tests/userprog/multithreading/exit-simple.ck tests/userprog/multithreading/exit-simple tests/userprog/multithreading/exit-simple.result
pass tests/userprog/multithreading/exit-simple
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exit-clean-1 -a exit-clean-1 -p ../../tests/userprog/sample.txt -a sample.txt -- -q   -f run 'exit-clean-1 root exit 1' < /dev/null 2> tests/userprog/multithreading/exit-clean-1.errors > tests/userprog/multithreading/exit-clean-1.output
perl -I../.. ../../tests/userprog/multithreading/exit-clean-1.ck tests/userprog/multithreading/exit-clean-1 tests/userprog/multithreading/exit-clean-1.result
pass tests/userprog/multithreading/exit-clean-1
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exit-clean-2 -a exit-clean-2 -p ../../tests/userprog/sample.txt -a sample.txt -- -q   -f run 'exit-clean-2 root exception 2' < /dev/null 2> tests/userprog/multithreading/exit-clean-2.errors > tests/userprog/multithreading/exit-clean-2.output
perl -I../.. ../../tests/userprog/multithreading/exit-clean-2.ck tests/userprog/multithreading/exit-clean-2 tests/userprog/multithreading/exit-clean-2.result
pass tests/userprog/multithreading/exit-clean-2
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/join-exit-1 -a join-exit-1 -- -q   -f run join-exit-1 < /dev/null 2> tests/userprog/multithreading/join-exit-1.errors > tests/userprog/multithreading/join-exit-1.output
perl -I../.. ../../tests/userprog/multithreading/join-exit-1.ck tests/userprog/multithreading/join-exit-1 tests/userprog/multithreading/join-exit-1.result
pass tests/userprog/multithreading/join-exit-1
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/join-exit-2 -a join-exit-2 -- -q   -f run join-exit-2 < /dev/null 2> tests/userprog/multithreading/join-exit-2.errors > tests/userprog/multithreading/join-exit-2.output
perl -I../.. ../../tests/userprog/multithreading/join-exit-2.ck tests/userprog/multithreading/join-exit-2 tests/userprog/multithreading/join-exit-2.result
pass tests/userprog/multithreading/join-exit-2
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/join-recur -a join-recur -- -q   -f run join-recur < /dev/null 2> tests/userprog/multithreading/join-recur.errors > tests/userprog/multithreading/join-recur.output
perl -I../.. ../../tests/userprog/multithreading/join-recur.ck tests/userprog/multithreading/join-recur tests/userprog/multithreading/join-recur.result
pass tests/userprog/multithreading/join-recur
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/create-simple -a create-simple -- -q   -f run create-simple < /dev/null 2> tests/userprog/multithreading/create-simple.errors > tests/userprog/multithreading/create-simple.output
perl -I../.. ../../tests/userprog/multithreading/create-simple.ck tests/userprog/multithreading/create-simple tests/userprog/multithreading/create-simple.result
pass tests/userprog/multithreading/create-simple
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/create-many -a create-many -- -q   -f run create-many < /dev/null 2> tests/userprog/multithreading/create-many.errors > tests/userprog/multithreading/create-many.output
perl -I../.. ../../tests/userprog/multithreading/create-many.ck tests/userprog/multithreading/create-many tests/userprog/multithreading/create-many.result
pass tests/userprog/multithreading/create-many
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/create-reuse -a create-reuse -- -q   -f run create-reuse < /dev/null 2> tests/userprog/multithreading/create-reuse.errors > tests/userprog/multithreading/create-reuse.output
perl -I../.. ../../tests/userprog/multithreading/create-reuse.ck tests/userprog/multithreading/create-reuse tests/userprog/multithreading/create-reuse.result
pass tests/userprog/multithreading/create-reuse
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exec-thread-1 -a exec-thread-1 -- -q   -f run 'exec-thread-1 thread' < /dev/null 2> tests/userprog/multithreading/exec-thread-1.errors > tests/userprog/multithreading/exec-thread-1.output
perl -I../.. ../../tests/userprog/multithreading/exec-thread-1.ck tests/userprog/multithreading/exec-thread-1 tests/userprog/multithreading/exec-thread-1.result
pass tests/userprog/multithreading/exec-thread-1
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exec-thread-n -a exec-thread-n -- -q   -f run 'exec-thread-n root' < /dev/null 2> tests/userprog/multithreading/exec-thread-n.errors > tests/userprog/multithreading/exec-thread-n.output
perl -I../.. ../../tests/userprog/multithreading/exec-thread-n.ck tests/userprog/multithreading/exec-thread-n tests/userprog/multithreading/exec-thread-n.result
pass tests/userprog/multithreading/exec-thread-n
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exit-simple -a exit-simple -- -q   -f run exit-simple < /dev/null 2> tests/userprog/multithreading/exit-simple.errors > tests/userprog/multithreading/exit-simple.output
perl -I../.. ../../tests/userprog/multithreading/exit-simple.ck tests/userprog/multithreading/exit-simple tests/userprog/multithreading/exit-simple.result
pass tests/userprog/multithreading/exit-simple
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exit-clean-1 -a exit-clean-1 -p ../../tests/userprog/sample.txt -a sample.txt -- -q   -f run 'exit-clean-1 root exit 1' < /dev/null 2> tests/userprog/multithreading/exit-clean-1.errors > tests/userprog/multithreading/exit-clean-1.output
perl -I../.. ../../tests/userprog/multithreading/exit-clean-1.ck tests/userprog/multithreading/exit-clean-1 tests/userprog/multithreading/exit-clean-1.result
pass tests/userprog/multithreading/exit-clean-1
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/exit-clean-2 -a exit-clean-2 -p ../../tests/userprog/sample.txt -a sample.txt -- -q   -f run 'exit-clean-2 root exception 2' < /dev/null 2> tests/userprog/multithreading/exit-clean-2.errors > tests/userprog/multithreading/exit-clean-2.output
perl -I../.. ../../tests/userprog/multithreading/exit-clean-2.ck tests/userprog/multithreading/exit-clean-2 tests/userprog/multithreading/exit-clean-2.result
pass tests/userprog/multithreading/exit-clean-2
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/join-exit-1 -a join-exit-1 -- -q   -f run join-exit-1 < /dev/null 2> tests/userprog/multithreading/join-exit-1.errors > tests/userprog/multithreading/join-exit-1.output
perl -I../.. ../../tests/userprog/multithreading/join-exit-1.ck tests/userprog/multithreading/join-exit-1 tests/userprog/multithreading/join-exit-1.result
pass tests/userprog/multithreading/join-exit-1
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/join-exit-2 -a join-exit-2 -- -q   -f run join-exit-2 < /dev/null 2> tests/userprog/multithreading/join-exit-2.errors > tests/userprog/multithreading/join-exit-2.output
perl -I../.. ../../tests/userprog/multithreading/join-exit-2.ck tests/userprog/multithreading/join-exit-2 tests/userprog/multithreading/join-exit-2.result
pass tests/userprog/multithreading/join-exit-2
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/join-recur -a join-recur -- -q   -f run join-recur < /dev/null 2> tests/userprog/multithreading/join-recur.errors > tests/userprog/multithreading/join-recur.output
perl -I../.. ../../tests/userprog/multithreading/join-recur.ck tests/userprog/multithreading/join-recur tests/userprog/multithreading/join-recur.result
pass tests/userprog/multithreading/join-recur
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/lock-acq-fail -a lock-acq-fail -- -q   -f run lock-acq-fail < /dev/null 2> tests/userprog/multithreading/lock-acq-fail.errors > tests/userprog/multithreading/lock-acq-fail.output
perl -I../.. ../../tests/userprog/multithreading/lock-acq-fail.ck tests/userprog/multithreading/lock-acq-fail tests/userprog/multithreading/lock-acq-fail.result
pass tests/userprog/multithreading/lock-acq-fail
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/lock-data -a lock-data -- -q   -f run lock-data < /dev/null 2> tests/userprog/multithreading/lock-data.errors > tests/userprog/multithreading/lock-data.output
perl -I../.. ../../tests/userprog/multithreading/lock-data.ck tests/userprog/multithreading/lock-data tests/userprog/multithreading/lock-data.result
pass tests/userprog/multithreading/lock-data
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/multithreading/lock-dbl-acq -a lock-dbl-acq -- -q   -f run lock-dbl-acq < /dev/null 2> tests/userprog/multithreading/lock-dbl-acq.errors > tests/userprog/multithreading/lock-dbl-acq.output
perl -I../.. ../../tests/userprog/multithreading/lock-dbl-acq.ck tests/userprog/multithreading/lock-dbl-acq tests/userprog/multithreading/lock-dbl-acq.result
pass tests/userprog/multithreading/lock-dbl-acq
