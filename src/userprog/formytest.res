pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/args-single -a args-single -- -q   -f run 'args-single onearg' < /dev/null 2> tests/userprog/args-single.errors > tests/userprog/args-single.output
perl -I../.. ../../tests/userprog/args-single.ck tests/userprog/args-single tests/userprog/args-single.result
pass tests/userprog/args-single
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/args-none -a args-none -- -q   -f run args-none < /dev/null 2> tests/userprog/args-none.errors > tests/userprog/args-none.output
perl -I../.. ../../tests/userprog/args-none.ck tests/userprog/args-none tests/userprog/args-none.result
pass tests/userprog/args-none
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/args-multiple -a args-multiple -- -q   -f run 'args-multiple some arguments for you!' < /dev/null 2> tests/userprog/args-multiple.errors > tests/userprog/args-multiple.output
perl -I../.. ../../tests/userprog/args-multiple.ck tests/userprog/args-multiple tests/userprog/args-multiple.result
pass tests/userprog/args-multiple
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/args-many -a args-many -- -q   -f run 'args-many a b c d e f g h i j k l m n o p q r s t u v' < /dev/null 2> tests/userprog/args-many.errors > tests/userprog/args-many.output
perl -I../.. ../../tests/userprog/args-many.ck tests/userprog/args-many tests/userprog/args-many.result
pass tests/userprog/args-many
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/args-dbl-space -a args-dbl-space -- -q   -f run 'args-dbl-space two  spaces!' < /dev/null 2> tests/userprog/args-dbl-space.errors > tests/userprog/args-dbl-space.output
perl -I../.. ../../tests/userprog/args-dbl-space.ck tests/userprog/args-dbl-space tests/userprog/args-dbl-space.result
pass tests/userprog/args-dbl-space
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/bad-jump -a bad-jump -- -q   -f run bad-jump < /dev/null 2> tests/userprog/bad-jump.errors > tests/userprog/bad-jump.output
perl -I../.. ../../tests/userprog/bad-jump.ck tests/userprog/bad-jump tests/userprog/bad-jump.result
pass tests/userprog/bad-jump
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/bad-jump2 -a bad-jump2 -- -q   -f run bad-jump2 < /dev/null 2> tests/userprog/bad-jump2.errors > tests/userprog/bad-jump2.output
perl -I../.. ../../tests/userprog/bad-jump2.ck tests/userprog/bad-jump2 tests/userprog/bad-jump2.result
pass tests/userprog/bad-jump2
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/bad-read -a bad-read -- -q   -f run bad-read < /dev/null 2> tests/userprog/bad-read.errors > tests/userprog/bad-read.output
perl -I../.. ../../tests/userprog/bad-read.ck tests/userprog/bad-read tests/userprog/bad-read.result
pass tests/userprog/bad-read
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/bad-read2 -a bad-read2 -- -q   -f run bad-read2 < /dev/null 2> tests/userprog/bad-read2.errors > tests/userprog/bad-read2.output
perl -I../.. ../../tests/userprog/bad-read2.ck tests/userprog/bad-read2 tests/userprog/bad-read2.result
pass tests/userprog/bad-read2
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/bad-write -a bad-write -- -q   -f run bad-write < /dev/null 2> tests/userprog/bad-write.errors > tests/userprog/bad-write.output
perl -I../.. ../../tests/userprog/bad-write.ck tests/userprog/bad-write tests/userprog/bad-write.result
pass tests/userprog/bad-write
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/bad-write2 -a bad-write2 -- -q   -f run bad-write2 < /dev/null 2> tests/userprog/bad-write2.errors > tests/userprog/bad-write2.output
perl -I../.. ../../tests/userprog/bad-write2.ck tests/userprog/bad-write2 tests/userprog/bad-write2.result
pass tests/userprog/bad-write2
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/stack-align-0 -a stack-align-0 -- -q   -f run stack-align-0 < /dev/null 2> tests/userprog/stack-align-0.errors > tests/userprog/stack-align-0.output
perl -I../.. ../../tests/userprog/stack-align-0.ck tests/userprog/stack-align-0 tests/userprog/stack-align-0.result
pass tests/userprog/stack-align-0
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/stack-align-1 -a stack-align-1 -- -q   -f run stack-align-1 < /dev/null 2> tests/userprog/stack-align-1.errors > tests/userprog/stack-align-1.output
perl -I../.. ../../tests/userprog/stack-align-1.ck tests/userprog/stack-align-1 tests/userprog/stack-align-1.result
pass tests/userprog/stack-align-1
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/stack-align-2 -a stack-align-2 -- -q   -f run 'stack-align-2 a' < /dev/null 2> tests/userprog/stack-align-2.errors > tests/userprog/stack-align-2.output
perl -I../.. ../../tests/userprog/stack-align-2.ck tests/userprog/stack-align-2 tests/userprog/stack-align-2.result
pass tests/userprog/stack-align-2
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/stack-align-3 -a stack-align-3 -- -q   -f run 'stack-align-3 ab' < /dev/null 2> tests/userprog/stack-align-3.errors > tests/userprog/stack-align-3.output
perl -I../.. ../../tests/userprog/stack-align-3.ck tests/userprog/stack-align-3 tests/userprog/stack-align-3.result
pass tests/userprog/stack-align-3
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/stack-align-4 -a stack-align-4 -- -q   -f run 'stack-align-4 ab cd' < /dev/null 2> tests/userprog/stack-align-4.errors > tests/userprog/stack-align-4.output
perl -I../.. ../../tests/userprog/stack-align-4.ck tests/userprog/stack-align-4 tests/userprog/stack-align-4.result
pass tests/userprog/stack-align-4
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/do-nothing -a do-nothing -- -q   -f run do-nothing < /dev/null 2> tests/userprog/do-nothing.errors > tests/userprog/do-nothing.output
perl -I../.. ../../tests/userprog/do-nothing.ck tests/userprog/do-nothing tests/userprog/do-nothing.result
pass tests/userprog/do-nothing
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/iloveos -a iloveos -- -q   -f run iloveos < /dev/null 2> tests/userprog/iloveos.errors > tests/userprog/iloveos.output
perl -I../.. ../../tests/userprog/iloveos.ck tests/userprog/iloveos tests/userprog/iloveos.result
pass tests/userprog/iloveos
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/practice -a practice -- -q   -f run practice < /dev/null 2> tests/userprog/practice.errors > tests/userprog/practice.output
perl -I../.. ../../tests/userprog/practice.ck tests/userprog/practice tests/userprog/practice.result
pass tests/userprog/practice
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/halt -a halt -- -q   -f run halt < /dev/null 2> tests/userprog/halt.errors > tests/userprog/halt.output
perl -I../.. ../../tests/userprog/halt.ck tests/userprog/halt tests/userprog/halt.result
pass tests/userprog/halt
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/exec-arg -a exec-arg -p tests/userprog/child-args -a child-args -- -q   -f run exec-arg < /dev/null 2> tests/userprog/exec-arg.errors > tests/userprog/exec-arg.output
perl -I../.. ../../tests/userprog/exec-arg.ck tests/userprog/exec-arg tests/userprog/exec-arg.result
pass tests/userprog/exec-arg
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/exec-bad-ptr -a exec-bad-ptr -- -q   -f run exec-bad-ptr < /dev/null 2> tests/userprog/exec-bad-ptr.errors > tests/userprog/exec-bad-ptr.output
perl -I../.. ../../tests/userprog/exec-bad-ptr.ck tests/userprog/exec-bad-ptr tests/userprog/exec-bad-ptr.result
pass tests/userprog/exec-bad-ptr
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/exec-bound -a exec-bound -p tests/userprog/child-args -a child-args -- -q   -f run exec-bound < /dev/null 2> tests/userprog/exec-bound.errors > tests/userprog/exec-bound.output
perl -I../.. ../../tests/userprog/exec-bound.ck tests/userprog/exec-bound tests/userprog/exec-bound.result
pass tests/userprog/exec-bound
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/exec-bound-2 -a exec-bound-2 -- -q   -f run exec-bound-2 < /dev/null 2> tests/userprog/exec-bound-2.errors > tests/userprog/exec-bound-2.output
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/exec-bound-3 -a exec-bound-3 -- -q   -f run exec-bound-3 < /dev/null 2> tests/userprog/exec-bound-3.errors > tests/userprog/exec-bound-3.output
perl -I../.. ../../tests/userprog/exec-bound-3.ck tests/userprog/exec-bound-3 tests/userprog/exec-bound-3.result
FAIL tests/userprog/exec-bound-3
run: should have killed process: FAILED
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/exec-missing -a exec-missing -- -q   -f run exec-missing < /dev/null 2> tests/userprog/exec-missing.errors > tests/userprog/exec-missing.output
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/exec-multiple -a exec-multiple -p tests/userprog/child-simple -a child-simple -- -q   -f run exec-multiple < /dev/null 2> tests/userprog/exec-multiple.errors > tests/userprog/exec-multiple.output
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/exec-once -a exec-once -p tests/userprog/child-simple -a child-simple -- -q   -f run exec-once < /dev/null 2> tests/userprog/exec-once.errors > tests/userprog/exec-once.output
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/create-bad-ptr -a create-bad-ptr -- -q   -f run create-bad-ptr < /dev/null 2> tests/userprog/create-bad-ptr.errors > tests/userprog/create-bad-ptr.output
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/create-bound -a create-bound -- -q   -f run create-bound < /dev/null 2> tests/userprog/create-bound.errors > tests/userprog/create-bound.output
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/create-empty -a create-empty -- -q   -f run create-empty < /dev/null 2> tests/userprog/create-empty.errors > tests/userprog/create-empty.output
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/create-exists -a create-exists -- -q   -f run create-exists < /dev/null 2> tests/userprog/create-exists.errors > tests/userprog/create-exists.output
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/create-long -a create-long -- -q   -f run create-long < /dev/null 2> tests/userprog/create-long.errors > tests/userprog/create-long.output
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/create-normal -a create-normal -- -q   -f run create-normal < /dev/null 2> tests/userprog/create-normal.errors > tests/userprog/create-normal.output
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/create-null -a create-null -- -q   -f run create-null < /dev/null 2> tests/userprog/create-null.errors > tests/userprog/create-null.output
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/open-bad-ptr -a open-bad-ptr -- -q   -f run open-bad-ptr < /dev/null 2> tests/userprog/open-bad-ptr.errors > tests/userprog/open-bad-ptr.output
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/open-boundary -a open-boundary -p ../../tests/userprog/sample.txt -a sample.txt -- -q   -f run open-boundary < /dev/null 2> tests/userprog/open-boundary.errors > tests/userprog/open-boundary.output
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/open-empty -a open-empty -- -q   -f run open-empty < /dev/null 2> tests/userprog/open-empty.errors > tests/userprog/open-empty.output
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/open-missing -a open-missing -- -q   -f run open-missing < /dev/null 2> tests/userprog/open-missing.errors > tests/userprog/open-missing.output
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/open-normal -a open-normal -p ../../tests/userprog/sample.txt -a sample.txt -- -q   -f run open-normal < /dev/null 2> tests/userprog/open-normal.errors > tests/userprog/open-normal.output
pintos -v -k -T 60 --qemu  --filesys-size=2 -p tests/userprog/open-null -a open-null -- -q   -f run open-null < /dev/null 2> tests/userprog/open-null.errors > tests/userprog/open-null.output
