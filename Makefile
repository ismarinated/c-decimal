# Цели в Makefile:
# - all: форматирование кода, сборка библиотеки и тестов.
# - clean: удаление всех сгенерированных файлов и промежуточных данных.
# - gcov_report: очистка, добавление покрытия кода, выполнение тестов, генерация и открытие отчета по покрытию.
# - add_coverage: добавление флагов для покрытия кода.
# - clang: форматирование кода с использованием clang-format.
# - test: сборка и запуск всех тестов.

# Компилятор, который будет использоваться для сборки
CC = gcc

# Флаги компиляции: 
# -Wall: включить все предупреждения
# -Werror: рассматривать предупреждения как ошибки
# -Wextra: включить дополнительные предупреждения
CFLAGS = -Wall -Werror -Wextra #-fsanitize=unreachable

# Директория, в которой будут размещены все скомпилированные объектные файлы
BUILD_DIR = building

# Директория, в которой хранятся исходные файлы функций
FUNCTIONS_DIR = functions

# Директория, в которой хранятся исходные файлы тестов
TESTS_DIR = tests

# Переменная для флагов покрытия кода, по умолчанию пустая
COVERAGE = 

# Имя создаваемой статической библиотеки
LIB_NAME = s21_decimal.a

# Список всех исходных файлов функций (поиск всех .c файлов в FUNCTIONS_DIR)
SRCS = $(wildcard $(FUNCTIONS_DIR)/*.c)

# Список объектных файлов, соответствующих исходным файлам (в BUILD_DIR)
OBJS = $(patsubst $(FUNCTIONS_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Список всех исходных файлов тестов (поиск всех .c файлов в TESTS_DIR)
TEST_SRCS = $(wildcard $(TESTS_DIR)/*.c)

# Список тестовых исполняемых файлов, которые будут сгенерированы в BUILD_DIR
TEST_EXECUTABLES = $(patsubst $(TESTS_DIR)/%.c,$(BUILD_DIR)/%,$(TEST_SRCS))

# Определение конфигурации сборки в зависимости от операционной системы и архитектуры процессора
ifeq ($(shell uname -s),Darwin)
    ARCH=$(shell uname -m)
    ifeq ($(ARCH),arm64)
        # Для архитектуры ARM на macOS
        CFLAGS += -I/opt/homebrew/include
        LDFLAGS=-L/opt/homebrew/lib -lcheck -lm -lpthread $(COVERAGE)
    else ifeq ($(ARCH),x86_64)
        # Для архитектуры Intel на macOS
        CFLAGS += -I/usr/local/include
        LDFLAGS=-L/usr/local/lib -lcheck -lm -lpthread $(COVERAGE)
    endif
else ifeq ($(shell uname -s),Linux)
    # Для Linux, не зависит от архитектуры
    LDFLAGS=-lcheck -lsubunit -lm -lrt -lpthread $(COVERAGE)
endif

# Цель по умолчанию: сборка библиотеки и тестов после форматирования кода
all: clang $(LIB_NAME) $(TEST_EXECUTABLES)

# Создание статической библиотеки из объектных файлов
$(LIB_NAME): $(OBJS)
	ar rcs $@ $(OBJS)

# Правило для компиляции каждого исходного файла в объектный файл
$(BUILD_DIR)/%.o: $(FUNCTIONS_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Правило для компиляции и линковки каждого тестового файла с библиотекой
$(BUILD_DIR)/%: $(TESTS_DIR)/%.c $(LIB_NAME)
	$(CC) $(CFLAGS) -I. $< $(LIB_NAME) -o $@ $(LDFLAGS)

# Цель для очистки: удаляет все сгенерированные файлы и промежуточные данные
clean:
	rm -rf *.a $(BUILD_DIR)/* $(TESTS_DIR)/*.o tests.info report gcov .clang-format *.gcno *.gcda

# Генерация отчета покрытия кода с использованием lcov и его открытие в браузере
gcov_report: clean add_coverage test
	lcov -o tests.info -c -d ./
	genhtml -o gcov tests.info

# Добавление флагов для покрытия кода
add_coverage:
	$(eval COVERAGE += -fprofile-arcs -ftest-coverage --coverage)

# Форматирование кода с использованием clang-format
clang:
	cp ../materials/linters/.clang-format ./
	clang-format -n *.h functions/*.c tests/*.c
	clang-format -i *.h functions/*.c tests/*.c

# Цель для запуска всех тестов: компиляция и выполнение каждого теста
test: $(BUILD_DIR) $(TEST_EXECUTABLES)
	@for test in $(TEST_EXECUTABLES); do \
		$$test || true; \
	done