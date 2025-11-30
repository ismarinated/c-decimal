#include "../s21_decimal.h"

// Функция s21_from_decimal_to_int конвертирует значение типа s21_decimal в int.
// Параметры:
// - src: исходное значение типа s21_decimal, которое нужно преобразовать.
// - dst: указатель на переменную типа int, куда будет сохранён результат.
// Возвращаемое значение: 0 - OK, 1 - ошибка конвертации.

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int result = 0;

  if (dst == NULL) {
    result = 1;
  } else {
    *dst = 0;
    int sign = get_sign(src);
    // Убираем дробную часть, уменьшая масштаб до 0.
    while (get_exp(src)) {
      decrease_exponent(&src);
    }

    s21_decimal max_int = {{0x7FFFFFFF, 0x0, 0x0, 0x0}};
    s21_decimal min_int = {{0x80000000, 0x0, 0x0, 0x80000000}};

    if ((sign == 1 && s21_is_less(src, min_int)) ||
        (sign == 0 && s21_is_greater(src, max_int)))
      result = 1;

    for (int i = 0; result == 0 && i < 32; i++) {
      if (((src.bits[i / 32] >> i % 32) & 1) != 0) {
        *dst += pow(2, i);
      }
    }

    if (result == 0 && sign == 1) {
      *dst *= -1;
    }
  }

  return result;
}