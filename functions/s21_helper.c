#include "../s21_decimal.h"

// Устанавливает бит в позиции pos в числе num
int setBit(unsigned int num, int pos) { return (num | ((unsigned)1 << pos)); }

// Возвращает значение бита в позиции pos в числе num
int getBit(unsigned int num, int pos) {
  return ((num & ((unsigned)1 << pos)) >> pos);
}

// Очищает (устанавливает в 0) бит в позиции pos в числе num
int clearBit(unsigned int num, int pos) {
  return (num & (~((unsigned)1 << pos)));
}

// Инвертирует бит в позиции pos в числе num
int toggleBit(unsigned int num, int pos) {
  return (num ^ ((unsigned)1 << pos));
}

// Возвращает экспоненту (количество десятичных знаков) из числа типа
// s21_decimal
int get_exp(s21_decimal value) { return (value.bits[3] << 10) >> 26; }

// Возвращает знак числа типа s21_decimal (0 - положительное, 1 - отрицательное)
int get_sign(s21_decimal value) { return (value.bits[3] >> 31) & 1; }

// Устанавливает знак числа типа s21_decimal (0 - положительное, 1 -
// отрицательное)
void set_sign(s21_decimal *value, int bit) {
  if (bit) {
    value->bits[3] |= (1u << 31);
  } else {
    value->bits[3] &= ~(1u << 31);
  }
}

s21_decimal *set_scale(s21_decimal *value, int scale) {
  // Устанавливает масштаб числа в указанное значение
  if (scale >= 0 && scale <= 28) {  // Проверка корректности масштаба
    int currentSign = get_sign(*value);  // Сохраняем текущий знак числа
    // Очищаем старшие 8 бит в четвертом элементе bits, чтобы сбросить старый
    // масштаб
    value->bits[3] &= ~(0xFF << 16);
    // Устанавливаем новый масштаб, сдвигая его влево на 16 бит
    value->bits[3] |= (scale << 16);
    // Восстанавливаем знак числа, если он был установлен ранее
    if (currentSign) {
      set_sign(value, 1);
    }
  }
  return value;
}

// Приводит экспоненты двух чисел типа s21_decimal к одному значению,
// изменяя числа и корректируя значения
void equal_exponents(s21_decimal *value_1, s21_decimal *value_2) {
  int exp1 = get_exp(*value_1);
  int exp2 = get_exp(*value_2);
  int decreased = 10;
  int val_num = 2;
  int more_than_once = 0;
  while (exp1 != exp2) {
    if (exp2 > exp1) {
      if (!increase_exponent(value_1)) {
        decreased = decrease_exponent(value_2);
        exp2 = get_exp(*value_2);
        if (!more_than_once && val_num != 2) {
          more_than_once = 1;
        }
        val_num = 0;
      }
      exp1 = get_exp(*value_1);
    } else {
      if (!increase_exponent(value_2)) {
        decreased = decrease_exponent(value_1);
        exp1 = get_exp(*value_1);
        if (!more_than_once && val_num != 2) {
          more_than_once = 1;
        }
        val_num = 1;
      }
      exp2 = get_exp(*value_2);
    }
  }
  if (decreased < 10) {
    if (val_num) {
      if (((getBit(value_1->bits[0], 0) || more_than_once ||
            getBit(value_2->bits[0], 0)) &&
           (decreased == 5)) ||
          (decreased > 5) || (get_sign(*value_1) && decreased)) {
        s21_add(*value_1, (s21_decimal){{1, 0, 0, value_1->bits[3]}}, value_1);
      }
    } else {
      if (((getBit(value_2->bits[0], 0) || more_than_once ||
            getBit(value_1->bits[0], 0)) &&
           (decreased == 5)) ||
          (decreased > 5) || (get_sign(*value_2) && decreased)) {
        s21_add(*value_2, (s21_decimal){{1, 0, 0, value_2->bits[3]}}, value_2);
      }
    }
  }
}

