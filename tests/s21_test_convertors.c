#include <check.h>

#include "../s21_decimal.h"

START_TEST(from_int_to_decimal_positive) {
  int src = 12345;
  s21_decimal dst = {0};
  int result = s21_from_int_to_decimal(src, &dst);

  ck_assert_int_eq(result, 0);
  ck_assert_int_eq(dst.bits[0], src);
  ck_assert_int_eq(dst.bits[1], 0);
  ck_assert_int_eq(dst.bits[2], 0);
  ck_assert_int_eq(dst.bits[3], 0);
}
END_TEST

START_TEST(from_int_to_decimal_negative) {
  int src = -12345;
  s21_decimal dst = {0};
  int result = s21_from_int_to_decimal(src, &dst);

  ck_assert_int_eq(result, 0);
  ck_assert_int_eq(dst.bits[0], -src);
  ck_assert_int_eq(dst.bits[1], 0);
  ck_assert_int_eq(dst.bits[2], 0);
  ck_assert_int_eq(dst.bits[3], 2147483648);
}
END_TEST

START_TEST(from_int_to_decimal_zero) {
  int src = 0;
  s21_decimal dst = {0};
  int result = s21_from_int_to_decimal(src, &dst);

  ck_assert_int_eq(result, 0);
  ck_assert_int_eq(dst.bits[0], src);
  ck_assert_int_eq(dst.bits[1], 0);
  ck_assert_int_eq(dst.bits[2], 0);
  ck_assert_int_eq(dst.bits[3], 0);
}
END_TEST

