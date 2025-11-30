#include "../s21_decimal.h"

// Функция s21_from_float_to_decimal конвертирует значение типа float в
// s21_decimal. Параметры:
// - src: исходное значение типа float, которое нужно преобразовать.
// - dst: указатель на переменную типа s21_decimal, куда будет сохранён
// результат. Возвращаемое значение: 0 - OK, 1 - ошибка конвертации.

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int result = 0;
  nulify_dec(dst);

  if (dst != NULL && is_valid_float(src)) {
    if (src == 0.0) {
      if (signbit(src) != 0) set_sign(dst, 1);
    } else {
      result = process_float(src, dst);
    }
  } else {
    result = 1;
  }

  return result;
}