// Увеличивает экспоненту числа типа s21_decimal на 1
int increase_exponent(s21_decimal *value) {
  int res = 1;
  s21_decimal temp = *value;
  if (!mul_by_10(&temp)) {
    *value = temp;
    value->bits[3] += (1 << 16);  // Увеличиваем экспоненту на 1
  } else {
    res = 0;  // Ошибка, если увеличить экспоненту не удалось
  }
  return res;
}

// Уменьшает экспоненту числа типа s21_decimal на 1, возвращает остаток от
// деления на 10
int decrease_exponent(s21_decimal *value) {
  int last = div_by_10(*value, value, 0);
  value->bits[3] -= (1 << 16);  // Уменьшаем экспоненту на 1
  return last;
}

// Умножает число типа s21_decimal на 10
int mul_by_10(s21_decimal *value) {
  int res = 0;
  s21_decimal value_2 = (s21_decimal){
      {value->bits[0] << 1, (value->bits[1] << 1) | getBit(value->bits[0], 31),
       (value->bits[2] << 1) | getBit(value->bits[1], 31), value->bits[3]}};
  s21_decimal value_8 = (s21_decimal){
      {value->bits[0] << 3, (value->bits[1] << 3) | (value->bits[0] >> 29),
       (value->bits[2] << 3) | (value->bits[1] >> 29), value->bits[3]}};
  if (!(value->bits[2] >> 28)) {  // Проверка на переполнение
    s21_add(value_2, value_8, value);
  } else {
    res = 1;  // Ошибка, если произошло переполнение
  }
  return res;
}

// Умножает число типа s21_big_decimal на 10
int big_mul_by_10(s21_big_decimal *value) {
  int res = 0;
  s21_big_decimal value_2 = (s21_big_decimal){
      {value->bits[0] << 1, (value->bits[1] << 1) | getBit(value->bits[0], 31),
       (value->bits[2] << 1) | getBit(value->bits[1], 31),
       (value->bits[3] << 1) | getBit(value->bits[2], 31),
       (value->bits[4] << 1) | getBit(value->bits[3], 31),
       (value->bits[5] << 1) | getBit(value->bits[4], 31)},
      0};
  s21_big_decimal value_8 = (s21_big_decimal){
      {value->bits[0] << 3, (value->bits[1] << 3) | (value->bits[0] >> 29),
       (value->bits[2] << 3) | (value->bits[1] >> 29),
       (value->bits[3] << 3) | (value->bits[2] >> 29),
       (value->bits[4] << 3) | (value->bits[3] >> 29),
       (value->bits[5] << 3) | (value->bits[4] >> 29)},
      0};
  big_add(value_2, value_8, value);
  return res;
}

// Выполняет базовое сложение двух чисел типа s21_decimal,
// корректирует знак и экспоненту результата
int basic_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result,
              int *res) {
  int carry = 0;
  int default_sign = get_sign(value_1);
  for (int i = 0; i < 96; i++) {
    int bit1 = getBit(value_1.bits[i / 32], i % 32);
    int bit2 = getBit(value_2.bits[i / 32], i % 32);
    int sum = bit1 + bit2 + carry;
    carry = getBit(sum, 1);
    if (getBit(sum, 0)) {
      (*result).bits[i / 32] = setBit((*result).bits[i / 32], i % 32);
    }
  }
  if (carry) {
    int last = div_by_10(*result, result, 1);
    if ((getBit(result->bits[0], 0) && (last == 5)) || (last > 5) ||
        (get_sign(*result) && last)) {
      s21_add(*result, (s21_decimal){{1, 0, 0, result->bits[3]}}, result);
    }
  }
  (*result).bits[3] =
      (unsigned)get_sign(value_1) << 31 | (get_exp(value_1) - carry) << 16;
  if (get_exp(*result) > 28) {
    *res = 1 + default_sign;
  }
  return carry;
}