START_TEST(from_int_to_decimal_null_pointer) {
  int src = 12345;
  int result = s21_from_int_to_decimal(src, NULL);

  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(from_int_to_decimal_max_int) {
  int src = 2147483647;
  s21_decimal dst = {0};
  int result = s21_from_int_to_decimal(src, &dst);

  ck_assert_int_eq(result, 0);
  ck_assert_int_eq(dst.bits[0], src);
  ck_assert_int_eq(dst.bits[1], 0);
  ck_assert_int_eq(dst.bits[2], 0);
  ck_assert_int_eq(dst.bits[3], 0);
}
END_TEST

START_TEST(from_int_to_decimal_min_int) {
  int src = -2147483648;
  s21_decimal dst = {0};
  int result = s21_from_int_to_decimal(src, &dst);

  ck_assert_int_eq(result, 0);
  ck_assert_int_eq(dst.bits[0], 2147483648);
  ck_assert_int_eq(dst.bits[1], 0);
  ck_assert_int_eq(dst.bits[2], 0);
  ck_assert_int_eq(dst.bits[3], 2147483648);
}
END_TEST

START_TEST(from_decimal_to_int_positive_integer) {
  s21_decimal src = {{123, 0, 0, 0}};
  int dst = 0;
  int ret = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(dst, 123);
}
END_TEST

START_TEST(from_decimal_to_int_negative_integer) {
  s21_decimal src = {{123, 0, 0, 0x80000000}};
  int dst = 0;
  int ret = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(dst, -123);
}
END_TEST

START_TEST(from_decimal_to_int_max_int) {
  s21_decimal src = {{0x7FFFFFFF, 0, 0, 0}};
  int dst = 0;
  int ret = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(dst, 0x7FFFFFFF);
}
END_TEST

START_TEST(from_decimal_to_int_decimal_with_fraction) {
  s21_decimal src = {{12345, 0, 0, 0x00020000}};
  int dst = 0;
  int ret = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(dst, 123);
}
END_TEST

START_TEST(from_decimal_to_int_large_fraction) {
  s21_decimal src = {{123456789, 0, 0, 0x00050000}};
  int dst = 0;
  int ret = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(dst, 1234);
}
END_TEST

START_TEST(from_decimal_to_int_null_pointer) {
  s21_decimal src = {{123, 0, 0, 0}};
  int ret = s21_from_decimal_to_int(src, NULL);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(from_decimal_to_int_out_of_bounds_positive) {
  s21_decimal src = {{0xFFFFFFFF, 0, 0, 0}};
  int dst = 0;
  int ret = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(from_decimal_to_int_out_of_bounds_negative) {
  s21_decimal src = {{0x80000001, 0, 0, 0x80000000}};
  int dst = 0;
  int ret = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(from_float_to_decimal_zero_positive) {
  s21_decimal dst;
  float value = 0.0f;
  int result = s21_from_float_to_decimal(value, &dst);
  ck_assert_int_eq(result, 0);
  ck_assert_uint_eq(dst.bits[0], 0);
  ck_assert_uint_eq(dst.bits[1], 0);
  ck_assert_uint_eq(dst.bits[2], 0);
  ck_assert_uint_eq(get_exp(dst), 0);
  ck_assert_int_eq(get_sign(dst), 0);
}
END_TEST

START_TEST(from_float_to_decimal_zero_negative) {
  s21_decimal dst;
  float value = -0.0f;
  int result = s21_from_float_to_decimal(value, &dst);
  ck_assert_int_eq(result, 0);
  ck_assert_uint_eq(dst.bits[0], 0);
  ck_assert_uint_eq(dst.bits[1], 0);
  ck_assert_uint_eq(dst.bits[2], 0);
  ck_assert_uint_eq(get_exp(dst), 0);
  ck_assert_int_eq(get_sign(dst), 1);
}
END_TEST

START_TEST(from_float_to_decimal_positive) {
  s21_decimal dst;
  float value = 12345.6789f;
  int result = s21_from_float_to_decimal(value, &dst);
  ck_assert_int_eq(result, 0);
  ck_assert_uint_eq(get_sign(dst), 0);
  ck_assert_uint_eq(get_exp(dst), 3);
  ck_assert_uint_eq(dst.bits[0], 12345680);
}
END_TEST

START_TEST(from_float_to_decimal_negative) {
  s21_decimal dst;
  float value = -98765.4321f;
  int result = s21_from_float_to_decimal(value, &dst);
  ck_assert_int_eq(result, 0);
  ck_assert_uint_eq(get_sign(dst), 1);
  ck_assert_uint_eq(get_exp(dst), 2);
  ck_assert_uint_eq(dst.bits[0], 9876543);
}
END_TEST

START_TEST(from_float_to_decimal_large_positive) {
  s21_decimal dst;
  float value = 1.2345678e+28f;
  int result = s21_from_float_to_decimal(value, &dst);
  ck_assert_int_eq(result, 0);
  ck_assert_uint_eq(get_sign(dst), 0);
  ck_assert_uint_eq(get_exp(dst), 0);
  ck_assert_uint_eq(dst.bits[0], 0);
  ck_assert_uint_eq(dst.bits[1], 0);
  ck_assert_uint_eq(dst.bits[2], 0x27E41B80);
}
END_TEST

START_TEST(from_float_to_decimal_large_negative) {
  s21_decimal dst;
  float value = -1.2345678e+28f;
  int result = s21_from_float_to_decimal(value, &dst);
  ck_assert_int_eq(result, 0);
  ck_assert_uint_eq(get_sign(dst), 1);
  ck_assert_uint_eq(get_exp(dst), 0);
  ck_assert_uint_eq(dst.bits[0], 0);
  ck_assert_uint_eq(dst.bits[1], 0);
  ck_assert_uint_eq(dst.bits[2], 0x27E41B80);
}
END_TEST

START_TEST(from_float_to_decimal_small_positive) {
  s21_decimal dst;
  float value = 1.2345678e-28f;
  int result = s21_from_float_to_decimal(value, &dst);
  ck_assert_int_eq(result, 1);
  ck_assert_uint_eq(get_sign(dst), 0);
  ck_assert_uint_eq(get_exp(dst), 0);
  ck_assert_uint_eq(dst.bits[0], 0);
}
END_TEST

START_TEST(from_float_to_decimal_small_negative) {
  s21_decimal dst;
  float value = -1.2345678e-28f;
  int result = s21_from_float_to_decimal(value, &dst);
  ck_assert_int_eq(result, 1);
  ck_assert_uint_eq(get_sign(dst), 0);
  ck_assert_uint_eq(get_exp(dst), 0);
  ck_assert_uint_eq(dst.bits[0], 0);
}
END_TEST

START_TEST(from_float_to_decimal_invalid_too_large) {
  s21_decimal dst;
  float value = 3.402823466e+38f;
  int result = s21_from_float_to_decimal(value, &dst);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(from_float_to_decimal_invalid_too_small) {
  s21_decimal dst;
  float value = 1.175494351e-38f;
  int result = s21_from_float_to_decimal(value, &dst);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(from_float_to_decimal_nan) {
  s21_decimal dst;
  float value = NAN;
  int result = s21_from_float_to_decimal(value, &dst);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(from_float_to_decimal_infinity) {
  s21_decimal dst;
  float value = INFINITY;
  int result = s21_from_float_to_decimal(value, &dst);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(from_decimal_to_float_zero) {
  s21_decimal src = {{0, 0, 0, 0}};
  float dest = 0;
  int ret = s21_from_decimal_to_float(src, &dest);
  ck_assert_int_eq(ret, 0);
  ck_assert_float_eq(dest, 0.0);
}
END_TEST

START_TEST(from_decimal_to_float_pos_fraction) {
  unsigned int exp = 1 << 16;
  s21_decimal src = {{12345, 0, 0, exp}};
  float dest = 0;
  int ret = s21_from_decimal_to_float(src, &dest);
  ck_assert_int_eq(ret, 0);
  ck_assert_float_eq(dest, 1234.5);
}
END_TEST

START_TEST(from_decimal_to_float_neg_fraction) {
  s21_decimal src = {{12345, 0, 0, -2147352576}};
  float dest = 0;
  int ret = s21_from_decimal_to_float(src, &dest);
  ck_assert_int_eq(ret, 0);
  ck_assert_float_eq(dest, -123.45);
}
END_TEST

START_TEST(from_decimal_to_float_max) {
  s21_decimal src = {{MAX_UINT, MAX_UINT, MAX_UINT, 0}};
  float dest = 0;
  int ret = s21_from_decimal_to_float(src, &dest);
  ck_assert_int_eq(ret, 0);
  ck_assert_float_eq(dest, MAX_FLOAT_VALUE);
}
END_TEST

START_TEST(from_decimal_to_float_min) {
  s21_decimal src = {{0, 0, 0, 0}};
  float dest = 0;
  int ret = s21_from_decimal_to_float(src, &dest);
  ck_assert_int_eq(ret, 0);
  ck_assert_float_eq(dest, 0.0);
}
END_TEST

START_TEST(from_decimal_to_float_large_negative) {
  s21_decimal src = {{MAX_UINT, MAX_UINT, MAX_UINT, -2147483648}};
  float dest = 0;
  int ret = s21_from_decimal_to_float(src, &dest);
  ck_assert_int_eq(ret, 0);
  ck_assert(dest == -MAX_FLOAT_VALUE);
}
END_TEST

START_TEST(from_decimal_to_float_max_scale) {
  unsigned int exp = 28 << 16;
  s21_decimal src = {{12345678, 0, 0, exp}};
  float dest = 0;
  int ret = s21_from_decimal_to_float(src, &dest);
  ck_assert_int_eq(ret, 0);
  ck_assert_float_eq(dest, 0.0000000000000000000012345678);
}
END_TEST

START_TEST(from_decimal_to_float_null) {
  s21_decimal src = {{12345678, 0, 0, 0}};
  int ret = s21_from_decimal_to_float(src, NULL);
  ck_assert_int_eq(ret, 1);
}
END_TEST

int main(void) {
  Suite *suite = suite_create("Convertor");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, from_int_to_decimal_positive);
  tcase_add_test(tc_core, from_int_to_decimal_negative);
  tcase_add_test(tc_core, from_int_to_decimal_zero);
  tcase_add_test(tc_core, from_int_to_decimal_null_pointer);
  tcase_add_test(tc_core, from_int_to_decimal_max_int);
  tcase_add_test(tc_core, from_int_to_decimal_min_int);
  tcase_add_test(tc_core, from_decimal_to_int_positive_integer);
  tcase_add_test(tc_core, from_decimal_to_int_negative_integer);
  tcase_add_test(tc_core, from_decimal_to_int_max_int);
  tcase_add_test(tc_core, from_decimal_to_int_decimal_with_fraction);
  tcase_add_test(tc_core, from_decimal_to_int_large_fraction);
  tcase_add_test(tc_core, from_decimal_to_int_null_pointer);
  tcase_add_test(tc_core, from_decimal_to_int_out_of_bounds_positive);
  tcase_add_test(tc_core, from_decimal_to_int_out_of_bounds_negative);
  tcase_add_test(tc_core, from_float_to_decimal_zero_positive);
  tcase_add_test(tc_core, from_float_to_decimal_zero_negative);
  tcase_add_test(tc_core, from_float_to_decimal_positive);
  tcase_add_test(tc_core, from_float_to_decimal_negative);
  tcase_add_test(tc_core, from_float_to_decimal_large_positive);
  tcase_add_test(tc_core, from_float_to_decimal_large_negative);
  tcase_add_test(tc_core, from_float_to_decimal_small_positive);
  tcase_add_test(tc_core, from_float_to_decimal_small_negative);
  tcase_add_test(tc_core, from_float_to_decimal_invalid_too_large);
  tcase_add_test(tc_core, from_float_to_decimal_invalid_too_small);
  tcase_add_test(tc_core, from_float_to_decimal_nan);
  tcase_add_test(tc_core, from_float_to_decimal_infinity);
  tcase_add_test(tc_core, from_decimal_to_float_zero);
  tcase_add_test(tc_core, from_decimal_to_float_pos_fraction);
  tcase_add_test(tc_core, from_decimal_to_float_neg_fraction);
  tcase_add_test(tc_core, from_decimal_to_float_max);
  tcase_add_test(tc_core, from_decimal_to_float_min);
  tcase_add_test(tc_core, from_decimal_to_float_large_negative);
  tcase_add_test(tc_core, from_decimal_to_float_max_scale);
  tcase_add_test(tc_core, from_decimal_to_float_null);

  suite_add_tcase(suite, tc_core);

  SRunner *runner = srunner_create(suite);

  srunner_run_all(runner, CK_NORMAL);
  int number_failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}