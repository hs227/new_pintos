#!/bin/bash


# checkpoint1
pintos-test args-single    | tee    formytest.res
pintos-test args-none      | tee -a formytest.res
pintos-test args-multiple  | tee -a formytest.res
pintos-test args-many      | tee -a formytest.res
pintos-test args-dbl-space | tee -a formytest.res
pintos-test bad-jump       | tee -a formytest.res
pintos-test bad-jump2      | tee -a formytest.res
pintos-test bad-read       | tee -a formytest.res
pintos-test bad-read2      | tee -a formytest.res
pintos-test bad-write      | tee -a formytest.res
pintos-test bad-write2     | tee -a formytest.res
pintos-test stack-align-0  | tee -a formytest.res
pintos-test stack-align-1  | tee -a formytest.res
pintos-test stack-align-2  | tee -a formytest.res
pintos-test stack-align-3  | tee -a formytest.res
pintos-test stack-align-4  | tee -a formytest.res
pintos-test do-nothing     | tee -a formytest.res
pintos-test iloveos        | tee -a formytest.res
pintos-test practice       | tee -a formytest.res
#pintos-test exit           | tee -a formytest.res
pintos-test halt           | tee -a formytest.res

# checkpoint2
pintos-test exec-arg       | tee -a formytest.res
pintos-test exec-bad-ptr   | tee -a formytest.res
pintos-test exec-bound     | tee -a formytest.res
pintos-test exec-bound-2   | tee -a formytest.res
pintos-test exec-bound-3   | tee -a formytest.res
pintos-test exec-missing   | tee -a formytest.res
pintos-test exec-multiple  | tee -a formytest.res
pintos-test exec-once      | tee -a formytest.res
pintos-test create-bad-ptr | tee -a formytest.res
pintos-test create-bound   | tee -a formytest.res
pintos-test create-empty   | tee -a formytest.res
pintos-test create-exists  | tee -a formytest.res
pintos-test create-long    | tee -a formytest.res
pintos-test create-normal  | tee -a formytest.res
pintos-test create-null    | tee -a formytest.res
pintos-test open-bad-ptr   | tee -a formytest.res
pintos-test open-boundary  | tee -a formytest.res
pintos-test open-empty     | tee -a formytest.res
pintos-test open-missing   | tee -a formytest.res
pintos-test open-normal    | tee -a formytest.res
pintos-test open-null      | tee -a formytest.res
pintos-test open-twice     | tee -a formytest.res
pintos-test read-bad-fd    | tee -a formytest.res
pintos-test read-bad-ptr   | tee -a formytest.res
pintos-test read-boundary  | tee -a formytest.res
pintos-test read-normal    | tee -a formytest.res
pintos-test read-stdout    | tee -a formytest.res
pintos-test read-zero      | tee -a formytest.res
pintos-test write-bad-fd   | tee -a formytest.res
pintos-test write-bad-ptr  | tee -a formytest.res
pintos-test write-boundary | tee -a formytest.res
pintos-test write-normal   | tee -a formytest.res
pintos-test write-stdin    | tee -a formytest.res
pintos-test write-zero     | tee -a formytest.res
pintos-test close-bad-fd   | tee -a formytest.res
pintos-test close-normal   | tee -a formytest.res
pintos-test close-stdin    | tee -a formytest.res
pintos-test close-stdout   | tee -a formytest.res
pintos-test close-twice    | tee -a formytest.res
pintos-test multi-child-fd | tee -a formytest.res
pintos-test multi-recurse  | tee -a formytest.res
pintos-test rox-simple     | tee -a formytest.res
pintos-test rox-child      | tee -a formytest.res
pintos-test rox-multichild | tee -a formytest.res
pintos-test sc-bad-arg     | tee -a formytest.res
pintos-test sc-bad-sp      | tee -a formytest.res
pintos-test sc-boundary-2  | tee -a formytest.res
#pintos-test sc-boundary-3  | tee -a formytest.res
pintos-test sc-boundary    | tee -a formytest.res
pintos-test wait-bad-pid   | tee -a formytest.res
pintos-test wait-killed    | tee -a formytest.res
pintos-test wait-simple    | tee -a formytest.res
pintos-test wait-twice     | tee -a formytest.res

#final check
pintos-test floating-point | tee -a formytest.res
pintos-test fp-asm         | tee -a formytest.res
pintos-test fp-init        | tee -a formytest.res
pintos-test fp-kernel-e    | tee -a formytest.res
pintos-test fp-simul       | tee -a formytest.res
pintos-test fp-syscall     | tee -a formytest.res
pintos-test fp-kasm        | tee -a formytest.res
pintos-test fp-kinit       | tee -a formytest.res
#pintos-test multi-oom      | tee -a formytest.res




