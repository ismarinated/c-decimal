#include "../s21_decimal.h"

// Функция s21_is_less_or_equal проверяет, меньше ли или равно первое число по
// сравнению со вторым. Параметры:
// - value_1: первое число для сравнения.
// - value_2: второе число для сравнения.
// Возвращаемое значение: 1, если value_1 меньше или равно value_2, и 0 в
// противном случае.

int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2) {
  // Используем функцию s21_is_greater для определения результата.
  // Если value_1 не больше value_2, то value_1 меньше или равно value_2.
  return !s21_is_greater(value_1, value_2);
}