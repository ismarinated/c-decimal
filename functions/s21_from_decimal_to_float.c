#include "../s21_decimal.h"

// Функция s21_from_decimal_to_float конвертирует число типа s21_decimal в
// float. Параметры:
// - src: исходное значение типа s21_decimal, которое нужно преобразовать.
// - dst: указатель на переменную типа float, куда будет сохранён результат.
// Возвращаемое значение: 0 - OK, 1 - ошибка конвертации.

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int result = 0;

  if (dst == NULL) {
    result = 1;
  } else {
    *dst = (float)0;
    int sign = get_sign(src);

    double tmp = 0;

    for (int i = 0; i < 96; i++) {
      if (((src.bits[i / 32] >> i % 32) & 1) != 0) {
        tmp += (double)pow(2, i);
      }
    }
    int scale = get_exp(src);

    while (scale > 0) {
      tmp /= 10.0;
      scale--;
    }
    *dst = (float)tmp;

    if (sign == 1) *dst *= -1.0f;
  }

  return result;
}