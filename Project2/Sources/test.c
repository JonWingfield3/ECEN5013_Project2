#include <stdbool.h>
#include <stdarg.h>
#include <setjmp.h>
#include <stdlib.h>
#include "/usr/include/cmocka.h"

#include "circbuf.h"
#include "data.h"
#include "memory.h"
/*
#include "mock_circbuf.h"
#include "mock_data.h"
#include "mock_memory.h"
*/

#include <stdint.h>

static void test_invalid_pointer_mem() {
  uint8_t * src = NULL;
  uint8_t dst[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE};
  uint32_t length = 3;

  int ret1 = (int) my_memmove(src, dst, length);

  src = dst;
  uint8_t * ndst = NULL;

  int ret2 = (int) my_memmove(src, ndst, length);

  assert_int_equal(ret1, -1);
  assert_int_equal(ret2, -1);
}

int main() {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(test_invalid_pointer_mem),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
