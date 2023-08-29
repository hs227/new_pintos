#!/bin/bash


# checkpoint 1
#pintos-test dir-empty-name     | grep -E "^(pass|FAIL|run)" | tee    formytest.res
#pintos-test dir-mkdir          | grep -E "^(pass|FAIL|run)" | tee    formytest.res
#pintos-test dir-open           | grep -E "^(pass|FAIL|run)" | tee    formytest.res
#pintos-test dir-over-file      | grep -E "^(pass|FAIL|run)" | tee    formytest.res
#pintos-test dir-mk-tree        | grep -E "^(pass|FAIL|run)" | tee    formytest.res
#pintos-test dir-under-file     | grep -E "^(pass|FAIL|run)" | tee    formytest.res
#pintos-test dir-rmdir          | grep -E "^(pass|FAIL|run)" | tee    formytest.res
#pintos-test dir-rm-root        | grep -E "^(pass|FAIL|run)" | tee    formytest.res
#pintos-test dir-rm-tree        | grep -E "^(pass|FAIL|run)" | tee    formytest.res
#pintos-test dir-rm-parent      | grep -E "^(pass|FAIL|run)" | tee    formytest.res
##pintos-test dir-rm-cwd         | grep -E "^(pass|FAIL|run)" | tee    formytest.res

#checkpoint 2
pintos-test grow-create        | grep -E "^(pass|FAIL|run)" | tee    formytest.res
pintos-test grow-dir-lg        | grep -E "^(pass|FAIL|run)" | tee    formytest.res
pintos-test grow-file-size     | grep -E "^(pass|FAIL|run)" | tee    formytest.res
pintos-test grow-root-lg       | grep -E "^(pass|FAIL|run)" | tee    formytest.res
pintos-test grow-root-sm       | grep -E "^(pass|FAIL|run)" | tee    formytest.res
pintos-test grow-seq-lg        | grep -E "^(pass|FAIL|run)" | tee    formytest.res
pintos-test grow-seq-sm        | grep -E "^(pass|FAIL|run)" | tee    formytest.res
pintos-test grow-tell          | grep -E "^(pass|FAIL|run)" | tee    formytest.res
pintos-test grow-two-files     | grep -E "^(pass|FAIL|run)" | tee    formytest.res
#pintos-test grow-sparse        | grep -E "^(pass|FAIL|run)" | tee    formytest.res
pintos-test dir-vine           | grep -E "^(pass|FAIL|run)" | tee    formytest.res










