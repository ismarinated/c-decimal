#include <check.h>

#include "../s21_decimal.h"

START_TEST(add_test_1) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal dest = {0};
  int ret = s21_add(value_1, value_2, &dest);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(dest.bits[0], 0);
  ck_assert_uint_eq(dest.bits[1], 0);
  ck_assert_uint_eq(dest.bits[2], 0);
  ck_assert_uint_eq(dest.bits[3], 0);
}
END_TEST

START_TEST(add_test_2) {
  s21_decimal value_1 = {{1234, 124, 242, 24}};
  s21_decimal value_2 = {0};
  s21_decimal dest = {0};
  int ret = s21_add(value_1, value_2, &dest);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(dest.bits[0], 1234);
  ck_assert_uint_eq(dest.bits[1], 124);
  ck_assert_uint_eq(dest.bits[2], 242);
}
END_TEST

START_TEST(add_test_3) {
  s21_decimal value_1 = {{12345, 0, 0, 0}};
  s21_decimal value_2 = {{12345, 0, 0, 0}};
  set_scale(&value_1, 1);
  set_scale(&value_2, 3);
  s21_decimal dest = {0};
  int ret = s21_add(value_1, value_2, &dest);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(dest.bits[0], 1246845);
  ck_assert_uint_eq(dest.bits[1], 0);
  ck_assert_uint_eq(dest.bits[2], 0);
  ck_assert_uint_eq(dest.bits[3], value_2.bits[3]);
}
END_TEST

START_TEST(add_test_4) {
  s21_decimal value_1 = {{12345, 0, 0, 0}};
  s21_decimal value_2 = {{12345, 0, 0, 0}};
  set_scale(&value_1, 1);
  set_scale(&value_2, 3);
  set_sign(&value_2, 1);
  s21_decimal dest = {0};
  unsigned int res_exp = 3 << 16;
  int ret = s21_add(value_1, value_2, &dest);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(dest.bits[0], 1222155);
  ck_assert_uint_eq(dest.bits[1], 0);
  ck_assert_uint_eq(dest.bits[2], 0);
  ck_assert_uint_eq(dest.bits[3], res_exp);
}
END_TEST

