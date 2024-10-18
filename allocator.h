/**
 * @file allocator.h
 * @brief Заголовочный файл для аллокатора памяти.
 *
 * Этот файл содержит объявления функций и структур, используемых для управления динамической памятью.
 */

#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>
#include <stdbool.h>

#define MAX_FREED_BLOCKS 100 ///< Максимальное количество освобожденных блоков

// Объявление массивов и переменных как extern
extern void* freed_blocks[MAX_FREED_BLOCKS]; ///< Массив для хранения освобожденных блоков
extern int freed_count; ///< Счетчик освобожденных блоков

/**
 * @struct BlockHeader
 * @brief Структура заголовка блока памяти.
 * 
 * Структура, описывающая заголовок каждого блока памяти, используемого в аллокаторе.
 */
typedef struct BlockHeader {
    size_t size; ///< Размер блока памяти
    int is_free; ///< Статус свободности блока (1 — свободен, 0 — занят)
    struct BlockHeader* next; ///< Указатель на следующий блок
} BlockHeader;

/**
 * @brief Инициализирует аллокатор памяти.
 */
void init_allocator();

/**
 * @brief Выделяет блок памяти заданного размера.
 * 
 * @param size Размер выделяемой памяти в байтах.
 * @return Указатель на выделенную память или NULL при ошибке.
 */
void* my_malloc(size_t size);

/**
 * @brief Освобождает ранее выделенный блок памяти.
 * 
 * @param ptr Указатель на блок памяти, который нужно освободить.
 */
void my_free(void* ptr);

/**
 * @brief Перевыделяет блок памяти на новый размер.
 * 
 * @param ptr Указатель на блок памяти, который нужно перевыделить.
 * @param size Новый размер блока памяти.
 * @return Указатель на новый блок памяти или NULL при ошибке.
 */
void* my_realloc(void* ptr, size_t size);

/**
 * @brief Проверяет, был ли указатель освобожден.
 * 
 * @param ptr Указатель, который нужно проверить.
 * @return true, если указатель был освобожден, иначе false.
 */
bool is_pointer_freed(void* ptr);

#endif // ALLOCATOR_H