// Выполняет базовое вычитание двух чисел типа s21_decimal,
// корректирует знак и экспоненту результата
void basic_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result,
               int *res) {
  *result = (s21_decimal){{0, 0, 0, 0}};
  int default_sign = get_sign(value_1);
  for (int i = 0; i < 96; i++) {
    int bit1 = getBit(value_1.bits[i / 32], i % 32);
    int bit2 = getBit(value_2.bits[i / 32], i % 32);
    int sum;
    int current_bit = i;
    int flag = 0;
    while ((sum = (bit1 - bit2)) < 0) {
      if ((bit1 = getBit(value_1.bits[current_bit / 32], current_bit % 32))) {
        flag = 1;
        value_1.bits[current_bit / 32] =
            clearBit(value_1.bits[current_bit / 32], current_bit % 32);
        while (current_bit > i) {
          current_bit--;
          value_1.bits[current_bit / 32] =
              setBit(value_1.bits[current_bit / 32], current_bit % 32);
        }
      } else {
        current_bit++;
      }
    }
    if (flag || sum) {
      (*result).bits[i / 32] = setBit((*result).bits[i / 32], i % 32);
    }
  }
  (*result).bits[3] = (unsigned)get_sign(value_1) << 31 | (get_exp(value_1))
                                                              << 16;
  if (get_exp(*result) > 28) {
    *res = 1 + default_sign;
  }
}

// Выполняет умножение двух чисел типа s21_decimal, поддерживающее длинную
// арифметику, и возвращает значение экспоненты
int long_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int exp = 0;
  *result = (s21_decimal){{0, 0, 0, 0}};
  s21_big_decimal long_result = (s21_big_decimal){{0, 0, 0, 0, 0, 0}, 0};
  big_mul(
      (s21_big_decimal){
          {value_1.bits[0], value_1.bits[1], value_1.bits[2], 0, 0, 0}, 0},
      (s21_big_decimal){
          {value_2.bits[0], value_2.bits[1], value_2.bits[2], 0, 0, 0}, 0},
      &long_result);
  exp = big_decimal_to_decimal(long_result, result);
  return exp;
}

// Конвертирует результат длинного умножения (s21_big_decimal) в s21_decimal,
// возвращая значение экспоненты
int big_decimal_to_decimal(s21_big_decimal long_result, s21_decimal *result) {
  s21_big_decimal cpy = long_result;
  int exp = 0;
  int last = 0;
  while (cpy.bits[3] || cpy.bits[4] || cpy.bits[5]) {
    exp++;
    last = big_div_by_10(cpy, &cpy);
  }
  result->bits[0] = cpy.bits[0];
  result->bits[1] = cpy.bits[1];
  result->bits[2] = cpy.bits[2];
  if ((getBit(result->bits[0], 0) && (last == 5)) || (last > 5)) {
    s21_add(*result, (s21_decimal){{1, 0, 0, result->bits[3]}}, result);
  }
  return exp;
}

// Выполняет сложение двух чисел типа s21_big_decimal
void big_add(s21_big_decimal value_1, s21_big_decimal value_2,
             s21_big_decimal *result) {
  *result = (s21_big_decimal){{0, 0, 0, 0, 0, 0}, 0};
  int carry = 0;
  for (int i = 0; i < 192; i++) {
    int bit1 = getBit(value_1.bits[i / 32], i % 32);
    int bit2 = getBit(value_2.bits[i / 32], i % 32);
    int sum = bit1 + bit2 + carry;
    carry = getBit(sum, 1);
    if (getBit(sum, 0)) {
      (*result).bits[i / 32] = setBit((*result).bits[i / 32], i % 32);
    }
  }
}

// Выполняет умножение двух чисел типа s21_big_decimal
void big_mul(s21_big_decimal value_1, s21_big_decimal value_2,
             s21_big_decimal *result) {
  *result = (s21_big_decimal){{0, 0, 0, 0, 0, 0}, 0};
  for (int i = 0; i < 96; i++) {
    int bit1 = getBit(value_1.bits[i / 32], i % 32);
    if (bit1) {
      s21_big_decimal value_to_add = value_2;
      for (int j = 0; j < i; j++) {
        big_shift(&value_to_add);
      }
      big_add(*result, value_to_add, result);
    }
  }
}