START_TEST(add_pos_pos) {
  s21_decimal var1 = {{50, 0, 0, 0}};
  s21_decimal var2 = {{6, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal ch = {{56, 0, 0, 0}};
  int ret = s21_add(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(add_pos_posE) {
  s21_decimal var1 = {{50, 0, 0, 0}};
  s21_decimal var2 = {{777, 3, 0, 0}};
  var2.bits[3] = 3 << 16;
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal ch = {{50777, 3, 0, 0}};
  ch.bits[3] = 3 << 16;
  int ret = s21_add(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(add_posE_posE) {
  s21_decimal var1 = {{8666, 5, 0, 0}};
  var1.bits[3] = 5 << 16;
  s21_decimal var2 = {{777, 3, 0, 0}};
  var2.bits[3] = 10 << 16;
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal ch = {{866600777, 500003, 0, 0}};
  ch.bits[3] = 10 << 16;
  int ret = s21_add(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(add_neg_neg) {
  s21_decimal var1 = {{50, 0, 0, 0}};
  set_sign(&var1, 1);
  s21_decimal var2 = {{50, 0, 0, 0}};
  set_sign(&var2, 1);
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal ch = {{100, 0, 0, 0}};
  set_sign(&ch, 1);
  int ret = s21_add(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(add_neg_negE) {
  s21_decimal var1 = {{50, 0, 0, 0}};
  set_sign(&var1, 1);
  s21_decimal var2 = {{777, 3, 0, 0}};
  var2.bits[3] = 3 << 16;
  set_sign(&var2, 1);
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal ch = {{50777, 3, 0, 0}};
  ch.bits[3] = 3 << 16;
  set_sign(&ch, 1);
  int ret = s21_add(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(add_negE_negE) {
  s21_decimal var1 = {{50, 60, 0, 0}};
  set_sign(&var1, 1);
  var1.bits[3] |= 3 << 16;
  s21_decimal var2 = {{4, 5555, 333, 0}};
  var2.bits[3] = 7 << 16;
  set_sign(&var2, 1);
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal ch = {{500004, 605555, 333, 0}};
  ch.bits[3] = 7 << 16;
  set_sign(&ch, 1);
  int ret = s21_add(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(add_pos_neg) {
  s21_decimal var1 = {{50, 50, 0, 0}};
  s21_decimal var2 = {{50, 0, 0, 0}};
  set_sign(&var2, 1);
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal ch = {{0, 50, 0, 0}};
  int ret = s21_add(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}

START_TEST(add_neg_pos) {
  s21_decimal var1 = {{50, 50, 0, 0}};
  set_sign(&var1, 1);
  s21_decimal var2 = {{0, 50, 0, 0}};
  s21_decimal ch = {{50, 0, 0, 0}};
  set_sign(&ch, 1);
  s21_decimal result = {{0, 0, 0, 0}};
  int ret = s21_add(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(add_0_0) {
  s21_decimal var1 = {{0, 0, 0, 0}};
  s21_decimal var2 = {{0, 0, 0, 0}};
  s21_decimal ch = {{0, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int ret = s21_add(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(sub_pos_pos) {
  s21_decimal var1 = {{50, 50, 50, 0}};
  s21_decimal var2 = {{50, 50, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal ch = {{0, 0, 50, 0}};
  int ret = s21_sub(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(sub_pos_pos2) {
  s21_decimal var1 = {{0, 50, 50, 0}};
  s21_decimal var2 = {{50, 50, 50, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal ch = {{50, 0, 0, 0}};
  set_sign(&ch, 1);
  int ret = s21_sub(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(sub_pos_posE) {
  s21_decimal var1 = {{30, 00, 00, 0}};
  s21_decimal var2 = {{55555550, 50, 0, 0}};
  var2.bits[3] = 3 << 16;
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal ch = {{55525550, 50, 0, 0}};
  ch.bits[3] = 3 << 16;
  set_sign(&ch, 1);
  int ret = s21_sub(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(sub_posE_posE) {
  s21_decimal var1 = {{7, 55, 0, 0}};
  var1.bits[3] = 6 << 16;
  s21_decimal var2 = {{30, 50, 0, 0}};
  var2.bits[3] = 7 << 16;
  s21_decimal ch = {{40, 500, 0, 0}};
  ch.bits[3] = 7 << 16;
  s21_decimal result = {{0, 0, 0, 0}};
  int ret = s21_sub(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(sub_neg_neg) {
  s21_decimal var1 = {{7, 33, 0, 0}};
  set_sign(&var1, 1);
  s21_decimal var2 = {{10, 55, 0, 0}};
  set_sign(&var2, 1);
  s21_decimal ch = {{3, 22, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int ret = s21_sub(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(sub_neg_negE) {
  s21_decimal var1 = {{7, 55, 0, 0}};
  set_sign(&var1, 1);
  s21_decimal var2 = {{30, 50, 0, 0}};
  set_sign(&var2, 1);
  var2.bits[3] |= (2 << 16);
  s21_decimal ch = {{670, 5450, 0, 0}};
  set_sign(&ch, 1);
  ch.bits[3] |= (2 << 16);
  s21_decimal result = {{0, 0, 0, 0}};
  int ret = s21_sub(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(sub_negE_negE) {
  s21_decimal var1 = {{70, 55, 0, 0}};
  set_sign(&var1, 1);
  var1.bits[3] |= (4 << 16);
  s21_decimal var2 = {{30, 50, 0, 0}};
  set_sign(&var2, 1);
  var2.bits[3] |= (3 << 16);
  s21_decimal ch = {{230, 445, 0, 0}};
  ch.bits[3] |= (4 << 16);
  s21_decimal result = {{0, 0, 0, 0}};
  int ret = s21_sub(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(div_test_1) {
  int a = 5;
  int b = 2;
  int c = -1;
  s21_decimal _a;
  s21_decimal _b;
  s21_decimal res;
  s21_from_int_to_decimal(a, &_a);
  s21_from_int_to_decimal(b, &_b);
  s21_div(_a, _b, &res);
  s21_from_decimal_to_int(res, &c);
  ck_assert_int_eq(a / b, c);
}
END_TEST

START_TEST(div_test_2) {
  float a = 0.6;
  float b = 3.0;

  float c = 0;

  s21_decimal _a;
  s21_decimal _b;
  s21_decimal res;

  s21_from_float_to_decimal(a, &_a);
  s21_from_float_to_decimal(b, &_b);
  s21_div(_a, _b, &res);
  s21_from_decimal_to_float(res, &c);

  if (c != a / b) printf("%f %f | %f %f\n", a, b, a / b, c);
  ck_assert_float_eq(a / b, c);
}
END_TEST

START_TEST(div_test_3) {
  float a = 1;
  float b = 0.2;

  float c = 0;

  s21_decimal _a;
  s21_decimal _b;
  s21_decimal res;

  s21_from_float_to_decimal(a, &_a);
  s21_from_float_to_decimal(b, &_b);
  s21_div(_a, _b, &res);

  s21_from_decimal_to_float(res, &c);

  ck_assert_float_eq(a / b, c);
}
END_TEST

START_TEST(sub_pos_neg) {
  s21_decimal var1 = {{50, 50, 50, 0}};
  s21_decimal var2 = {{50, 50, 50, 0}};
  set_sign(&var2, 1);
  s21_decimal ch = {{100, 100, 100, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int ret = s21_sub(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(sub_neg_pos) {
  s21_decimal var1 = {{50, 50, 50, 0}};
  set_sign(&var1, 1);
  var1.bits[3] |= 3 << 16;
  s21_decimal var2 = {{50, 50, 50, 0}};
  s21_decimal ch = {{50050, 50050, 50050, 0}};
  set_sign(&ch, 1);
  ch.bits[3] |= 3 << 16;
  s21_decimal result = {{0, 0, 0, 0}};
  int ret = s21_sub(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(sub_0_0) {
  s21_decimal var1 = {{0, 0, 0, 0}};
  s21_decimal var2 = {{0, 0, 0, 0}};
  s21_decimal ch = {{0, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int ret = s21_sub(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(sub_pos29_pos) {
  s21_decimal var1 = {{50, 0, 0, 0}};
  var1.bits[3] |= (29 << 16);
  s21_decimal ch = {{0, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(mul_test_1) {
  s21_decimal var1 = {{50, 0, 0, MINUS}};
  unsigned int n = 1 << 16;
  s21_decimal var2 = {{32, 0, 0, n}};
  s21_decimal result = {{0, 0, 0, 0}};
  int ret = s21_mul(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[1], 0);
  ck_assert_uint_eq(result.bits[2], 0);
}
END_TEST

START_TEST(mul_test_2) {
  unsigned int n = 27 << 16;
  s21_decimal var1 = {{1234321345, 1234321345, 1234321345, n}};
  s21_decimal var2 = {{1234321345, 1234321345, 1234321345, n}};
  s21_decimal result = {{0, 0, 0, 0}};
  int ret = s21_mul(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
}
END_TEST

static s21_decimal num1[] = {
    {{0x0000000A, 0x00000000, 0x00000000, 0x00000000}},  // 0 10
    {{0x0000000A, 0x00000000, 0x00000000, 0x00000000}},  // 1 10
    {{0x0001F0F1, 0x00000000, 0x00000000, 0x00000000}},  // 3 127217
    {{0x000000D4, 0x00000000, 0x00000000, 0x80000000}},  // 4 -212
    {{0x014CF9BF, 0x00000000, 0x00000000, 0x80020000}},  // 5 -218218.87
    {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},  // 6 0
    {{0x0001DC88, 0x00000000, 0x00000000, 0x00020000}},  // 7 1219.92
    {{0x001D537F, 0x00000000, 0x00000000, 0x80020000}},  // 8 -19219.19
    {{0x7D17C946, 0x00000BA8, 0x00000000, 0x00040000}},  // 9 1281828112.2118
    {{0x7D17C946, 0x00000BA8, 0x00000000, 0x00040000}},  // 10 1281828112.2118
    {{0x00000369, 0x00000000, 0x00000000, 0x00000000}},  // 11 873
    {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
      0x00000000}},  // 12 79228162514264337593543950335
    {{0x0001DD09, 0x00000000, 0x00000000,
      0x00110000}},  // 13 0.00000000000122121
    {{0x00140263, 0x00000000, 0x00000000,
      0x00120000}},  // 14 0.000000000001311331
    {{0x0098967F, 0x00000000, 0x00000000, 0x00070000}},  // 15 0.9999999
    {{0x0098967F, 0x00000000, 0x00000000, 0x00070000}},  // 16 0.9999999
    {{0x0098967F, 0x00000000, 0x00000000, 0x80070000}},  // 17 -0.9999999
    {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
      0x80000000}},  // 18 -79228162514264337593543950335
    {{0x4B743118, 0xC29C4065, 0xF1343498,
      0x00190000}},  // 20 7464.8999890549484848686575896
    {{0xE17A7F9D, 0x0DAED47B, 0x00000000,
      0x000C0000}},  // 21 985958.996969684893
    {{0x9925D7EE, 0x8A987A9C, 0x00000000,
      0x000B0000}},  // 22 99868669.86696955886
    {{0x000186A0, 0x00000000, 0x00000000, 0x00030000}},  // 23 100.000
    {{0x007FEBFE, 0x00000000, 0x00000000, 0x00020000}},  // 24 83834.86
    {{0x15C0748C, 0x00000000, 0x00000000, 0x00000000}},  // 25 364934284
    {{0x15C0748C, 0x00000000, 0x00000000, 0x80000000}},  // 26 -364934284
};

static s21_decimal num2[] = {
    {{0x00000005, 0x00000000, 0x00000000, 0x00000000}},  // 0 5
    {{0x00004E2A, 0x00000000, 0x00000000, 0x00000000}},  // 1 20010
    {{0x04FDBA54, 0x00000000, 0x00000000, 0x00000000}},  // 3 83737172
    {{0x00000011, 0x00000000, 0x00000000, 0x80000000}},  // 4 -17
    {{0x9336DD9D, 0x00000032, 0x00000000, 0x80020000}},  // 5 -2172182112.29
    {{0x0000000A, 0x00000000, 0x00000000, 0x00000000}},  // 6 10
    {{0x0116F341, 0x00000000, 0x00000000, 0x80030000}},  // 7 -18281.281
    {{0x00007208, 0x00000000, 0x00000000, 0x00000000}},  // 8 29192
    {{0xCED1C28B, 0x00000032, 0x00000000, 0x00030000}},  // 9 218218218.123
    {{0x7EE8E7D9, 0x00000000, 0x00000000, 0x00050000}},  // 10 21291.92921
    {{0x000D9987, 0x00000000, 0x00000000, 0x00000000}},  // 11 891271
    {{0x00001DE5, 0x00000000, 0x00000000, 0x00010000}},  // 12 765.3
    {{0x000E1209, 0x00000000, 0x00000000,
      0x001B0000}},  // 13 0.000000000000000000000922121
    {{0xEFB25CB0, 0x00000008, 0x00000000, 0x00060000}},  // 14 38381.182128
    {{0x00000001, 0x00000000, 0x00000000, 0x00000000}},  // 15 1
    {{0x3B9AC60D, 0x00000000, 0x00000000, 0x80090000}},  // 16 -0.999998989
    {{0xD4B49FE1, 0x000000E8, 0x00000000, 0x000C0000}},  // 17 1.000001019873
    {{0x00000019, 0x00000000, 0x00000000, 0x00010000}},  // 18 2.5
    {{0x036EACCF, 0x00000000, 0x00000000, 0x00030000}},  // 20 57584.847
    {{0x00004706, 0x00000000, 0x00000000, 0x00030000}},  // 21 18.182
    {{0xA4E4CE45, 0x5D99269E, 0x00000043,
      0x00040000}},  // 22 124267631731831713.3381
    {{0x00000005, 0x00000000, 0x00000000, 0x00000000}},  // 23 5
    {{0x00000002, 0x00000000, 0x00000000, 0x00000000}},  // 24 2
    {{0x0000D8B6, 0x00000000, 0x00000000, 0x00000000}},  // 25 55478
    {{0x000019B2, 0x00000000, 0x00000000, 0x80000000}},  // 26 -6578
};

static s21_decimal result[] = {
    {{0x00000002, 0x00000000, 0x00000000, 0x00000000}},  // 0: 2
    {{0x9BD58676, 0x15E87A6F, 0x00042243,
      0x001C0000}},  // 1: 0.0004997501249375312343828086
    {{0x32BBC141, 0x8D8AFA58, 0x000C911E,
      0x001C0000}},  // 3: 0.0015192416576953422788149569
    {{0x89696969, 0xD80A02F7, 0x284B6DC6,
      0x001B0000}},  // 4: 12.470588235294117647058823529
    {{0xADCFD4F0, 0xD696F916, 0x0000D4BB,
      0x001C0000}},  // 5: 0.0001004606698330394895308016
    {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},  // 6: 0
    {{0x9C29AC03, 0x8B157841, 0x0227FB71,
      0x801C0000}},  // 7: -0.0667305535098990054362164227
    {{0xEF1E9F89, 0xA0889681, 0x1545EC8B,
      0x801C0000}},  // 8: -0.6583718141956700465881063305
    {{0xD2206F19, 0x481D7320, 0xBDCD2196,
      0x001C0000}},  // 9: 5.8740655259557198854380089113
    {{0xF563AF1E, 0x924473AA, 0xC2865EA6,
      0x00180000}},  // 10: 60202.534940317885830506196766
    {{0x39A7B007, 0x0C3825D8, 0x00081A2C,
      0x001C0000}},  // 11: 0.0009795000622706225154863111
    {{0x1AA07F61, 0xB8149B14, 0x2173706D,
      0x00020000}},  // 12: 103525627223656523707753757.13
    {{0x74CC4BF6, 0x387BB251, 0x2ACAC209,
      0x00130000}},  // 13: 1324348973.7247064105469889526
    {{0x8C840219, 0x0000004F, 0x00000000,
      0x001C0000}},  // 14: 0.0000000000000000341659877913
    {{0x0098967F, 0x00000000, 0x00000000, 0x00070000}},  // 15: 0.9999999
    {{0x1AF606CC, 0x18EB582D, 0x204FD04C,
      0x801C0000}},  // 16: -1.0000009110009210219311531724
    {{0xF97B0D96, 0x28B45460, 0x204FCBFF,
      0x801C0000}},  // 17: -0.9999988801281421270713044374
    {{0x66666666, 0x66666666, 0x66666666,
      0x80000000}},  // 18: -31691265005705735037417580134
    {{0x97AB5CFD, 0x3AF46695, 0x04304CAF,
      0x001C0000}},  // 20: 0.1296330610907926608690765053
    {{0xA658CE14, 0x3D31233A, 0xAF37B0CD,
      0x00180000}},  // 21: 54227.202561307056044439555604
    {{0x1A33E97E, 0x6F87AA74, 0x00000000,
      0x001C0000}},  // 22: 0.0000000008036579475698477438
    {{0x00004E20, 0x00000000, 0x00000000, 0x00030000}},  // 23: 20.000
    {{0x003FF5FF, 0x00000000, 0x00000000, 0x00020000}},  // 24: 41917.43
    {{0x000019B2, 0x00000000, 0x00000000, 0x00000000}},  // 25: 6578
    {{0x0000D8B6, 0x00000000, 0x00000000, 0x00000000}},  // 26: 55478
};

static s21_decimal err_num1[] = {
    {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
      0x00000000}},  // 0 79228162514264337593543950335
    {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
      0x00000000}},  // 1 79228162514264337593543950335
    {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
      0x80000000}},  // 2 -79228162514264337593543950335
    {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
      0x80000000}},  // 3 -79228162514264337593543950335
    {{0x204479BE, 0x0000006F, 0x00000000, 0x00040000}},  // 4 47728272.8382
    {{0xFA01F028, 0x00000001, 0x00000000, 0x80020000}},  // 5 -84893983.12
    {{0x00000000, 0x00000000, 0x00000000, 0x00040000}},  // 6 0.0000
};

static s21_decimal err_num2[] = {
    {{0x00000312, 0x00000000, 0x00000000, 0x00090000}},  // 0 0.000000786
    {{0x00000002, 0x00000000, 0x00000000, 0x80020000}},  // 1 -0.02
    {{0x000026A1, 0x00000000, 0x00000000, 0x00060000}},  // 2 0.009889
    {{0x000002FD, 0x00000000, 0x00000000, 0x80050000}},  // 3 -0.00765
    {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},  // 4 0
    {{0x00000000, 0x00000000, 0x00000000, 0x80080000}},  // 5 0.00000000
    {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},  // 6 0
};

static int err_result[] = {
    1, 2, 2, 1, 3, 3, 3,
};

START_TEST(div_test_4) {
  for (size_t i = 0; i < sizeof(num1) / sizeof(s21_decimal); ++i) {
    s21_decimal tmp;
    int ret = s21_div(num1[i], num2[i], &tmp);
    ck_assert_int_eq(tmp.bits[0], result[i].bits[0]);
    ck_assert_int_eq(tmp.bits[1], result[i].bits[1]);
    ck_assert_int_eq(tmp.bits[2], result[i].bits[2]);
    ck_assert_int_eq(tmp.bits[3], result[i].bits[3]);
    ck_assert_int_eq(ret, 0);
  }
}
END_TEST

START_TEST(div_test_5) {
  for (size_t i = 0; i < sizeof(err_num1) / sizeof(s21_decimal); ++i) {
    s21_decimal tmp;
    int ret = s21_div(err_num1[i], err_num2[i], &tmp);
    if (tmp.bits[0] == 0) {
      tmp.bits[0] = 1;
    }
    ck_assert_int_eq(ret, err_result[i]);
  }
}
END_TEST

int main(void) {
  Suite *suite = suite_create("Arithmetic");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, add_test_1);
  tcase_add_test(tc_core, add_test_2);
  tcase_add_test(tc_core, add_test_3);
  tcase_add_test(tc_core, add_test_4);
  tcase_add_test(tc_core, sub_pos_pos);
  tcase_add_test(tc_core, sub_pos_pos2);
  tcase_add_test(tc_core, sub_pos_posE);
  tcase_add_test(tc_core, sub_posE_posE);
  tcase_add_test(tc_core, sub_neg_neg);
  tcase_add_test(tc_core, sub_neg_negE);
  tcase_add_test(tc_core, sub_negE_negE);
  tcase_add_test(tc_core, sub_pos_neg);
  tcase_add_test(tc_core, sub_neg_pos);
  tcase_add_test(tc_core, sub_0_0);
  tcase_add_test(tc_core, sub_pos29_pos);
  tcase_add_test(tc_core, add_pos_pos);
  tcase_add_test(tc_core, add_pos_posE);
  tcase_add_test(tc_core, add_posE_posE);
  tcase_add_test(tc_core, add_neg_neg);
  tcase_add_test(tc_core, add_neg_negE);
  tcase_add_test(tc_core, add_negE_negE);
  tcase_add_test(tc_core, add_pos_neg);
  tcase_add_test(tc_core, add_neg_pos);
  tcase_add_test(tc_core, add_0_0);
  tcase_add_test(tc_core, mul_test_1);
  tcase_add_test(tc_core, mul_test_2);
  tcase_add_test(tc_core, div_test_1);
  tcase_add_test(tc_core, div_test_2);
  tcase_add_test(tc_core, div_test_3);
  tcase_add_test(tc_core, div_test_4);
  tcase_add_test(tc_core, div_test_5);
  suite_add_tcase(suite, tc_core);

  SRunner *runner = srunner_create(suite);

  srunner_run_all(runner, CK_NORMAL);
  int number_failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}