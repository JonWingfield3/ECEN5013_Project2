#include <stdbool.h>
#include <stdarg.h>
#include <setjmp.h>
#include <stdlib.h>
#include <cmocka.h>

#include <stdio.h>

#include "circbuf.h"
#include "data.h"
#include "memory.h"
#include "defines.h"

#include "mock_memory.h"


#include <stdint.h>

#ifdef MEMORY
// Will test that memmove returns properly when passed a null pointer input
static void test_invalid_pointer_memmove(void **state) {
  uint8_t * src = NULL;
  uint8_t dst[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE};
  uint32_t length = 3;

  // Test with NULL src
  int ret1 = (int) my_memmove(src, dst, length);

  src = dst;
  uint8_t * ndst = NULL;

  // Test with NULL ndst
  int ret2 = (int) my_memmove(src, ndst, length);

  assert_int_equal(ret1, PTR_ERROR);
  assert_int_equal(ret2, PTR_ERROR);
}

// Tests that memmove works correctly on memory regions with no overlap
static void test_overlap_memmove(void **state) {
  uint8_t src[] = {0xAA, 0xBB, 0xCC, 0xDD};
  uint8_t dst[] = {0xEE,0xFF,0x00,0x11};
  uint32_t length = 4;

  // test mocve with no overlap
  int ret = (int) my_memmove(src, dst, length);
  uint32_t i;
  assert_int_equal(ret, SUCCESS);
  for(i = 0; i < length; i ++) {
    assert_true(dst[i] == src[i]);
  }
}

// Tests that memmove works correctly when the source region is a subset of the destination region
static void test_SRC_DST_overlap_memmove(void **state) {
  uint8_t dst[] = {0xAA, 0xBB, 0xCC, 0xDD};
  uint8_t * src = dst + 1;
  src[3] = 0x99;
  uint32_t length = 4;
  // Need temporary holder to compare destination to as src will be overwritten
  uint8_t hld[] = {0xBB, 0xCC, 0xDD, 0x99};


  // Test when src is in dst
  int ret = (int) my_memmove(src, dst, length);
  uint32_t i;
  assert_int_equal(ret, SUCCESS);
  for(i = 0; i < length; i ++) {
    assert_true(dst[i] == hld[i]);

  }
}

// Tests that memmove works correctly when the destination region is  a subset of the source region
static void test_DST_SRC_overlap_memmove(void **state) {
  uint8_t src[] = {0xAA, 0xBB, 0xCC, 0xDD};
  uint8_t * dst = src + 1;
  dst[3] = 0xEE;
  uint32_t length = 4;
  // Need temporary holder to compare destination to as src will be overwritten
  uint8_t hld[] = {0xAA, 0xBB, 0xCC, 0xDD};

  // test when dst is in src
  int ret = (int) my_memmove(src, dst, length);
  uint32_t i;
  assert_int_equal(ret, SUCCESS);
  for(i = 0; i < length; i ++) {
    assert_true(dst[i] == hld[i]);
  }
}

// Tests that memset returns correctly with null pointer input
static void test_invalid_pointer_memset(void **state) {
  uint8_t * src = NULL;
  uint32_t length = 4;
  uint8_t value = 17;

  int ret = (int) my_memset(src, length, value);

  assert_int_equal(ret, PTR_ERROR);
}

// Tests that memset works correctly for entire array and susbset of array
static void test_check_set_memset(void **state) {
  uint8_t * src;
  uint8_t arr[] = {9,8,7,6,5,0};
  src = arr;
  uint32_t length = 6;
  uint8_t value = 4;
  int i = 0;

  int ret1 = (int) my_memset(src, length, value);
  for(i = 0; i < length; i++) {
    assert_true(*(src+i) == value);
  }

  value = 0;
  length = 2;
  src = src + 3;

  int ret2 = (int) my_memset(src, length, value);
  for(i = 0; i < length; i++) {
    assert_true(*(src+i) == value);
  }

  assert_int_equal(ret1, SUCCESS);
  assert_int_equal(ret2, SUCCESS);
}

// Tests that memzero returns correctly with null pointer input
static void test_invalid_pointer_memzero(void **state) {
  uint8_t * src = NULL;
  uint32_t length = 5;

  int ret = (int) my_memzero(src, length);

  assert_int_equal(ret, PTR_ERROR);
}

// Tests that memzero works correctly for  entire array and subset of array
static void test_check_set_memzero(void **state) {
  uint8_t * src = NULL;
  uint8_t arr[] = {1,2,3,4,5,6,7};
  src = arr;
  uint32_t length = 5;
  int i;

  int ret1 = (int) my_memzero(src, length);
  for(i = 0; i < length; i++) {
    assert_true(*(src+i) == 0);
  }

  length = 2;
  uint8_t ard[] = {1,2,3,4,5,6,7};
  src = ard;
  src = src + 3;

  int ret2 = (int) my_memzero(src, length);
  for(i = 0; i < length; i++) {
    assert_true(*(src+i) == 0);
  }

  assert_int_equal(ret1, SUCCESS);
  assert_int_equal(ret2, SUCCESS);
}