// Проверяет возможность переполнения при умножении двух чисел типа s21_decimal,
// возвращает количество битов в старшей части числа
int overflow_check(s21_decimal value_1, s21_decimal value_2) {
  int res = 0;
  int flag = 0;
  for (int i = 95; (i >= 0) && !flag; i--) {
    if (getBit(value_1.bits[i / 32], i % 32)) {
      flag = 1;
      res += i;
    }
  }
  flag = 0;
  for (int i = 95; (i >= 0) && !flag; i--) {
    if (getBit(value_2.bits[i / 32], i % 32)) {
      flag = 1;
      res += i;
    }
  }
  return res;
}

// Выполняет базовое умножение двух чисел типа s21_decimal,
// поддерживает только обычную (не длинную) арифметику
void basic_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  *result = (s21_decimal){{0, 0, 0, 0}};
  for (int i = 0; i < 96; i++) {
    int bit1 = getBit(value_1.bits[i / 32], i % 32);
    if (bit1) {
      s21_decimal value_to_add = value_2;
      for (int j = 0; j < i; j++) {
        shift(&value_to_add);
      }
      value_to_add.bits[3] = 0;
      s21_add(*result, value_to_add, result);
    }
  }
}

// Выполняет деление числа типа s21_big_decimal на 10,
// возвращает остаток от деления на 10
int big_div_by_10(s21_big_decimal value, s21_big_decimal *result) {
  unsigned long buf1 = 0;
  unsigned long buf2 = 0;
  unsigned long buf3 = 0;
  unsigned long buf4 = 0;
  unsigned long buf5 = 0;
  result->bits[5] = value.bits[5] / 10;
  buf1 = ((unsigned long)value.bits[5] % 10) * ((unsigned long)MAX_UINT + 1);
  result->bits[4] = (((unsigned long)value.bits[4] + buf1) / 10);
  buf2 = (((unsigned long)value.bits[4] + buf1) % 10) *
         ((unsigned long)MAX_UINT + 1);
  result->bits[3] = (((unsigned long)value.bits[3] + buf2) / 10);
  buf3 = (((unsigned long)value.bits[3] + buf2) % 10) *
         ((unsigned long)MAX_UINT + 1);
  result->bits[2] = ((unsigned long)value.bits[2] + buf3) / 10;
  buf4 = (((unsigned long)value.bits[2] + buf3) % 10) *
         ((unsigned long)MAX_UINT + 1);
  result->bits[1] = (((unsigned long)value.bits[1] + buf4) / 10);
  buf5 = (((unsigned long)value.bits[1] + buf4) % 10) *
         ((unsigned long)MAX_UINT + 1);
  result->bits[0] = (((unsigned long)value.bits[0] + buf5) / 10);
  return ((unsigned long)value.bits[0] + buf5) % 10;
}

// Выполняет деление числа типа s21_decimal на 10,
// возвращает остаток от деления на 10
int div_by_10(s21_decimal value, s21_decimal *result, int carry) {
  unsigned long buf1 = 0;
  unsigned long buf2 = 0;
  result->bits[2] =
      ((unsigned long)value.bits[2] + carry * ((unsigned long)MAX_UINT + 1)) /
      10;
  buf1 =
      (((unsigned long)value.bits[2] + carry * ((unsigned long)MAX_UINT + 1)) %
       10) *
      ((unsigned long)MAX_UINT + 1);

  result->bits[1] = (((unsigned long)value.bits[1] + buf1) / 10);
  buf2 = (((unsigned long)value.bits[1] + buf1) % 10) *
         ((unsigned long)MAX_UINT + 1);

  result->bits[0] = (((unsigned long)value.bits[0] + buf2) / 10);
  return ((unsigned long)value.bits[0] + buf2) % 10;
}

