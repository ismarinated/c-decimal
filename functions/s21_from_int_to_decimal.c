#include "../s21_decimal.h"

// Функция s21_from_int_to_decimal конвертирует значение типа int в s21_decimal.
// Параметры:
// - src: исходное значение типа int, которое нужно преобразовать.
// - dst: указатель на переменную типа s21_decimal, куда будет сохранён
// результат. Возвращаемое значение: 0 - OK, 1 - ошибка конвертации.

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int result = 0;

  if (dst != NULL) {
    nulify_dec(dst);
    unsigned int tmp = src;

    if (src < 0) {
      set_sign(dst, 1);
      tmp *= -1;
    }
    dst->bits[0] = tmp;

  } else
    result = 1;

  return result;
}