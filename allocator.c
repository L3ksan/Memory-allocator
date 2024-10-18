/**
 * @file allocator.c
 * @brief Реализация функций аллокатора памяти.
 *
 * Этот файл содержит реализацию функций для управления динамической памятью.
 */

#include "allocator.h"

#define BUFFER_SIZE 1024 ///< Размер статического буфера для аллокатора

// Определяем массив freed_blocks и переменную freed_count
void* freed_blocks[MAX_FREED_BLOCKS]; ///< Массив для хранения освобожденных блоков
int freed_count = 0; ///< Счетчик освобожденных блоков

static char buffer[BUFFER_SIZE]; ///< Статический буфер для хранения данных
static BlockHeader* free_list = (void*)buffer; ///< Список свободных блоков

/**
 * @brief Инициализирует аллокатор, создавая первый свободный блок.
 */
void init_allocator() {
    free_list->size = BUFFER_SIZE - sizeof(BlockHeader);
    free_list->is_free = 1;
    free_list->next = NULL;
}

/**
 * @brief Находит первый свободный блок, который достаточно велик для размещения.
 * 
 * @param size Размер памяти, который нужно выделить.
 * @return Указатель на найденный свободный блок или NULL при отсутствии подходящего блока.
 */
BlockHeader* find_free_block(size_t size) {
    BlockHeader* current = free_list;
    while (current != NULL && (!current->is_free || current->size < size)) {
        current = current->next;
    }
    return current;
}

/**
 * @brief Разделяет большой блок на два меньших.
 * 
 * @param block Указатель на исходный блок.
 * @param size Размер, который нужно выделить.
 */
void split_block(BlockHeader* block, size_t size) {
    BlockHeader* new_block = (void*)((char*)block + sizeof(BlockHeader) + size);
    new_block->size = block->size - size - sizeof(BlockHeader);
    new_block->is_free = 1;
    new_block->next = block->next;
    block->size = size;
    block->next = new_block;
}

/**
 * @brief Выделяет блок памяти заданного размера.
 * 
 * @param size Размер выделяемой памяти в байтах.
 * @return Указатель на выделенную память или NULL при ошибке.
 */
void* my_malloc(size_t size) {
    if (size <= 0) return NULL;

    BlockHeader* block = find_free_block(size);
    if (!block) return NULL;

    if (block->size > size + sizeof(BlockHeader)) {
        split_block(block, size);
    }
    block->is_free = 0;
    return (void*)((char*)block + sizeof(BlockHeader));
}

/**
 * @brief Освобождает ранее выделенный блок памяти.
 * 
 * @param ptr Указатель на блок памяти, который нужно освободить.
 */
void my_free(void* ptr) {
    if (!ptr) return; // Проверка на NULL

    // Добавляем указатель ptr в массив freed_blocks
    if (freed_count < MAX_FREED_BLOCKS) {
        freed_blocks[freed_count++] = ptr; // Сохраняем освобожденный указатель
    }

    BlockHeader* block = (BlockHeader*)((char*)ptr - sizeof(BlockHeader));
    block->is_free = 1;
}

/**
 * @brief Перевыделяет блок памяти на новый размер.
 * 
 * @param ptr Указатель на блок памяти, который нужно перевыделить.
 * @param size Новый размер блока памяти.
 * @return Указатель на новый блок памяти или NULL при ошибке.
 */
void* my_realloc(void* ptr, size_t size) {
    if (is_pointer_freed(ptr)) {
        return NULL;
    }
    if (!ptr) return my_malloc(size);
    if (size <= 0) {
        my_free(ptr);
        return NULL;
    }

    BlockHeader* block = (BlockHeader*)((char*)ptr - sizeof(BlockHeader));
    if (block->size >= size) return ptr;

    void* new_ptr = my_malloc(size);
    if (new_ptr) {
        for (size_t i = 0; i < block->size; i++) {
            ((char*)new_ptr)[i] = ((char*)ptr)[i];
        }
        my_free(ptr);
    }
    return new_ptr;
}

/**
 * @brief Проверяет, был ли указатель освобожден.
 * 
 * @param ptr Указатель, который нужно проверить.
 * @return true, если указатель был освобожден, иначе false.
 */
bool is_pointer_freed(void* ptr) {
    for (int i = 0; i < freed_count; i++) {
        if (freed_blocks[i] == ptr) {
            return true;
        }
    }
    return false;
}