// Выполняет деление двух чисел типа s21_decimal, возвращает экспоненту
// результата
int remainder_div(s21_decimal value_1, s21_decimal value_2,
                  s21_decimal *result) {
  int cur_exp = get_exp(value_1) - get_exp(value_2);
  int max_shift = 0;
  for (int i = 95; i >= 0 && !max_shift; i--) {
    if (getBit(value_2.bits[i / 32], i % 32)) {
      max_shift = 96 - i;
    }
  }
  *result = (s21_decimal){{0, 0, 0, 0}};
  value_1.bits[3] = 0;
  value_2.bits[3] = 0;
  while (s21_is_greater_or_equal(value_1, value_2)) {
    s21_decimal value_to_add = value_2;
    s21_decimal tmp = value_2;
    int q = 0;
    while (s21_is_greater_or_equal(value_1, value_to_add) && q < max_shift &&
           !getBit(result->bits[2], 31)) {  // b*2^q<=a
      shift(&value_to_add);
      if (q != 0) {
        shift(&tmp);
      }
      q++;
    }
    value_to_add = (s21_decimal){{0, 0, 0, 0}};
    value_to_add.bits[(q - 1) / 32] = ((unsigned)1 << ((q - 1) % 32));
    s21_add(*result, value_to_add, result);
    s21_sub(value_1, tmp, &value_1);
    while (!(is_zero(value_1)) && s21_is_less(value_1, value_2) &&
           (cur_exp < 28) && (!(result->bits[2] >> 28))) {
      mul_by_10(&value_1);
      mul_by_10(result);
      cur_exp++;
    }
  }
  if ((result->bits[2] >> 28) || (cur_exp >= 28)) {
    round_after_division(result, &cur_exp, max_shift, value_1, value_2);
  }
  result->bits[3] = 0;
  return cur_exp;
}

// Выполняет округление после деления двух чисел типа s21_decimal
void round_after_division(s21_decimal *result, int *cur_exp, int max_shift,
                          s21_decimal value_1, s21_decimal value_2) {
  s21_decimal copy = *result;
  s21_big_decimal big_result = (s21_big_decimal){
      {result->bits[0], result->bits[1], result->bits[2], 0, 0}, 0};
  for (int i = 0; i <= 28 - *cur_exp; i++) {
    mul_by_10(&value_1);
    big_mul_by_10(&big_result);
  }
  while (s21_is_greater_or_equal(value_1, value_2)) {
    s21_decimal value_to_add = value_2;
    s21_decimal tmp = value_2;
    int q = 0;
    while (s21_is_greater_or_equal(value_1, value_to_add) && q < max_shift &&
           !getBit(result->bits[2], 31)) {
      shift(&value_to_add);
      if (q != 0) {
        shift(&tmp);
      }
      q++;
    }
    s21_big_decimal value = (s21_big_decimal){{0, 0, 0, 0, 0, 0}, 0};
    value.bits[(q - 1) / 32] = ((unsigned)1 << ((q - 1) % 32));
    big_add(big_result, value, &big_result);
    s21_sub(value_1, tmp, &value_1);
  }
  int exp = big_decimal_to_decimal(big_result, result);
  if (*cur_exp == 28 && (exp == (28 - *cur_exp))) {
    int last = div_by_10(*result, result, 0);
    if ((getBit(result->bits[0], 0) && (last == 5)) || (last > 5) ||
        (get_sign(*result) && last)) {
      s21_add(*result, (s21_decimal){{1, 0, 0, result->bits[3]}}, result);
    }
  }
  if (copy.bits[2] != result->bits[2]) {
    (*cur_exp)++;
  }
}

// Выполняет сдвиг числа типа s21_big_decimal влево на 1 бит
void big_shift(s21_big_decimal *value) {
  *value = (s21_big_decimal){
      {value->bits[0] << 1, (value->bits[1] << 1) | (value->bits[0] >> 31),
       (value->bits[2] << 1) | (value->bits[1] >> 31),
       (value->bits[3] << 1) | (value->bits[2] >> 31),
       (value->bits[4] << 1) | (value->bits[3] >> 31),
       (value->bits[5] << 1) | (value->bits[4] >> 31)},
      value->exp};
}

// Выполняет сдвиг числа типа s21_decimal влево на 1 бит
void shift(s21_decimal *value) {
  *value = (s21_decimal){
      {value->bits[0] << 1, (value->bits[1] << 1) | (value->bits[0] >> 31),
       (value->bits[2] << 1) | (value->bits[1] >> 31), value->bits[3]}};
}

