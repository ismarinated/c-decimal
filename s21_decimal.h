#ifndef _SRC_S21_DECIMAL_H
#define _SRC_S21_DECIMAL_H
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  unsigned int bits[4];
} s21_decimal;

typedef struct {
  unsigned int bits[6];
  unsigned int exp;
} s21_big_decimal;

typedef union {
  unsigned int uin_t;
  float floa_t;
} s21_float_bits;

typedef unsigned int uint32;

#define MINUS 0x80000000
#define MAX_FLOAT_VALUE 7.9228162514264337593543950335e28
#define MIN_FLOAT_VALUE 1e-28f
#define MAX_UINT 4294967295

// stephenp
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

// kraznysc
int s21_is_less(s21_decimal a, s21_decimal b);
int s21_is_less_or_equal(s21_decimal a, s21_decimal b);
int s21_is_greater(s21_decimal a, s21_decimal b);
int s21_is_greater_or_equal(s21_decimal a, s21_decimal b);
int s21_is_equal(s21_decimal a, s21_decimal b);
int s21_is_not_equal(s21_decimal a, s21_decimal b);

// fondapho
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

// geromarc
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

// Вспомогательные функции
void equal_exponents(s21_decimal *value_1, s21_decimal *value_2);
int basic_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result,
              int *res);
void basic_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result,
               int *res);
int getBit(unsigned int num, int pos);
int increase_exponent(s21_decimal *value);
int mul_by_10(s21_decimal *value);
int big_mul_by_10(s21_big_decimal *value);
int decrease_exponent(s21_decimal *value);
void basic_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int overflow_check(s21_decimal value_1, s21_decimal value_2);
int remainder_div(s21_decimal value_1, s21_decimal value_2,
                  s21_decimal *result);
void shift(s21_decimal *value);
void big_shift(s21_big_decimal *value);
void big_add(s21_big_decimal value_1, s21_big_decimal value_2,
             s21_big_decimal *result);
void big_mul(s21_big_decimal value_1, s21_big_decimal value_2,
             s21_big_decimal *result);
void nulify_dec(s21_decimal *dst);
int count_digits(long n);
int normalize_big(s21_big_decimal *x1, s21_big_decimal *x2);
int multiply_10_big(s21_big_decimal *src);
int add_for_multiply(s21_big_decimal value_1, s21_big_decimal value_2,
                     s21_big_decimal *result);
void shift_big_decimal_left(s21_big_decimal *value);
void shift_right(s21_decimal *value);
void decimal_to_big_decimal(s21_decimal from, s21_big_decimal *to);
int is_zero(s21_decimal value);
void set_bit(unsigned int *destination, unsigned int position,
             unsigned int value);
int big_div_by_10(s21_big_decimal value, s21_big_decimal *result);
int long_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int big_decimal_to_decimal(s21_big_decimal long_result, s21_decimal *result);
int div_by_10(s21_decimal value, s21_decimal *result, int carry);
void round_after_division(s21_decimal *result, int *cur_exp, int max_shift,
                          s21_decimal value_1, s21_decimal value_2);
int get_exp(s21_decimal value);
int get_sign(s21_decimal value);
void set_sign(s21_decimal *value, int bit);
int setBit(unsigned int num, int pos);
int clearBit(unsigned int num, int pos);
int toggleBit(unsigned int num, int pos);
s21_decimal *set_scale(s21_decimal *value, int scale);
int is_valid_float(float x);
void normalize(double *tmp, int *scale);
void to_7_digits(double *number);
void set_bit_to_mantissa(unsigned int *digit, int index);
int process_float(float src, s21_decimal *dst);

#endif  // _SRC_S21_DECIMAL_H