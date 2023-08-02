#!/bin/bash


# checkpoint1
#pintos-test alarm-negative     | tee    formytest.res
#pintos-test alarm-simultaneous | tee -a formytest.res
#pintos-test alarm-single       | tee -a formytest.res
#pintos-test alarm-zero         | tee -a formytest.res
#pintos-test alarm-multiple     | tee -a formytest.res

# checkpoint2
#pintos-test alarm-priority     | tee -a formytest.res
#pintos-test priority-donate-one| tee -a formytest.res
#pintos-test priority-preempt   | tee -a formytest.res
#pintos-test priority-starve    | tee -a formytest.res
#pintos-test priority-starve-sema| tee -a formytest.res
#pintos-test priority-sema      | tee -a formytest.res
#pintos-test priority-condvar   | tee -a formytest.res
#pintos-test priority-change    | tee -a formytest.res
#pintos-test priority-donate-chain| tee -a formytest.res
#pintos-test priority-donate-lower| tee -a formytest.res
#pintos-test priority-fifo      | tee -a formytest.res
#pintos-test priority-nest      | tee -a formytest.res
#pintos-test priority-donate-sema| tee -a formytest.res
#pintos-test priority-donate-multiple| tee -a formytest.res
#pintos-test priority-donate-multiple2| tee -a formytest.res

# checkpoint3
pintos-test create-simple      | tee -a formytest.res
pintos-test create-many        | tee -a formytest.res
pintos-test create-reuse       | tee -a formytest.res
pintos-test exec-thread-1      | tee -a formytest.res
pintos-test exec-thread-n      | tee -a formytest.res
pintos-test exit-simple        | tee -a formytest.res
pintos-test exit-clean-1       | tee -a formytest.res
pintos-test exit-clean-2       | tee -a formytest.res
pintos-test join-exit-1        | tee -a formytest.res
pintos-test join-exit-2        | tee -a formytest.res
#pintos-test join-fail          | tee -a formytest.res
pintos-test join-recur         | tee -a formytest.res
pintos-test lock-acq-fail      | tee -a formytest.res
pintos-test lock-data          | tee -a formytest.res
pintos-test lock-dbl-acq       | tee -a formytest.res
pintos-test lock-dbl-rel       | tee -a formytest.res
pintos-test lock-ll            | tee -a formytest.res
pintos-test lock-simple        | tee -a formytest.res
pintos-test pcb-syn            | tee -a formytest.res
pintos-test reuse_stack        | tee -a formytest.res
pintos-test sema-init-fail     | tee -a formytest.res
pintos-test sema-simple        | tee -a formytest.res
pintos-test sema-up-fail       | tee -a formytest.res
pintos-test sema-wait-many     | tee -a formytest.res
pintos-test sema-wait          | tee -a formytest.res
pintos-test synch-many         | tee -a formytest.res
pintos-test wait-fail          | tee -a formytest.res












 