// Устанавливает бит в позиции position в числе destination в значение value (0
// или 1)
void set_bit(unsigned int *destination, unsigned int position,
             unsigned int value) {
  if (value)
    *destination |= ((unsigned)1 << position);
  else
    *destination &= ~((unsigned)1 << position);
}

// Проверяет, является ли число типа s21_decimal нулём
int is_zero(s21_decimal value) {
  return (!value.bits[0] && !value.bits[1] && !value.bits[2]) ? 1 : 0;
}

// Преобразует число типа s21_decimal в s21_big_decimal
void decimal_to_big_decimal(s21_decimal from, s21_big_decimal *to) {
  for (int i = 0; i < 3; i++) {
    to->bits[i] = from.bits[i];
  }
  to->bits[3] = 0;
  to->bits[4] = 0;
  to->bits[5] = 0;
  to->exp = get_exp(from);
}

// Выполняет сдвиг числа типа s21_decimal вправо на 1 бит
void shift_right(s21_decimal *value) {
  int store_bit = 0;
  for (int i = 2; i >= 0; i--) {
    int new_bit = getBit(value->bits[i], 0);
    value->bits[i] = value->bits[i] >> 1;
    set_bit(&(value->bits[i]), 31, store_bit);
    store_bit = new_bit;
  }
}

// Выполняет сдвиг числа типа s21_big_decimal влево на 1 бит
void shift_big_decimal_left(s21_big_decimal *value) {
  int store_bit = 0;
  for (int i = 0; i < 6; i++) {
    int new_bit = getBit(value->bits[i], 31);
    value->bits[i] = value->bits[i] << 1;
    set_bit(&(value->bits[i]), 31, store_bit);
    store_bit = new_bit;
  }
}

// Выполняет сложение двух чисел типа s21_big_decimal для умножения
int add_for_multiply(s21_big_decimal value_1, s21_big_decimal value_2,
                     s21_big_decimal *result) {
  int check = 0;
  int store_bit = 0;
  for (int i = 0; i < 192; i++) {
    int a = getBit(value_2.bits[i / 32], i % 32);
    int b = getBit(value_1.bits[i / 32], i % 32);
    set_bit(&(result->bits[i / 32]), i % 32, a ^ b ^ store_bit);
    store_bit = (a && b) || (b && store_bit) || (a && store_bit);
  }
  if (store_bit) check = 1;
  return check;
}

// Умножает число типа s21_big_decimal на 10
int multiply_10_big(s21_big_decimal *src) {
  s21_big_decimal result = {{0}, 0};
  s21_decimal value_2 = {{10, 0, 0, 0}};
  int check = 0;
  s21_big_decimal tmp_1 = *src;
  while (!is_zero(value_2) && !check) {
    if (getBit(value_2.bits[0], 0))
      check = add_for_multiply(result, tmp_1, &result);
    shift_big_decimal_left(&tmp_1);
    if (getBit(result.bits[5], 31)) check = 1;
    shift_right(&value_2);
  }
  result.exp = src->exp;
  *src = result;
  return check;
}

// Считает количество цифр в числе типа long
int count_digits(long n) {
  int count = 0;
  if (n < 0) {
    n *= -1;
  }
  while (n >= 1) {
    n /= 10;
    count++;
  }
  return count;
}

// Обнуляет структуру s21_decimal
void nulify_dec(s21_decimal *dst) {
  dst->bits[0] = 0;
  dst->bits[1] = 0;
  dst->bits[2] = 0;
  dst->bits[3] = 0;
}

// Нормализует два числа типа s21_big_decimal (приводит к одинаковой экспоненте)
int normalize_big(s21_big_decimal *x1, s21_big_decimal *x2) {
  int check = 0;
  s21_big_decimal *x_fix;
  int diff = 0;
  if (x1->exp <= x2->exp) {
    x_fix = x1;
    diff = x2->exp - x1->exp;
  } else {
    x_fix = x2;
    diff = x1->exp - x2->exp;
  }
  while (diff--) {
    check = multiply_10_big(x_fix);
    if (check) break;
    x_fix->exp += 1;
  }
  return check;
}

