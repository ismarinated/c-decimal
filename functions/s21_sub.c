#include "../s21_decimal.h"

// Функция s21_sub выполняет вычитание числа value_2 из числа value_1.
// Параметры:
// - value_1: уменьшаемое число.
// - value_2: вычитаемое число.
// - result: указатель на переменную, в которую будет сохранён результат
// вычитания. Возвращаемое значение: код результата операции.

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;  // Переменная для хранения кода результата операции.

  // Инициализируем результат нулями.
  *result = (s21_decimal){{0, 0, 0, 0}};

  // Приводим экспоненты обоих чисел к одному значению.
  equal_exponents(&value_1, &value_2);

  // Если знаки у обоих чисел одинаковы.
  if (get_sign(value_1) == get_sign(value_2)) {
    // Если оба числа отрицательные.
    if (get_sign(value_1)) {
      // Если value_1 меньше или равно value_2, вычитаем value_2 из value_1.
      if (s21_is_less_or_equal(value_1, value_2)) {
        basic_sub(value_1, value_2, result, &res);
      } else {
        // Иначе вычитаем value_1 из value_2 и инвертируем знак результата.
        basic_sub(value_2, value_1, result, &res);
        (*result).bits[3] = toggleBit((*result).bits[3], 31);
      }
    } else {
      // Если оба числа положительные.
      // Если value_1 больше или равно value_2, вычитаем value_2 из value_1.
      if (s21_is_greater_or_equal(value_1, value_2)) {
        basic_sub(value_1, value_2, result, &res);
      } else {
        // Иначе вычитаем value_1 из value_2 и инвертируем знак результата.
        basic_sub(value_2, value_1, result, &res);
        (*result).bits[3] = toggleBit((*result).bits[3], 31);
      }
    }
  } else {
    // Если знаки у чисел разные, сводим операцию к сложению.
    if (get_sign(value_1) > get_sign(value_2)) {
      value_1.bits[3] = setBit(value_2.bits[3], 31);
    } else {
      value_2.bits[3] = setBit(value_1.bits[3], 31);
    }
    basic_add(value_1, value_2, result, &res);
  }

  return res;
}