// Test that reverse returns correctly with null pointer input
static void test_invalid_pointer_reverse(void **state) {
  uint8_t * src = NULL;
  uint32_t length = 5;

  int ret = (int) my_reverse(src, length);

  assert_int_equal(ret, PTR_ERROR);
}

// Tests that reverse functions correctly for odd numbered arrays
static void test_odd_reverse(void **state) {
  uint8_t src[] = {1,2,3,4,5};
  uint32_t length = 5;

  uint8_t comp[] = {5,4,3,2,1};
  int i;

  int ret = (int) my_reverse(src, length);

  assert_int_equal(ret, SUCCESS);
  for(i = 0; i < length; i++) {
    assert_true(src[i] == comp[i]);
  }
}

// Test that reverse functions correctly for even numbered arrays
static void test_even_reverse(void **state) {
  uint8_t src[] = {1,2,3,4};
  uint32_t length = 4;

  uint8_t comp[] = {4,3,2,1};
  int i;

  int ret = (int) my_reverse(src, length);

  assert_int_equal(ret, SUCCESS);
  for(i = 0; i < length; i++) {
    assert_true(src[i] == comp[i]);
  }
}

// Tests that reverse correctly revereses input arrays
static void test_check_characters_reverse(void **state) {
  uint8_t src[256];
  uint32_t length = 256;
  int i;
  for(i = 0; i < (int)length; i++) {
    src[i] = i;
  }

  uint8_t comp[256];
  for(i = 0; i < (int)length; i++) {
    comp[i] = 255 - i;
  }

  int ret = (int) my_reverse(src, length);

  assert_int_equal(ret, SUCCESS);
  for(i = 0; i < length; i++) {
    assert_true(src[i] == comp[i]);
  }
}
#endif
/**********************************************/
#ifdef DATA
// Test that big to little returns correctly with null pointer input
static void test_invalid_pointer_BtL(void **state) {
  uint8_t * src = NULL;
  uint32_t length = 1;

  int ret = (int) big_to_little32((uint32_t *)src, length);
  assert_int_equal(ret, PTR_ERROR);
}

// Tests that big to little correctly converts inputs
static void test_valid_converstion_BtL(void **state) {
  uint8_t * src = NULL;
  uint8_t arr[] = {0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,0x00,0x11};
  uint8_t ar2[] = {0xDD,0xCC,0xBB,0xAA,0x11,0x00,0xFF,0xEE};
  src = arr;
  uint32_t length = 2;
  int i;
  int j = 0;
  for(i = 1; i <= length; i++) {
    while(j < i*4) {
      will_return(__wrap_my_reverse, ar2[j]);
      j++;
    }
    will_return(__wrap_my_reverse, SUCCESS);
  }

  int ret = big_to_little32((uint32_t *)src, length);

  assert_int_equal(ret, SUCCESS);
  for(i = 0; i < 4 * length; i++) {
    assert_true(src[i] == ar2[i]);
  }
}

// Test that little to big returns correctly with null pointer input
static void test_invalid_pointer_LtB(void **state) {
  uint8_t * src = NULL;
  uint32_t length = 1;

  int ret = (int) little_to_big32((uint32_t *)src, length);
  assert_int_equal(ret, PTR_ERROR);
}

// Tests that little to big correctly converts inputs
static void test_valid_conversion_LtB(void **state) {
  uint8_t * src = NULL;
  uint8_t arr[] = {0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,0x00,0x11};
  uint8_t ar2[] = {0xDD,0xCC,0xBB,0xAA,0x11,0x00,0xFF,0xEE};
  src = arr;
  uint32_t length = 2;
  int i;
  int j = 0;
  for(i = 1; i <= length; i++) {
    while(j < i*4) {
      will_return(__wrap_my_reverse, ar2[j]);
      j++;
    }
    will_return(__wrap_my_reverse, SUCCESS);
  }

  int ret = little_to_big32((uint32_t *)src, length);

  assert_int_equal(ret, SUCCESS);
  for(i = 0; i < 4 * length; i++) {
    assert_true(src[i] == ar2[i]);
  }
}
#endif
/************************************************/
#ifdef CIRCBUF
// Test that bufferinit functions correctly for various inputs
static void test_allocate_free(void **state) {
  CircBuf buf;
  uint32_t size = 0;

  int ret = (int) BufferInit(NULL, size);
  assert_int_equal(ret, PTR_ERROR_BUF);

  ret = (int) BufferInit(&buf, size);
  assert_int_equal(ret, INIT_FAILURE);

  size = 256;
  ret = (int) BufferInit(&buf, size);
  assert_int_equal(ret, SUCCESS_BUF);

  ret = (int) BufferDestroy(NULL);
  assert_int_equal(ret, PTR_ERROR_BUF);

  ret = (int) BufferDestroy(&buf);
  assert_int_equal(ret, SUCCESS_BUF);
}