int is_valid_float(float x) {
  int result = 1;
  // Проверка на слишком малые значения и некорректные значения
  if ((fabsf(x) > 0 && fabsf(x) < MIN_FLOAT_VALUE) || isnan(x)) result = 0;

  // Проверка на слишком большие значения или бесконечность
  if (fabsf(x) > MAX_FLOAT_VALUE || !isfinite(x)) result = 0;

  if (x != (float)x) result = 0;
  return result;
}

// Приведение числа к формату, чтобы оно представляло целое число
// путем увеличения степени 10, пока число не станет больше или равно 2^21
// Здесь 2^21 примерно соответствует числовому диапазону для нормализации,
// иначе его невозможно сохранить (очень маленькие числа)
void normalize(double *tmp, int *scale) {
  // Цикл, который увеличивает число до тех пор, пока оно не достигнет кратности
  // 2^21
  while (!((int)*tmp / (int)pow(2, 21))) {
    *tmp *= 10;
    (*scale)++;
  }

  *tmp = (float)*tmp;

  // Удаление нулей в конце числа, если масштаб больше нуля для сохранения
  // только значащих цифр Соответственно раз удаляются нули, то масштаб числа
  // также уменьшается
  while (fmod(*tmp, 10) == 0 && *scale > 0) {
    *tmp /= 10;
    (*scale)--;
  }
}

// Функция для сокращения десятичного числа до 7 значащих цифр
void to_7_digits(double *number) {
  int num_digits = 0;
  double tmp = *number;
  double factor = 1.0;

  // Подсчет значащих цифр
  while (tmp >= 1.0) {
    tmp /= 10.0;
    num_digits++;
  }

  // Определение количества лишних цифр
  int excess_digits = num_digits - 7;

  if (excess_digits > 0) {
    // Сокращение до 7 цифр
    factor = pow(10, excess_digits);
    *number = round(*number / factor) * factor;
  }
}

void set_bit_to_mantissa(unsigned int *digit, int index) {
  unsigned int mask = 1U << (index % 32);
  *digit |= mask;
}

// Конвертация числа с плавающей запятой
int process_float(float src, s21_decimal *dst) {
  int result = 0;
  int exponenta = 0, scale = 0;
  int sign = src < 0 ? 1 : 0;
  if (sign) src *= -1.0f;

  // Разделение числа на мантиссу (сохраняется в src) и экспоненту (сохраняется
  // в exponenta)
  frexp(src, &exponenta);
  // frexp() возвращает мантиссу в диапазоне [0.5, 1],
  // Для приведения мантиссы в корректный для работы диапазон [1.0, 2.0]
  // экспонента уменьшается на 1
  --exponenta;

  if (!(exponenta > 95 || exponenta < -94)) {
    double tmp = (double)src;

    // Обработка очень маленьких значений (нормальные по условию не попадут в
    // циклы)
    normalize(&tmp, &scale);

    if (scale > 28) {
      result = 1;
    } else {
      to_7_digits((double *)&tmp);

      float mantissa = frexp(tmp, &exponenta);
      --exponenta;

      // Получение побитового представления мантиссы
      unsigned int mantissa_bits = *((unsigned int *)&mantissa);
      // Установка экспоненты в decimal (т.к. выше нормализованное число было
      // ещё раз поделено на экспоненту и мантиссу, соответственно экспоненту
      // нужно установить, чтобы в сочетании с мантиссой значение было
      // корректным)
      set_bit_to_mantissa(&dst->bits[exponenta / 32], (exponenta % 32));
      for (int i = exponenta - 1, j = 22; j >= 0; i--, j--) {
        if (getBit(mantissa_bits, j))
          set_bit_to_mantissa(&dst->bits[i / 32], (i % 32));
      }

      set_sign(dst, sign);
      set_scale(dst, scale);
    }
  } else
    result = 1;
  return result;
}