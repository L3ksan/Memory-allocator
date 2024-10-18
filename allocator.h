#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>
#include <stdbool.h>

#define MAX_FREED_BLOCKS 100 // Максимальное количество освобожденных блоков

// Объявление массивов и переменных как extern
extern void* freed_blocks[MAX_FREED_BLOCKS]; // Массив для хранения освобожденных блоков
extern int freed_count; // Счетчик освобожденных блоков

// Структура заголовка блока памяти
typedef struct BlockHeader {
    size_t size;
    int is_free;
    struct BlockHeader* next;
} BlockHeader;

// Инициализация аллокатора
void init_allocator();

// Выделение памяти
void* my_malloc(size_t size);

// Освобождение памяти
void my_free(void* ptr);

// Перевыделение памяти
void* my_realloc(void* ptr, size_t size);

// Проверка свободности указателя
bool is_pointer_freed(void* ptr);

#endif // ALLOCATOR_H
