/*#include "mock_memory.h"

int8_t __wrap_my_memmove(uint8_t* src, uint8_t* dst, uint32_t length) {
  check_expected_ptr(src);
  check_expected_ptr(dst);

  return (int8_t) mock();
}

int8_t __wrap_my_memset(uint8_t* src, uint32_t length, uint8_t value) {
  check_expected_ptr(src);

  return (int8_t) mock();
}

int8_t __wrap_my_memzero(uint8_t* src, uint32_t length) {
  check_expected_ptr(src);

  return (int8_t) mock();
}

int8_t __wrap_my_reverse(uint8_t* src, uint32_t length) {
  check_expected_ptr(src);

  return (int8_t) mock();
}
*/
