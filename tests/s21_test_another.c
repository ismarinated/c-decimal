#include <check.h>

#include "../s21_decimal.h"

START_TEST(negate_test_1) {
  s21_decimal src = {{12, 12, 12, MINUS}};
  s21_decimal dest = {0};
  int ret = s21_negate(src, &dest);
  ck_assert_uint_eq(dest.bits[3], 0);
  ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(negate_test_2) {
  s21_decimal src = {{12, 12, 12, 0}};
  s21_decimal dest = {0};
  int ret = s21_negate(src, &dest);
  ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(floor_test_positive_number) {
  uint32 exp = 1 << 16;
  s21_decimal value_1 = {{34, 0, 0, exp}};
  s21_decimal result = {0};
  int ret = s21_floor(value_1, &result);
  ck_assert_uint_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], 3);
  ck_assert_uint_eq(result.bits[1], 0);
  ck_assert_uint_eq(result.bits[2], 0);
  ck_assert_uint_eq(result.bits[3], 0);
}
END_TEST

START_TEST(floor_test_negative_number) {
  uint32 exp = 1 << 16;
  s21_decimal value_1 = {{34, 0, 0, MINUS | exp}};
  s21_decimal result = {0};
  int ret = s21_floor(value_1, &result);
  ck_assert_uint_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], 4);
  ck_assert_uint_eq(result.bits[1], 0);
  ck_assert_uint_eq(result.bits[2], 0);
}
END_TEST

START_TEST(truncate_test) {
  uint32 exp = 2 << 16;
  s21_decimal value_1 = {{342, 0, 0, MINUS | exp}};
  s21_decimal result = {0};
  int ret = s21_truncate(value_1, &result);
  ck_assert_uint_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], 3);
  ck_assert_uint_eq(result.bits[1], 0);
  ck_assert_uint_eq(result.bits[2], 0);
}
END_TEST

START_TEST(round_test) {
  uint32 exp = 2 << 16;
  s21_decimal value_1 = {{362, 0, 0, exp}};
  s21_decimal result = {0};
  int ret = s21_round(value_1, &result);
  ck_assert_uint_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], 4);
  ck_assert_uint_eq(result.bits[1], 0);
  ck_assert_uint_eq(result.bits[2], 0);
  ck_assert_uint_eq(result.bits[3], 0);
}
END_TEST

START_TEST(round_test_2) {
  uint32 exp = 5 << 16;
  s21_decimal value_1 = {{36555555, 0, 0, exp}};
  s21_decimal result = {0};
  int ret = s21_round(value_1, &result);
  ck_assert_uint_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], 366);
  ck_assert_uint_eq(result.bits[1], 0);
  ck_assert_uint_eq(result.bits[2], 0);
  ck_assert_uint_eq(result.bits[3], 0);
}
END_TEST

int main(void) {
  Suite *suite = suite_create("Another");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, negate_test_1);
  tcase_add_test(tc_core, negate_test_2);
  tcase_add_test(tc_core, floor_test_positive_number);
  tcase_add_test(tc_core, floor_test_negative_number);
  tcase_add_test(tc_core, truncate_test);
  tcase_add_test(tc_core, round_test);
  tcase_add_test(tc_core, round_test_2);

  suite_add_tcase(suite, tc_core);

  SRunner *runner = srunner_create(suite);

  srunner_run_all(runner, CK_NORMAL);
  int number_failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}