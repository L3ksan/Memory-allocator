#include "allocator.h"

#define BUFFER_SIZE 1024

// Определяем массив freed_blocks и переменную freed_count
void* freed_blocks[MAX_FREED_BLOCKS]; // Массив для хранения освобожденных блоков
int freed_count = 0; // Счетчик освобожденных блоков

static char buffer[BUFFER_SIZE]; // Статический буфер для хранения данных
static BlockHeader* free_list = (void*)buffer; // Список свободных блоков

// Инициализация аллокатора
void init_allocator() {
    free_list->size = BUFFER_SIZE - sizeof(BlockHeader);
    free_list->is_free = 1;
    free_list->next = NULL;
}

// Функция поиска свободного блока
BlockHeader* find_free_block(size_t size) {
    BlockHeader* current = free_list;
    while (current != NULL && (!current->is_free || current->size < size)) {
        current = current->next;
    }
    return current;
}

// Функция разделения блока на два
void split_block(BlockHeader* block, size_t size) {
    BlockHeader* new_block = (void*)((char*)block + sizeof(BlockHeader) + size);
    new_block->size = block->size - size - sizeof(BlockHeader);
    new_block->is_free = 1;
    new_block->next = block->next;
    block->size = size;
    block->next = new_block;
}

// Выделение памяти
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

// Освобождение памяти
void my_free(void* ptr) {
    if (!ptr) return; // Проверка на NULL

    // Добавляем указатель ptr в массив freed_blocks
    if (freed_count < MAX_FREED_BLOCKS) {
        freed_blocks[freed_count++] = ptr; // Сохраняем освобожденный указатель
    }

    BlockHeader* block = (BlockHeader*)((char*)ptr - sizeof(BlockHeader));
    block->is_free = 1;
}

// Перевыделение памяти
void* my_realloc(void* ptr, size_t size) {
    // Проверка, был ли указатель освобожден
    if (is_pointer_freed(ptr)) {
        return NULL; // Запрет на переопределение освобожденной памяти
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

// Функция для проверки, был ли указатель освобожден
bool is_pointer_freed(void* ptr) {
    for (int i = 0; i < freed_count; i++) {
        if (freed_blocks[i] == ptr) {
            return true; // Указатель был освобожден
        }
    }
    return false; // Указатель не был освобожден
}