// Test that all functions in circbuf return correctly with null pointer input
static void test_invalid_pointer_circbuf(void **state) {
  CircBuf buf;
  uint32_t size = 256;
  int ret;
  CircBufData_t data = 28;
  CircBufData_t * ptr = NULL;
  uint32_t n = 2;

  ret = (int) BufferAdd(NULL, data);
  assert_int_equal(ret, PTR_ERROR_BUF);

  ret = (int) BufferRemove(NULL, ptr);
  assert_int_equal(ret, PTR_ERROR_BUF);

  ret = (int) BufferFull(NULL);
  assert_int_equal(ret, PTR_ERROR_BUF);

  ret = (int) BufferEmpty(NULL);
  assert_int_equal(ret, PTR_ERROR_BUF);

  ret = (int) BufferPeek(NULL, ptr, n);
  assert_int_equal(ret, PTR_ERROR_BUF);

  ret = (int) BufferDestroy(NULL);
  assert_int_equal(ret, PTR_ERROR_BUF);
/*******************************************/
  ret = (int) BufferInit(&buf, size);
  assert_int_equal(ret, SUCCESS_BUF);

  ret = (int) BufferAdd(&buf, data);
  assert_int_equal(ret, SUCCESS_BUF);

  ret = (int) BufferRemove(&buf, ptr);
  assert_int_equal(ret, SUCCESS_BUF);

  ret = (int) BufferFull(&buf);
  assert_int_equal(ret, BUFFER_NOT_FULL);

  ret = (int) BufferEmpty(&buf);
  assert_int_equal(ret, BUFFER_EMPTY);

  ret = (int) BufferPeek(&buf, ptr, n);
  assert_int_equal(ret, PTR_ERROR_BUF);
/*******************************************/
  ret = (int) BufferDestroy(&buf);
  assert_int_equal(ret, SUCCESS_BUF);
}

// Tests that buffer is initialized
static void test_non_init_buff(void **state) {
  CircBuf buf;
  uint32_t size = 256;
  int ret = (int) BufferInit(&buf, size);
  assert_int_equal(ret, SUCCESS_BUF);

  assert_true(buf.buffer);

  ret = (int) BufferDestroy(&buf);
  assert_int_equal(ret, SUCCESS_BUF);
}

// Tests that removed items are identical to the added items
static void test_add_remove(void **state) {
  CircBuf buf;
  uint32_t size = 256;
  CircBufData_t data = 26;
  CircBufData_t ptr;
  int ret = (int) BufferInit(&buf, size);
  assert_int_equal(ret, SUCCESS_BUF);

  BufferAdd(&buf, data);
  BufferRemove(&buf, &ptr);

  assert_true(ptr == data);

  ret = (int) BufferDestroy(&buf);
  assert_int_equal(ret, SUCCESS_BUF);
}

// Tests that bufferfull returns correctly when buffer is full
static void test_buffer_full(void **state) {
  CircBuf buf;
  uint32_t size = 256;
  int ret = (int) BufferInit(&buf, size);
  assert_int_equal(ret, SUCCESS_BUF);

  ret = (int) BufferFull(&buf);
  assert_int_equal(ret, BUFFER_NOT_FULL);

  int i;
  for(i = 0; i < size; i++) {
    BufferAdd(&buf, (CircBufData_t)i);
  }

  ret = (int) BufferFull(&buf);
  assert_int_equal(ret, BUFFER_FULL);

  ret = (int) BufferDestroy(&buf);
  assert_int_equal(ret, SUCCESS_BUF);
}

// Tests that bufferempty returns correctly based on buffer state
static void test_buffer_empty(void **state) {
  CircBuf buf;
  uint32_t size = 256;
  int ret = (int) BufferInit(&buf, size);
  assert_int_equal(ret, SUCCESS_BUF);

  ret = BufferEmpty(&buf);
  assert_int_equal(ret, BUFFER_EMPTY);

  int i;
  for(i = 0; i < size; i++) {
    BufferAdd(&buf, (CircBufData_t)i);
  }

  ret = (int) BufferEmpty(&buf);
  assert_int_equal(ret, BUFFER_NOT_EMPTY);

  ret = (int) BufferDestroy(&buf);
  assert_int_equal(ret, SUCCESS_BUF);
}

