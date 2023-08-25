/* Tries to create a directory named as the empty string,
   which must return failure. */

#include <syscall.h>
#include "tests/lib.h"
#include "tests/main.h"

int anchor(){
  return 1+2;
}
void test_main(void) { 
  anchor();
  CHECK(!mkdir(""), "mkdir \"\" (must return false)"); 
}