// Test that buffer adds across wrap
static void test_wrap_add(void **state) {
  CircBuf buf;
  uint32_t size = 256;
  CircBufData_t * ptr = NULL;
  int ret = (int) BufferInit(&buf, size);
  assert_int_equal(ret, SUCCESS_BUF);

  int i;
  // Fill buffer
  for(i = 0; i < size; i++) {
    BufferAdd(&buf, (CircBufData_t)i);
  }
  // Remove one item
  BufferRemove(&buf, ptr);

  // Add at wrap point
  ret = (int) BufferAdd(&buf, (CircBufData_t)i);
  assert_int_equal(ret, SUCCESS_BUF);

  ret = (int) BufferDestroy(&buf);
  assert_int_equal(ret, SUCCESS_BUF);
}

// Tests that buffer removes across wrap
static void test_wrap_remove(void **state) {
  CircBuf buf;
  uint32_t size = 256;
  CircBufData_t * ptr = NULL;
  int ret = (int) BufferInit(&buf, size);
  assert_int_equal(ret, SUCCESS_BUF);

  int i;
  // Fill buffer
  for(i = 0; i < size; i++) {
    BufferAdd(&buf, (CircBufData_t)i);
  }
  // remove one item
  BufferRemove(&buf, ptr);

  // Add at wrap point
  BufferAdd(&buf, (CircBufData_t)i);
  // Remove all but one
  for(i = 0; i < size - 1; i++) {
    BufferRemove(&buf, ptr);
  }

  // Remove at wrap point
  ret = (int) BufferRemove(&buf, ptr);
  assert_int_equal(ret, SUCCESS_BUF);

  ret = (int) BufferDestroy(&buf);
  assert_int_equal(ret, SUCCESS_BUF);
}

// Test that buffer doesn't overwrite
static void test_over_fill(void **state) {
  CircBuf buf;
  uint32_t size = 256;
  int ret = (int) BufferInit(&buf, size);
  assert_int_equal(ret, SUCCESS_BUF);

  int i;
  // Fill buffer
  for(i = 0; i < size; i++) {
    BufferAdd(&buf, (CircBufData_t)i);
  }

  // Attempt to add another item
  ret = (int) BufferAdd(&buf, (CircBufData_t) i);
  assert_int_equal(ret, OVERWRITE);

  ret = (int) BufferDestroy(&buf);
  assert_int_equal(ret, SUCCESS_BUF);
}

// Tests that buffer deosn't remove from an empty buffer
static void test_over_empty(void **state) {
  CircBuf buf;
  uint32_t size = 256;
  CircBufData_t * ptr = NULL;
  int ret = (int) BufferInit(&buf, size);
  assert_int_equal(ret, SUCCESS_BUF);

  // Attempt to remove non-existent item
  ret = (int) BufferRemove(&buf, ptr);
  assert_int_equal(ret, ITEM_REMOVE_FAILURE);

  ret = (int) BufferDestroy(&buf);
  assert_int_equal(ret, SUCCESS_BUF);
}
#endif

int main() {
  const struct CMUnitTest tests[] = {
#ifdef MEMORY
    cmocka_unit_test(test_invalid_pointer_memmove),
    cmocka_unit_test(test_overlap_memmove),
    cmocka_unit_test(test_SRC_DST_overlap_memmove),
    cmocka_unit_test(test_DST_SRC_overlap_memmove),
    cmocka_unit_test(test_invalid_pointer_memset),
    cmocka_unit_test(test_check_set_memset),
    cmocka_unit_test(test_invalid_pointer_memzero),
    cmocka_unit_test(test_check_set_memzero),
    cmocka_unit_test(test_invalid_pointer_reverse),
    cmocka_unit_test(test_odd_reverse),
    cmocka_unit_test(test_even_reverse),
    cmocka_unit_test(test_check_characters_reverse),
#endif
#ifdef DATA
    cmocka_unit_test(test_invalid_pointer_BtL),
    cmocka_unit_test(test_valid_converstion_BtL),
    cmocka_unit_test(test_invalid_pointer_LtB),
    cmocka_unit_test(test_valid_conversion_LtB),
#endif
#ifdef CIRCBUF
    cmocka_unit_test(test_allocate_free),
    cmocka_unit_test(test_invalid_pointer_circbuf),
    cmocka_unit_test(test_non_init_buff),
    cmocka_unit_test(test_add_remove),
    cmocka_unit_test(test_buffer_full),
    cmocka_unit_test(test_buffer_empty),
    cmocka_unit_test(test_wrap_add),
    cmocka_unit_test(test_wrap_remove),
    cmocka_unit_test(test_over_fill),
    cmocka_unit_test(test_over_empty),
#endif
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
